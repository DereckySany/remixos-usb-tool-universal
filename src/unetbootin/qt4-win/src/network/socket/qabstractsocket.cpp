/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

//#define QABSTRACTSOCKET_DEBUG

/*!
    \class QAbstractSocket

    \brief The QAbstractSocket class provides the base functionality
    common to all socket types.

    \reentrant
    \ingroup io
    \module network

    QAbstractSocket is the base class for QTcpSocket and QUdpSocket
    and contains all common functionality of these two classes. If
    you need a socket, you have two options:

    \list
    \i  Instantiate QTcpSocket or QUdpSocket.
    \i  Create a native socket descriptor, instantiate
        QAbstractSocket, and call setSocketDescriptor() to wrap the
        native socket.
    \endlist

    TCP (Transmission Control Protocol) is a reliable,
    stream-oriented, connection-oriented transport protocol. UDP
    (User Datagram Protocol) is an unreliable, datagram-oriented,
    connectionless protocol. In practice, this means that TCP is
    better suited for continuous transmission of data, whereas the
    more lightweight UDP can be used when reliability isn't
    important.

    QAbstractSocket's API unifies most of the differences between the
    two protocols. For example, although UDP is connectionless,
    connectToHost() establishes a virtual connection for UDP sockets,
    enabling you to use QAbstractSocket in more or less the same way
    regardless of the underlying protocol. Internally,
    QAbstractSocket remembers the address and port passed to
    connectToHost(), and functions like read() and write() use these
    values.

    At any time, QAbstractSocket has a state (returned by
    state()). The initial state is UnconnectedState. After
    calling connectToHost(), the socket first enters
    HostLookupState. If the host is found, QAbstractSocket enters
    ConnectingState and emits the hostFound() signal. When the
    connection has been established, it enters ConnectedState and
    emits connected(). If an error occurs at any stage, error() is
    emitted. Whenever the state changes, stateChanged() is emitted.
    For convenience, isValid() returns true if the socket is ready for
    reading and writing, but note that the socket's state must be
    ConnectedState before reading and writing can occur.

    Read or write data by calling read() or write(), or use the
    convenience functions readLine() and readAll(). QAbstractSocket
    also inherits getChar(), putChar(), and ungetChar() from
    QIODevice, which work on single bytes. For every chunk of data
    that has been written to the socket, the bytesWritten() signal is
    emitted.

    The readyRead() signal is emitted every time a new chunk of data
    has arrived. bytesAvailable() then returns the number of bytes
    that are available for reading. Typically, you would connect the
    readyRead() signal to a slot and read all available data there.
    If you don't read all the data at once, the remaining data will
    still be available later, and any new incoming data will be
    appended to QAbstractSocket's internal read buffer. To limit the
    size of the read buffer, call setReadBufferSize().

    To close the socket, call disconnectFromHost(). QAbstractSocket enters
    QAbstractSocket::ClosingState, then emits closing(). After all pending
    data has been written to the socket, QAbstractSocket actually closes the
    socket, enters QAbstractSocket::ClosedState, and emits disconnected(). If
    you want to abort a connection immediately, discarding all pending data,
    call abort() instead. If the remote host closes the connection,
    QAbstractSocket will emit error(QAbstractSocket::RemoteHostClosedError),
    during which the socket state will still be ConnectedState, and then the
    disconnected() signal will be emitted.

    The port and address of the connected peer is fetched by calling
    peerPort() and peerAddress(). peerName() returns the host name of
    the peer, as passed to connectToHost(). localPort() and
    localAddress() return the port and address of the local socket.

    QAbstractSocket provides a set of functions that suspend the
    calling thread until certain signals are emitted. These functions
    can be used to implement blocking sockets:

    \list
    \o waitForConnected() blocks until a connection has been established.

    \o waitForReadyRead() blocks until new data is available for
    reading.

    \o waitForBytesWritten() blocks until one payload of data has been
    written to the socket.

    \o waitForDisconnected() blocks until the connection has closed.
    \endlist

    We show an example:

    \snippet doc/src/snippets/network/tcpwait.cpp 0

    If \l{QIODevice::}{waitForReadyRead()} returns false, the
    connection has been closed or an error has occurred.

    Programming with a blocking socket is radically different from
    programming with a non-blocking socket. A blocking socket doesn't
    require an event loop and typically leads to simpler code.
    However, in a GUI application, blocking sockets should only be
    used in non-GUI threads, to avoid freezing the user interface.
    See the \l network/fortuneclient and \l network/blockingfortuneclient
    examples for an overview of both approaches.

    QAbstractSocket can be used with QTextStream and QDataStream's
    stream operators (operator<<() and operator>>()). There is one
    issue to be aware of, though: You must make sure that enough data
    is available before attempting to read it using operator>>().

    \sa QFtp, QHttp, QTcpServer
*/

/*!
    \fn void QAbstractSocket::hostFound()

    This signal is emitted after connectToHost() has been called and
    the host lookup has succeeded.

    \sa connected()
*/

/*!
    \fn void QAbstractSocket::connected()

    This signal is emitted after connectToHost() has been called and
    a connection has been successfully established.

    \sa connectToHost(), disconnected()
*/

/*!
    \fn void QAbstractSocket::disconnected()

    This signal is emitted when the socket has been disconnected.
    
    \warning If you need to delete the sender() of this signal in a slot connected
    to it, use the \l{QObject::deleteLater()}{deleteLater()} function.

    \sa connectToHost(), disconnectFromHost(), abort()
*/

/*!
    \fn void QAbstractSocket::error(QAbstractSocket::SocketError socketError)

    This signal is emitted after an error occurred. The \a socketError
    parameter describes the type of error that occurred.

    QAbstractSocket::SocketError is not a registered metatype, so for queued
    connections, you will have to register it with Q_REGISTER_METATYPE.

    \sa error(), errorString()
*/

/*!
    \fn void QAbstractSocket::stateChanged(QAbstractSocket::SocketState socketState)

    This signal is emitted whenever QAbstractSocket's state changes.
    The \a socketState parameter is the new state.

    QAbstractSocket::SocketState is not a registered metatype, so for queued
    connections, you will have to register it with Q_REGISTER_METATYPE.

    \sa state()
*/

/*!
    \fn void QAbstractSocket::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
    \since 4.3

    This signal can be emitted when a \a proxy that requires
    authentication is used. The \a authenticator object can then be
    filled in with the required details to allow authentication and
    continue the connection.

    \note It is not possible to use a QueuedConnection to connect to
    this signal, as the connection will fail if the authenticator has
    not been filled in with new information when the signal returns.

    \sa QAuthenticator, QNetworkProxy
*/

/*!
    \enum QAbstractSocket::NetworkLayerProtocol

    This enum describes the network layer protocol values used in Qt.

    \value IPv4Protocol IPv4
    \value IPv6Protocol IPv6
    \value UnknownNetworkLayerProtocol Other than IPv4 and IPv6

    \sa QHostAddress::protocol()
*/

/*!
    \enum QAbstractSocket::SocketType

    This enum describes the transport layer protocol.

    \value TcpSocket TCP
    \value UdpSocket UDP
    \value UnknownSocketType Other than TCP and UDP

    \sa QAbstractSocket::socketType()
*/

/*!
    \enum QAbstractSocket::SocketError

    This enum describes the socket errors that can occur.

    \value ConnectionRefusedError The connection was refused by the
           peer (or timed out).
    \value RemoteHostClosedError The remote host closed the
           connection. Note that the client socket (i.e., this socket)
           will be closed after the remote close notification has
           been sent.
    \value HostNotFoundError The host address was not found.
    \value SocketAccessError The socket operation failed because the
           application lacked the required privileges.
    \value SocketResourceError The local system ran out of resources
           (e.g., too many sockets).
    \value SocketTimeoutError The socket operation timed out.
    \value DatagramTooLargeError The datagram was larger than the
           operating system's limit (which can be as low as 8192
           bytes).
    \value NetworkError An error occurred with the network (e.g., the
           network cable was accidentally plugged out).
    \value AddressInUseError The address specified to QUdpSocket::bind() is
           already in use and was set to be exclusive.
    \value SocketAddressNotAvailableError The address specified to
           QUdpSocket::bind() does not belong to the host.
    \value UnsupportedSocketOperationError The requested socket operation is
           not supported by the local operating system (e.g., lack of
           IPv6 support).
    \value ProxyAuthenticationRequiredError The socket is using a proxy, and
           the proxy requires authentication.
    \value SslHandshakeFailedError The SSL/TLS handshake failed, so
           the connection was closed (only used in QSslSocket)
    \value UnknownSocketError An unidentified error occurred.
    \value UnfinishedSocketOperationError Used by QAbstractSocketEngine only,
           The last operation attempted has not finished yet (still in progress in
            the background).

    \sa QAbstractSocket::error()
*/

/*!
    \enum QAbstractSocket::SocketState

    This enum describes the different states in which a socket can be.

    \value UnconnectedState The socket is not connected.
    \value HostLookupState The socket is performing a host name lookup.
    \value ConnectingState The socket has started establishing a connection.
    \value ConnectedState A connection is established.
    \value BoundState The socket is bound to an address and port (for servers).
    \value ClosingState The socket is about to close (data may still
    be waiting to be written).
    \value ListeningState For internal use only.
    \omitvalue Idle
    \omitvalue HostLookup
    \omitvalue Connecting
    \omitvalue Connected
    \omitvalue Closing
    \omitvalue Connection

    \sa QAbstractSocket::state()
*/

#include "qabstractsocket.h"
#include "qabstractsocket_p.h"

#include <qabstracteventdispatcher.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include <qhostinfo.h>
#include <qmetaobject.h>
#include <qpointer.h>
#include <qtimer.h>

#ifndef QT_NO_OPENSSL
#include <QtNetwork/qsslsocket.h>
#endif

#include <private/qthread_p.h>

#ifdef QABSTRACTSOCKET_DEBUG
#include <qdebug.h>
#endif

#include <time.h>

#define Q_CHECK_SOCKETENGINE(returnValue) do { \
    if (!d->socketEngine) { \
        return returnValue; \
    } } while (0)

#ifndef QABSTRACTSOCKET_BUFFERSIZE
#define QABSTRACTSOCKET_BUFFERSIZE 32768
#endif
#define QT_CONNECT_TIMEOUT 30000
#define QT_TRANSFER_TIMEOUT 120000

QT_BEGIN_NAMESPACE

#if defined QABSTRACTSOCKET_DEBUG
QT_BEGIN_INCLUDE_NAMESPACE
#include <qstring.h>
#include <ctype.h>
QT_END_INCLUDE_NAMESPACE

/*
    Returns a human readable representation of the first \a len
    characters in \a data.
*/
static QByteArray qt_prettyDebug(const char *data, int len, int maxLength)
{
    if (!data) return "(null)";
    QByteArray out;
    for (int i = 0; i < len; ++i) {
        char c = data[i];
        if (isprint(int(uchar(c)))) {
            out += c;
        } else switch (c) {
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            QString tmp;
            tmp.sprintf("\\%o", c);
            out += tmp.toLatin1();
        }
    }

    if (len < maxLength)
        out += "...";

    return out;
}
#endif

/*! \internal

    Constructs a QAbstractSocketPrivate. Initializes all members.
*/
QAbstractSocketPrivate::QAbstractSocketPrivate()
    : readSocketNotifierCalled(false),
      readSocketNotifierState(false),
      readSocketNotifierStateSet(false),
      emittedReadyRead(false),
      emittedBytesWritten(false),
      closeCalled(false),
      pendingClose(false),
      port(0),
      localPort(0),
      peerPort(0),
      socketEngine(0),
      cachedSocketDescriptor(-1),
      readBufferMaxSize(0),
      readBuffer(QABSTRACTSOCKET_BUFFERSIZE),
      writeBuffer(QABSTRACTSOCKET_BUFFERSIZE),
      isBuffered(false),
      blockingTimeout(30000),
      connectTimer(0),
      connectTimeElapsed(0),
      hostLookupId(-1),
      state(QAbstractSocket::UnconnectedState),
      socketError(QAbstractSocket::UnknownSocketError)
#ifndef QT_NO_NETWORKPROXY
    , proxy(0)
#endif
{
}

/*! \internal

    Destructs the QAbstractSocket. If the socket layer is open, it
    will be reset.
*/
QAbstractSocketPrivate::~QAbstractSocketPrivate()
{
#ifndef QT_NO_NETWORKPROXY
    delete proxy;
#endif
}

/*! \internal

    Resets the socket layer, clears the read and write buffers and
    deletes any socket notifiers.
*/
void QAbstractSocketPrivate::resetSocketLayer()
{
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::resetSocketLayer()");
#endif

    if (socketEngine) {
        socketEngine->close();
        socketEngine->disconnect();
        delete socketEngine;
        socketEngine = 0;
        cachedSocketDescriptor = -1;
    }
    if (connectTimer) {
        connectTimer->stop();
        delete connectTimer;
        connectTimer = 0;
    }
}

/*! \internal

    Initializes the socket layer to by of type \a type, using the
    network layer protocol \a protocol. Resets the socket layer first
    if it's already initialized. Sets up the socket notifiers.
*/
bool QAbstractSocketPrivate::initSocketLayer(const QHostAddress &host, QAbstractSocket::SocketType type)
{
    Q_Q(QAbstractSocket);
#if defined (QABSTRACTSOCKET_DEBUG)
    QString typeStr;
    if (type == QAbstractSocket::TcpSocket) typeStr = "TcpSocket";
    else if (type == QAbstractSocket::UdpSocket) typeStr = "UdpSocket";
    else typeStr = "UnknownSocketType";
    QString protocolStr;
    if (host.protocol() == QAbstractSocket::IPv4Protocol) protocolStr = "IPv4Protocol";
    else if (host.protocol() == QAbstractSocket::IPv6Protocol) protocolStr = "IPv6Protocol";
    else protocolStr = "UnknownNetworkLayerProtocol";
#endif

    resetSocketLayer();
    socketEngine = QAbstractSocketEngine::createSocketEngine(host, type, q);
    if (!socketEngine->initialize(type, host.protocol())) {
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocketPrivate::initSocketLayer(%s, %s) failed (%s)",
               typeStr.toLatin1().constData(), protocolStr.toLatin1().constData(),
               socketEngine->errorString().toLatin1().constData());
#endif
        socketError = socketEngine->error();
	q->setErrorString(socketEngine->errorString());
        return false;
    }

    cachedSocketDescriptor = socketEngine->socketDescriptor();

    if (threadData->eventDispatcher)
        socketEngine->setReceiver(this);

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::initSocketLayer(%s, %s) success",
           typeStr.toLatin1().constData(), protocolStr.toLatin1().constData());
#endif
    return true;
}

/*! \internal

    Slot connected to the read socket notifier. This slot is called
    when new data is available for reading, or when the socket has
    been closed. Handles recursive calls.
*/
bool QAbstractSocketPrivate::canReadNotification()
{
    Q_Q(QAbstractSocket);
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::canReadNotification()");
#endif

    // Prevent recursive calls
    if (readSocketNotifierCalled) {
        if (!readSocketNotifierStateSet) {
            readSocketNotifierStateSet = true;
            readSocketNotifierState = socketEngine->isReadNotificationEnabled();
            socketEngine->setReadNotificationEnabled(false);
        }
    }
    readSocketNotifierCalled = true;

    if (!isBuffered)
        socketEngine->setReadNotificationEnabled(false);

    // If buffered, read data from the socket into the read buffer
    qint64 newBytes = 0;
    if (isBuffered) {
        // Return if there is no space in the buffer
        if (readBufferMaxSize && readBuffer.size() >= readBufferMaxSize) {
#if defined (QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocketPrivate::canReadNotification() buffer is full");
#endif
            readSocketNotifierCalled = false;
            return false;
        }

        // If reading from the socket fails after getting a read
        // notification, close the socket.
        newBytes = readBuffer.size();
        if (!readFromSocket()) {
#if defined (QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocketPrivate::canReadNotification() disconnecting socket");
#endif
            q->disconnectFromHost();
            readSocketNotifierCalled = false;
            return false;
        }
        newBytes = readBuffer.size() - newBytes;

        // If read buffer is full, disable the read socket notifier.
        if (readBufferMaxSize && readBuffer.size() == readBufferMaxSize) {
            socketEngine->setReadNotificationEnabled(false);
        }
    }

    // only emit readyRead() when not recursing, and only if there is data available
    bool hasData = newBytes > 0
#ifndef QT_NO_UDPSOCKET
        || (!isBuffered && socketEngine && socketEngine->hasPendingDatagrams())
#endif
        ;

    if (!emittedReadyRead && hasData) {
        emittedReadyRead = true;
        emit q->readyRead();
        emittedReadyRead = false;
    }

    // If we were closed as a result of the readyRead() signal,
    // return.
    if (state == QAbstractSocket::UnconnectedState || state == QAbstractSocket::ClosingState) {
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocketPrivate::canReadNotification() socket is closing - returning");
#endif
        readSocketNotifierCalled = false;
        return true;
    }

    if (!hasData && socketEngine)
        socketEngine->setReadNotificationEnabled(true);

    // reset the read socket notifier state if we reentered inside the
    // readyRead() connected slot.
    if (readSocketNotifierStateSet && socketEngine &&
        readSocketNotifierState != socketEngine->isReadNotificationEnabled()) {
        socketEngine->setReadNotificationEnabled(readSocketNotifierState);
        readSocketNotifierStateSet = false;
    }
    readSocketNotifierCalled = false;
    return true;
}

/*! \internal

    Slot connected to the write socket notifier. It's called during a
    delayed connect or when the socket is ready for writing.
*/
bool QAbstractSocketPrivate::canWriteNotification()
{
#if defined (Q_OS_WIN)
    if (socketEngine && socketEngine->isWriteNotificationEnabled())
        socketEngine->setWriteNotificationEnabled(false);
#endif

    // If in connecting state, check if the connection has been
    // established, otherwise flush pending data.
    if (state == QAbstractSocket::ConnectingState) {
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocketPrivate::canWriteNotification() testing connection");
#endif
        _q_testConnection();
        return false;
    }

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::canWriteNotification() flushing");
#endif
    int tmp = writeBuffer.size();
    flush();

    if (socketEngine) {
#if defined (Q_OS_WIN)
	if (!writeBuffer.isEmpty())
	    socketEngine->setWriteNotificationEnabled(true);
#else
	if (writeBuffer.isEmpty())
	    socketEngine->setWriteNotificationEnabled(false);
#endif
    }

    return (writeBuffer.size() < tmp);
}

/*! \internal

    Writes pending data in the write buffers to the socket. The
    function writes as much as it can without blocking.

    It is usually invoked by canWriteNotification after one or more
    calls to write().

    Emits bytesWritten().
*/
bool QAbstractSocketPrivate::flush()
{
    Q_Q(QAbstractSocket);
    if (!socketEngine || !socketEngine->isValid() || writeBuffer.isEmpty()) {
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::flush() nothing to do: valid ? %s, writeBuffer.isEmpty() ? %s",
           socketEngine->isValid() ? "yes" : "no", writeBuffer.isEmpty() ? "yes" : "no");
#endif
        return false;
    }

    int nextSize = writeBuffer.nextDataBlockSize();
    const char *ptr = writeBuffer.readPointer();

    // Attempt to write it all in one chunk.
    qint64 written = socketEngine->write(ptr, nextSize);
    if (written < 0) {
        socketError = socketEngine->error();
        q->setErrorString(socketEngine->errorString());
        emit q->error(socketError);
        // an unexpected error so close the socket.
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug() << "QAbstractSocketPrivate::flush() write error, aborting." << socketEngine->errorString();
#endif
        q->abort();
        return false;
    }

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::flush() %lld bytes written to the network",
           written);
#endif

    // Remove what we wrote so far.
    writeBuffer.free(written);
    if (written > 0) {
        // Don't emit bytesWritten() recursively.
        if (!emittedBytesWritten) {
            emittedBytesWritten = true;
            emit q->bytesWritten(written);
            emittedBytesWritten = false;
        }
    }

    if (writeBuffer.isEmpty() && socketEngine && socketEngine->isWriteNotificationEnabled())
        socketEngine->setWriteNotificationEnabled(false);
    if (state == QAbstractSocket::ClosingState)
        q->close();

    return true;
}

/*! \internal

    Slot connected to QHostInfo::lookupHost() in connectToHost(). This
    function starts the process of connecting to any number of
    candidate IP addresses for the host, if it was found. Calls
    _q_connectToNextAddress().
*/
void QAbstractSocketPrivate::_q_startConnecting(const QHostInfo &hostInfo)
{
    Q_Q(QAbstractSocket);
    if (state == QAbstractSocket::ConnectingState || state == QAbstractSocket::ConnectedState)
        return;

    addresses = hostInfo.addresses();

#if defined(QABSTRACTSOCKET_DEBUG)
    QString s = "{";
    for (int i = 0; i < addresses.count(); ++i) {
        if (i != 0) s += ", ";
        s += addresses.at(i).toString();
    }
    s += "}";
    qDebug("QAbstractSocketPrivate::_q_startConnecting(hostInfo == %s)", s.toLatin1().constData());
#endif

    // Try all addresses twice.
    addresses += addresses;

    // If there are no addresses in the host list, report this to the
    // user.
    if (addresses.isEmpty()) {
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocketPrivate::_q_startConnecting(), host not found");
#endif
        state = QAbstractSocket::UnconnectedState;
        socketError = QAbstractSocket::HostNotFoundError;
        q->setErrorString(QLatin1String(QT_TRANSLATE_NOOP("QAbstractSocket", "Host not found")));
        emit q->stateChanged(state);
        emit q->error(QAbstractSocket::HostNotFoundError);
        return;
    }

    // Enter Connecting state (see also sn_write, which is called by
    // the write socket notifier after connect())
    state = QAbstractSocket::ConnectingState;
    emit q->stateChanged(state);

    // Report the successful host lookup
    emit q->hostFound();

    // Reset the total time spent connecting.
    connectTimeElapsed = 0;

    // The addresses returned by the lookup will be tested one after
    // another by _q_connectToNextAddress().
    _q_connectToNextAddress();
}

/*! \internal

    Called by a queued or direct connection from _q_startConnecting() or
    _q_testConnection(), this function takes the first address of the
    pending addresses list and tries to connect to it. If the
    connection succeeds, QAbstractSocket will emit
    connected(). Otherwise, error(ConnectionRefusedError) or
    error(SocketTimeoutError) is emitted.
*/
void QAbstractSocketPrivate::_q_connectToNextAddress()
{
    Q_Q(QAbstractSocket);
    do {
        // Check for more pending addresses
        if (addresses.isEmpty()) {
#if defined(QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocketPrivate::_q_connectToNextAddress(), all addresses failed.");
#endif
            state = QAbstractSocket::UnconnectedState;
            if (socketEngine) {
                if ((socketEngine->error() == QAbstractSocket::UnknownSocketError
#ifdef Q_OS_AIX
                     // On AIX, the second connect call will result in EINVAL and not
                     // ECONNECTIONREFUSED; although the meaning is the same.
                     || socketEngine->error() == QAbstractSocket::UnsupportedSocketOperationError
#endif
                    ) && socketEngine->state() == QAbstractSocket::ConnectingState) {
                    socketError = QAbstractSocket::ConnectionRefusedError;
                    q->setErrorString(QLatin1String(QT_TRANSLATE_NOOP("QAbstractSocket",
                                                                      "Connection refused")));
                } else {
                    socketError = socketEngine->error();
                    q->setErrorString(socketEngine->errorString());
                }
            } else {
                socketError = QAbstractSocket::ConnectionRefusedError;
                q->setErrorString(QLatin1String(QT_TRANSLATE_NOOP("QAbstractSocket",
                                                                  "Connection refused")));
            }
            emit q->stateChanged(state);
            emit q->error(socketError);
            return;
        }

        // Pick the first host address candidate
        host = addresses.takeFirst();
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocketPrivate::_q_connectToNextAddress(), connecting to %s:%i",
               host.toString().toLatin1().constData(), port);
#endif

#if defined(QT_NO_IPV6)
        if (host.protocol() == QAbstractSocket::IPv6Protocol) {
            // If we have no IPv6 support, then we will not be able to
            // connect. So we just pretend we didn't see this address.
#if defined(QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocketPrivate::_q_connectToNextAddress(), skipping IPv6 entry");
#endif
            continue;
        }
#endif

        if (!initSocketLayer(host, q->socketType())) {
            // hope that the next address is better
#if defined(QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocketPrivate::_q_connectToNextAddress(), failed to initialize sock layer");
#endif
            continue;
        }

        // Tries to connect to the address. If it succeeds immediately
        // (localhost address on BSD or any UDP connect), emit
        // connected() and return.
        if (socketEngine->connectToHost(host, port)) {
            fetchConnectionParameters();
            return;
        }

        // Check that we're in delayed connection state. If not, try
        // the next address
        if (socketEngine->state() != QAbstractSocket::ConnectingState) {
#if defined(QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocketPrivate::_q_connectToNextAddress(), connection failed (%s)",
                   socketEngine->errorString().toLatin1().constData());
#endif
            continue;
        }

        // Start the connect timer.
        if (threadData->eventDispatcher) {
            if (!connectTimer) {
                connectTimer = new QTimer(q);
                QObject::connect(connectTimer, SIGNAL(timeout()),
                                 q, SLOT(_q_abortConnectionAttempt()),
                                 Qt::DirectConnection);
            }
            connectTimer->start(QT_CONNECT_TIMEOUT);
        }

        // Wait for a write notification that will eventually call
        // _q_testConnection().
        socketEngine->setWriteNotificationEnabled(true);
        break;
    } while (state != QAbstractSocket::ConnectedState);
}

/*! \internal

    Tests if a connection has been established. If it has, connected()
    is emitted. Otherwise, _q_connectToNextAddress() is invoked.
*/
void QAbstractSocketPrivate::_q_testConnection()
{
    if (socketEngine) {
        if (socketEngine->state() != QAbstractSocket::ConnectedState) {
            // Try connecting if we're not already connected.
            if (!socketEngine->connectToHost(host, port)) {
                if (socketEngine->error() == QAbstractSocket::UnfinishedSocketOperationError) {
                    // Connection in progress; wait for the next notification.
                    socketEngine->setWriteNotificationEnabled(true);
                    return;
                }
            }
        }

        if (threadData->eventDispatcher) {
            if (connectTimer)
                connectTimer->stop();
        }

        if (socketEngine->state() == QAbstractSocket::ConnectedState) {
            // Fetch the parameters if our connection is completed;
            // otherwise, fall out and try the next address.
            fetchConnectionParameters();
            if (pendingClose) {
                q_func()->disconnectFromHost();
                pendingClose = false;
            }
            return;
        }

        // don't retry the other addresses if we couldn't authenticate to the proxy.
        if (socketEngine->error() == QAbstractSocket::ProxyAuthenticationRequiredError)
            addresses.clear();
    }

    if (threadData->eventDispatcher) {
        if (connectTimer)
            connectTimer->stop();
    }

#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::_q_testConnection() connection failed,"
           " checking for alternative addresses");
#endif
    _q_connectToNextAddress();
}

/*! \internal

    This function is called after a certain number of seconds has
    passed while waiting for a connection. It simply tests the
    connection, and continues to the next address if the connection
    failed.
*/
void QAbstractSocketPrivate::_q_abortConnectionAttempt()
{
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::_q_abortConnectionAttempt() (timed out)");
#endif
    if (socketEngine) {
        socketEngine->setWriteNotificationEnabled(false);

        if (socketEngine->isValid())
            _q_testConnection();
    }
}

/*! \internal

    Reads data from the socket layer into the read buffer. Returns
    true on success; otherwise false.
*/
bool QAbstractSocketPrivate::readFromSocket()
{
    Q_Q(QAbstractSocket);
    // Find how many bytes we can read from the socket layer.
    qint64 bytesToRead = socketEngine->bytesAvailable();
#ifdef Q_OS_LINUX
    if (bytesToRead > 0) // ### See setSocketDescriptor()
        bytesToRead += addToBytesAvailable;
#endif
    if (bytesToRead == 0) {
        // Under heavy load, certain conditions can trigger read notifications
        // for socket notifiers on which there is no activity. If we continue
        // to read 0 bytes from the socket, we will trigger behavior similar
        // to that which signals a remote close. When we hit this condition,
        // we try to read 4k of data from the socket, which will give us either
        // an EAGAIN/EWOULDBLOCK if the connection is alive (i.e., the remote
        // host has _not_ disappeared).
        bytesToRead = 4096;
    }
    if (readBufferMaxSize && bytesToRead > (readBufferMaxSize - readBuffer.size()))
        bytesToRead = readBufferMaxSize - readBuffer.size();

#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::readFromSocket() about to read %d bytes",
           int(bytesToRead));
#endif

    // Read from the socket, store data in the read buffer.
    char *ptr = readBuffer.reserve(bytesToRead);
    qint64 readBytes = socketEngine->read(ptr, bytesToRead);
    if (readBytes == -2) {
        // No bytes currently available for reading.
        readBuffer.chop(bytesToRead);
        return true;
    }
    readBuffer.chop(int(bytesToRead - (readBytes < 0 ? qint64(0) : readBytes)));
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::readFromSocket() got %d bytes, buffer size = %d",
           int(readBytes), readBuffer.size());
#endif

    if (!socketEngine->isValid()) {
        socketError = socketEngine->error();
        q->setErrorString(socketEngine->errorString());
        emit q->error(socketError);
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocketPrivate::readFromSocket() read failed: %s",
               q->errorString().toLatin1().constData());
#endif
        resetSocketLayer();
        return false;
    }

    return true;
}

/*! \internal

    Sets up the the internal state after the connection has succeeded.
*/
void QAbstractSocketPrivate::fetchConnectionParameters()
{
    Q_Q(QAbstractSocket);

    peerName = hostName;
    if (socketEngine) {
        socketEngine->setReadNotificationEnabled(true);
        socketEngine->setWriteNotificationEnabled(true);
        localPort = socketEngine->localPort();
        peerPort = socketEngine->peerPort();
        localAddress = socketEngine->localAddress();
        peerAddress = socketEngine->peerAddress();
        cachedSocketDescriptor = socketEngine->socketDescriptor();
    }

    state = QAbstractSocket::ConnectedState;
    emit q->stateChanged(state);
    emit q->connected();

#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocketPrivate::fetchConnectionParameters() connection to %s:%i established",
           host.toString().toLatin1().constData(), port);
#endif
}

/*! \internal

    Constructs a new abstract socket of type \a socketType. The \a
    parent argument is passed to QObject's constructor.
*/
QAbstractSocket::QAbstractSocket(SocketType socketType,
                                 QAbstractSocketPrivate &dd, QObject *parent)
    : QIODevice(dd, parent)
{
    Q_D(QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::QAbstractSocket(%sSocket, QAbstractSocketPrivate == %p, parent == %p)",
           socketType == TcpSocket ? "Tcp" : socketType == UdpSocket
           ? "Udp" : "Unknown", &dd, parent);
#endif
    d->socketType = socketType;
}

/*!
    Creates a new abstract socket of type \a socketType. The \a
    parent argument is passed to QObject's constructor.

    \sa socketType(), QTcpSocket, QUdpSocket
*/
QAbstractSocket::QAbstractSocket(SocketType socketType, QObject *parent)
    : QIODevice(*new QAbstractSocketPrivate, parent)
{
    Q_D(QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::QAbstractSocket(%p)", parent);
#endif
    d->socketType = socketType;
}

/*!
    Destroys the socket.
*/
QAbstractSocket::~QAbstractSocket()
{
    Q_D(QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::~QAbstractSocket()");
#endif
    if (d->state != UnconnectedState)
        abort();
}

/*!
    Returns true if the socket is valid and ready for use; otherwise
    returns false.

    \bold{Note:} The socket's state must be ConnectedState before reading and
    writing can occur.

    \sa state()
*/
bool QAbstractSocket::isValid() const
{
    return d_func()->socketEngine ? d_func()->socketEngine->isValid() : isOpen();
}

/*!
    Attempts to make a connection to \a hostName on the given \a port.

    The socket is opened in the given \a openMode and first enters
    HostLookupState, then performs a host name lookup of \a hostName.
    If the lookup succeeds, hostFound() is emitted and QAbstractSocket
    enters ConnectingState. It then attempts to connect to the address
    or addresses returned by the lookup. Finally, if a connection is
    established, QAbstractSocket enters ConnectedState and
    emits connected().

    At any point, the socket can emit error() to signal that an error
    occurred.

    \a hostName may be an IP address in string form (e.g.,
    "43.195.83.32"), or it may be a host name (e.g.,
    "www.trolltech.com"). QAbstractSocket will do a lookup only if
    required. \a port is in native byte order.

    \sa state(), peerName(), peerAddress(), peerPort(), waitForConnected()
*/
void QAbstractSocket::connectToHost(const QString &hostName, quint16 port,
                                    OpenMode openMode)
{
    QMetaObject::invokeMethod(this, "connectToHostImplementation",
                              Qt::DirectConnection,
                              Q_ARG(QString, hostName),
                              Q_ARG(quint16, port),
                              Q_ARG(OpenMode, openMode));
}

/*!
    \since 4.1

    Contains the implementation of connectToHost().

    Attempts to make a connection to \a hostName on the given \a
    port. The socket is opened in the given \a openMode.
*/
void QAbstractSocket::connectToHostImplementation(const QString &hostName, quint16 port,
                                                  OpenMode openMode)
{
    Q_D(QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::connectToHost(\"%s\", %i, %i)...", hostName.toLatin1().constData(), port,
           (int) openMode);
#endif

    if (d->state == ConnectedState || d->state == ConnectingState) {
        qWarning("QAbstractSocket::connectToHost() called when already connecting/connected");
        return;
    }

    d->hostName = hostName;
    d->port = port;
    d->state = HostLookupState;
    d->readBuffer.clear();
    d->writeBuffer.clear();
    d->closeCalled = false;
    d->pendingClose = false;
    d->localPort = 0;
    d->peerPort = 0;
    d->localAddress.clear();
    d->peerAddress.clear();
    d->peerName = hostName;
#ifdef Q_OS_LINUX
    // ### See setSocketDescriptor().
    d->addToBytesAvailable = 0;
#endif
    if (d->hostLookupId != -1) {
        QHostInfo::abortHostLookup(d->hostLookupId);
        d->hostLookupId = -1;
    }

    if (!d_func()->isBuffered)
        openMode |= QAbstractSocket::Unbuffered;
    setOpenMode(openMode);
    emit stateChanged(d->state);

    QHostAddress temp;
    if (temp.setAddress(hostName)) {
        QHostInfo info;
        info.setAddresses(QList<QHostAddress>() << temp);
        d->_q_startConnecting(info);
    } else {
        if (d->threadData->eventDispatcher)
            d->hostLookupId = QHostInfo::lookupHost(hostName, this, SLOT(_q_startConnecting(QHostInfo)));
    }

#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::connectToHost(\"%s\", %i) == %s%s", hostName.toLatin1().constData(), port,
           (d->state == ConnectedState) ? "true" : "false",
           (d->state == ConnectingState || d->state == HostLookupState)
           ? " (connection in progress)" : "");
#endif
}

/*! \overload

    Attempts to make a connection to \a address on port \a port.
*/
void QAbstractSocket::connectToHost(const QHostAddress &address, quint16 port,
                                    OpenMode openMode)
{
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::connectToHost([%s], %i, %i)...",
           address.toString().toLatin1().constData(), port, (int) openMode);
#endif
    connectToHost(address.toString(), port, openMode);
}

/*!
    Returns the number of bytes that are waiting to be written. The
    bytes are written when control goes back to the event loop or
    when flush() is called.

    \sa bytesAvailable(), flush()
*/
qint64 QAbstractSocket::bytesToWrite() const
{
    Q_D(const QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::bytesToWrite() == %i", d->writeBuffer.size());
#endif
    return (qint64)d->writeBuffer.size();
}

/*!
    Returns the number of incoming bytes that are waiting to be read.

    \sa bytesToWrite(), read()
*/
qint64 QAbstractSocket::bytesAvailable() const
{
    Q_D(const QAbstractSocket);
    qint64 available = QIODevice::bytesAvailable();
    if (d->isBuffered)
        available += (qint64) d->readBuffer.size();
    else if (d->socketEngine && d->socketEngine->isValid())
        available += d->socketEngine->bytesAvailable();
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::bytesAvailable() == %llu", available);
#endif
    return available;
}

/*!
    Returns the host port number (in native byte order) of the local
    socket if available; otherwise returns 0.

    \sa localAddress(), peerPort(), setLocalPort()
*/
quint16 QAbstractSocket::localPort() const
{
    Q_D(const QAbstractSocket);
    return d->localPort;
}

/*!
    Returns the host address of the local socket if available;
    otherwise returns QHostAddress::Null.

    This is normally the main IP address of the host, but can be
    QHostAddress::LocalHost (127.0.0.1) for connections to the
    local host.

    \sa localPort(), peerAddress(), setLocalAddress()
*/
QHostAddress QAbstractSocket::localAddress() const
{
    Q_D(const QAbstractSocket);
    return d->localAddress;
}

/*!
    Returns the port of the connected peer if the socket is in
    ConnectedState; otherwise returns 0.

    \sa peerAddress(), localPort(), setPeerPort()
*/
quint16 QAbstractSocket::peerPort() const
{
    Q_D(const QAbstractSocket);
    return d->peerPort;
}

/*!
    Returns the address of the connected peer if the socket is in
    ConnectedState; otherwise returns QHostAddress::Null.

    \sa peerName(), peerPort(), localAddress(), setPeerAddress()
*/
QHostAddress QAbstractSocket::peerAddress() const
{
    Q_D(const QAbstractSocket);
    return d->peerAddress;
}

/*!
    Returns the name of the peer as specified by connectToHost(), or
    an empty QString if connectToHost() has not been called.

    \sa peerAddress(), peerPort(), setPeerName()
*/
QString QAbstractSocket::peerName() const
{
    Q_D(const QAbstractSocket);
    return d->peerName.isEmpty() ? d->hostName : d->peerName;
}

/*!
    Returns true if a line of data can be read from the socket;
    otherwise returns false.

    \sa readLine()
*/
bool QAbstractSocket::canReadLine() const
{
    bool hasLine = d_func()->readBuffer.canReadLine();
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::canReadLine() == %s, buffer size = %d, size = %d", hasLine ? "true" : "false",
           d_func()->readBuffer.size(), d_func()->buffer.size());
#endif
    return hasLine || QIODevice::canReadLine();
}

/*!
    Returns the native socket descriptor of the QAbstractSocket object
    if this is available; otherwise returns -1.

    If the socket is using QNetworkProxy, the returned descriptor
    may not be usable with native socket functions.

    The socket descriptor is not available when QAbstractSocket is in
    UnconnectedState.

    \sa setSocketDescriptor()
*/
int QAbstractSocket::socketDescriptor() const
{
    Q_D(const QAbstractSocket);
    return d->cachedSocketDescriptor;
}

/*!
    Initializes QAbstractSocket with the native socket descriptor \a
    socketDescriptor. Returns true if \a socketDescriptor is accepted
    as a valid socket descriptor; otherwise returns false.
    The socket is opened in the mode specified by \a openMode, and
    enters the socket state specified by \a socketState.

    \bold{Note:} It is not possible to initialize two abstract sockets
    with the same native socket descriptor.

    \sa socketDescriptor()
*/
bool QAbstractSocket::setSocketDescriptor(int socketDescriptor, SocketState socketState,
                                          OpenMode openMode)
{
    Q_D(QAbstractSocket);
#ifndef QT_NO_OPENSSL
    if (QSslSocket *socket = qobject_cast<QSslSocket *>(this))
        return socket->setSocketDescriptor(socketDescriptor, socketState, openMode);
#endif

    d->resetSocketLayer();
    d->socketEngine = QAbstractSocketEngine::createSocketEngine(socketDescriptor, this);
    bool result = d->socketEngine->initialize(socketDescriptor, socketState);
    if (!result) {
        d->socketError = d->socketEngine->error();
        setErrorString(d->socketEngine->errorString());
        return false;
    }

    if (d->threadData->eventDispatcher)
        d->socketEngine->setReceiver(d);

    setOpenMode(openMode);

    if (d->state != socketState) {
        d->state = socketState;
        emit stateChanged(d->state);
    }

    d->pendingClose = false;
    d->socketEngine->setReadNotificationEnabled(true);
    d->localPort = d->socketEngine->localPort();
    d->peerPort = d->socketEngine->peerPort();
    d->localAddress = d->socketEngine->localAddress();
    d->peerAddress = d->socketEngine->peerAddress();
    d->cachedSocketDescriptor = socketDescriptor;

#ifdef Q_OS_LINUX
    // ### This is a workaround for certain broken Linux kernels, when using
    // QTcpSocket with a Unix domain socket. It was introduced around 2.6.9,
    // and fixed at some point after that.
    // http://archive.linux-usenet.com/index-t-73300.html
    // We can provide a better workaround for this: readFromSocket() can loop
    // while reading, but this must happen without triggering an implicit
    // close because of reading after the socket has closed.
    d->addToBytesAvailable = 4096;
#endif

    return true;
}

/*
   Returns the difference between msecs and elapsed. If msecs is -1,
   however, -1 is returned.
*/
static int qt_timeout_value(int msecs, int elapsed)
{
    if (msecs == -1)
        return -1;

    int timeout = msecs - elapsed;
    return timeout < 0 ? 0 : timeout;
}

/*!
    Waits until the socket is connected, up to \a msecs
    milliseconds. If the connection has been established, this
    function returns true; otherwise it returns false. In the case
    where it returns false, you can call error() to determine
    the cause of the error.

    The following example waits up to one second for a connection
    to be established:

    \snippet doc/src/snippets/code/src.network.socket.qabstractsocket.cpp 0

    If msecs is -1, this function will not time out.

    Note: This function may wait slightly longer than \a msecs,
    depending on the time it takes to complete the host lookup.

    \sa connectToHost(), connected()
*/
bool QAbstractSocket::waitForConnected(int msecs)
{
    Q_D(QAbstractSocket);
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::waitForConnected(%i)", msecs);
#endif

    if (state() == ConnectedState) {
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::waitForConnected(%i) already connected", msecs);
#endif
        return true;
    }

#ifndef QT_NO_OPENSSL
    // Manual polymorphism; this function is not virtual, but has an overload
    // in QSslSocket.
    if (QSslSocket *socket = qobject_cast<QSslSocket *>(this))
        return socket->waitForConnected(msecs);
#endif

    bool wasPendingClose = d->pendingClose;
    d->pendingClose = false;
    QTime stopWatch;
    stopWatch.start();

    if (d->state == HostLookupState) {
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::waitForConnected(%i) doing host name lookup", msecs);
#endif
        QHostInfo::abortHostLookup(d->hostLookupId);
        d->hostLookupId = -1;
        d->_q_startConnecting(QHostInfo::fromName(d->hostName));
    } else {
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::waitForConnected(%i) testing connection", msecs);
#endif
        d->_q_testConnection();
    }
    if (state() == UnconnectedState)
        return false;

    bool timedOut = true;
#if defined (QABSTRACTSOCKET_DEBUG)
    int attempt = 1;
#endif
    while (state() == ConnectingState && (msecs == -1 || stopWatch.elapsed() < msecs)) {
        int timeout = qt_timeout_value(msecs, stopWatch.elapsed());
        if (msecs != -1 && timeout > QT_CONNECT_TIMEOUT)
            timeout = QT_CONNECT_TIMEOUT;
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::waitForConnected(%i) waiting %.2f secs for connection attempt #%i",
               msecs, timeout / 1000.0, attempt++);
#endif
        timedOut = false;

        if (d->socketEngine->waitForWrite(timeout, &timedOut) && !timedOut) {
            d->_q_testConnection();
        } else {
            d->_q_connectToNextAddress();
        }
    }

    if ((timedOut && state() != ConnectedState) || state() == ConnectingState) {
        d->socketError = SocketTimeoutError;
        setSocketState(UnconnectedState);
        d->resetSocketLayer();
        setErrorString(tr("Socket operation timed out"));
    }

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::waitForConnected(%i) == %s", msecs,
           state() == ConnectedState ? "true" : "false");
#endif
    if (state() != ConnectedState)
        return false;
    if (wasPendingClose)
        disconnectFromHost();
    return true;
}

/*!
    This function blocks until data is available for reading and the
    \l{QIODevice::}{readyRead()} signal has been emitted. The function
    will timeout after \a msecs milliseconds; the default timeout is
    3000 milliseconds.

    The function returns true if the readyRead() signal is emitted and
    there is data available for reading; otherwise it returns false
    (if an error occurred or the operation timed out).

    \sa waitForBytesWritten() 
*/
bool QAbstractSocket::waitForReadyRead(int msecs)
{
    Q_D(QAbstractSocket);
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::waitForReadyRead(%i)", msecs);
#endif

    // require calling connectToHost() before waitForReadyRead()
    if (state() == UnconnectedState) {
        /* If all you have is a QIODevice pointer to an abstractsocket, you cannot check
           this, so you cannot avoid this warning. */
//        qWarning("QAbstractSocket::waitForReadyRead() is not allowed in UnconnectedState");
        return false;
    }

    QTime stopWatch;
    stopWatch.start();

    // handle a socket in connecting state
    if (state() == HostLookupState || state() == ConnectingState) {
        if (!waitForConnected(msecs))
            return false;
    }

    Q_ASSERT(d->socketEngine);
    forever {
        bool readyToRead = false;
        bool readyToWrite = false;
        if (!d->socketEngine->waitForReadOrWrite(&readyToRead, &readyToWrite, true, !d->writeBuffer.isEmpty(),
                                               qt_timeout_value(msecs, stopWatch.elapsed()))) {
            d->socketError = d->socketEngine->error();
            setErrorString(d->socketEngine->errorString());
#if defined (QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocket::waitForReadyRead(%i) failed (%i, %s)",
                   msecs, d->socketError, errorString().toLatin1().constData());
#endif
            emit error(d->socketError);
            if (d->socketError != SocketTimeoutError)
                close();
            return false;
        }

        if (readyToRead) {
            if (d->canReadNotification())
                return true;
        }

        if (readyToWrite)
            d->canWriteNotification();

        if (state() != ConnectedState)
            return false;
    }
    return false;
}

/*! \reimp
 */
bool QAbstractSocket::waitForBytesWritten(int msecs)
{
    Q_D(QAbstractSocket);
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::waitForBytesWritten(%i)", msecs);
#endif

    // require calling connectToHost() before waitForBytesWritten()
    if (state() == UnconnectedState) {
        qWarning("QAbstractSocket::waitForBytesWritten() is not allowed in UnconnectedState");
        return false;
    }

    if (d->writeBuffer.isEmpty())
        return false;

    QTime stopWatch;
    stopWatch.start();

    // handle a socket in connecting state
    if (state() == HostLookupState || state() == ConnectingState) {
        if (!waitForConnected(msecs))
            return false;
    }

    forever {
        bool readyToRead = false;
        bool readyToWrite = false;
        if (!d->socketEngine->waitForReadOrWrite(&readyToRead, &readyToWrite, true, !d->writeBuffer.isEmpty(),
                                               qt_timeout_value(msecs, stopWatch.elapsed()))) {
            d->socketError = d->socketEngine->error();
            setErrorString(d->socketEngine->errorString());
#if defined (QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocket::waitForBytesWritten(%i) failed (%i, %s)",
                   msecs, d->socketError, errorString().toLatin1().constData());
#endif
            emit error(d->socketError);
            if (d->socketError != SocketTimeoutError)
                close();
            return false;
        }

        if (readyToRead) {
#if defined (QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocket::waitForBytesWritten calls canReadNotification");
#endif
            if(!d->canReadNotification())
                return false;
        }


        if (readyToWrite) {
            if (d->canWriteNotification()) {
#if defined (QABSTRACTSOCKET_DEBUG)
                qDebug("QAbstractSocket::waitForBytesWritten returns true");
#endif
                return true;
            }
        }

        if (state() != ConnectedState)
            return false;
    }
    return false;
}

/*!
    Waits until the socket has disconnected, up to \a msecs
    milliseconds. If the connection has been disconnected, this
    function returns true; otherwise it returns false. In the case
    where it returns false, you can call error() to determine
    the cause of the error.

    The following example waits up to one second for a connection
    to be closed:

    \snippet doc/src/snippets/code/src.network.socket.qabstractsocket.cpp 1

    If msecs is -1, this function will not time out.

    \sa disconnectFromHost(), close()
*/
bool QAbstractSocket::waitForDisconnected(int msecs)
{
    Q_D(QAbstractSocket);
#ifndef QT_NO_OPENSSL
    // Manual polymorphism; this function is not virtual, but has an overload
    // in QSslSocket.
    if (QSslSocket *socket = qobject_cast<QSslSocket *>(this))
        return socket->waitForDisconnected(msecs);
#endif

    // require calling connectToHost() before waitForDisconnected()
    if (state() == UnconnectedState) {
        qWarning("QAbstractSocket::waitForDisconnected() is not allowed in UnconnectedState");
        return false;
    }

    QTime stopWatch;
    stopWatch.start();

    // handle a socket in connecting state
    if (state() == HostLookupState || state() == ConnectingState) {
        if (!waitForConnected(msecs))
            return false;
        if (state() == UnconnectedState)
            return true;
    }

    forever {
        bool readyToRead = false;
        bool readyToWrite = false;
        if (!d->socketEngine->waitForReadOrWrite(&readyToRead, &readyToWrite, state() == ConnectedState,
                                               !d->writeBuffer.isEmpty(),
                                               qt_timeout_value(msecs, stopWatch.elapsed()))) {
            d->socketError = d->socketEngine->error();
            setErrorString(d->socketEngine->errorString());
#if defined (QABSTRACTSOCKET_DEBUG)
            qDebug("QAbstractSocket::waitForReadyRead(%i) failed (%i, %s)",
                   msecs, d->socketError, errorString().toLatin1().constData());
#endif
            emit error(d->socketError);
            if (d->socketError != SocketTimeoutError)
                close();
            return false;
        }

        if (readyToRead)
            d->canReadNotification();
        if (readyToWrite)
            d->canWriteNotification();

        if (state() == UnconnectedState)
            return true;
    }
    return false;
}

/*!
    Aborts the current connection and resets the socket. Unlike
    disconnectFromHost(), this function immediately closes the socket, discarding
    any pending data in the write buffer.

    \sa disconnectFromHost(), close()
*/
void QAbstractSocket::abort()
{
    Q_D(QAbstractSocket);
#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::abort()");
#endif
    if (d->state == UnconnectedState)
        return;
#ifndef QT_NO_OPENSSL
    if (QSslSocket *socket = qobject_cast<QSslSocket *>(this)) {
        socket->abort();
        return;
    }
#endif
    if (d->connectTimer) {
        d->connectTimer->stop();
        delete d->connectTimer;
        d->connectTimer = 0;
    }

    d->writeBuffer.clear();
    close();
}

/*! \reimp
*/
bool QAbstractSocket::isSequential() const
{
    return true;
}

/*! \reimp

     Returns true if no more data is currently
     available for reading; otherwise returns false.

     This function is most commonly used when reading data from the
     socket in a loop. For example:

     \snippet doc/src/snippets/code/src.network.socket.qabstractsocket.cpp 2

     \sa bytesAvailable(), readyRead()
 */
bool QAbstractSocket::atEnd() const
{
    return QIODevice::atEnd() && (!isOpen() || d_func()->readBuffer.isEmpty());
}

/*!
    This function writes as much as possible from the internal write buffer to
    the underlying network socket, without blocking. If any data was written,
    this function returns true; otherwise false is returned.

    Call this function if you need QAbstractSocket to start sending buffered
    data immediately. The number of bytes successfully written depends on the
    operating system. In most cases, you do not need to call this function,
    because QAbstractSocket will start sending data automatically once control
    goes back to the event loop. In the absence of an event loop, call
    waitForBytesWritten() instead.

    \sa write(), waitForBytesWritten()
*/
// Note! docs copied to QSslSocket::flush()
bool QAbstractSocket::flush()
{
    Q_D(QAbstractSocket);
#ifndef QT_NO_OPENSSL
    // Manual polymorphism; flush() isn't virtual, but QSslSocket overloads
    // it.
    if (QSslSocket *socket = qobject_cast<QSslSocket *>(this))
        return socket->flush();
#endif
    Q_CHECK_SOCKETENGINE(false);
    return d->flush();
}

/*! \reimp
*/
qint64 QAbstractSocket::readData(char *data, qint64 maxSize)
{
    Q_D(QAbstractSocket);
    if (d->socketEngine && !d->socketEngine->isReadNotificationEnabled() && d->socketEngine->isValid())
        d->socketEngine->setReadNotificationEnabled(true);

    if (!d->isBuffered) {
        if (!d->socketEngine)
            return -1;          // no socket engine is probably EOF
        qint64 readBytes = d->socketEngine->read(data, maxSize);
        if (readBytes < 0) {
            d->socketError = d->socketEngine->error();
            setErrorString(d->socketEngine->errorString());
        }
        if (!d->socketEngine->isReadNotificationEnabled())
            d->socketEngine->setReadNotificationEnabled(true);
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::readData(%p \"%s\", %lli) == %lld",
               data, qt_prettyDebug(data, 32, readBytes).data(), maxSize,
               readBytes);
#endif
        return readBytes;
    }

    if (d->readBuffer.isEmpty())
        // if we're still connected, return 0 indicating there may be more data in the future
        // if we're not connected, return -1 indicating EOF
        return d->state == QAbstractSocket::ConnectedState ? qint64(0) : qint64(-1);

    // If readFromSocket() read data, copy it to its destination.
    if (maxSize == 1) {
        *data = d->readBuffer.getChar();
#if defined (QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::readData(%p '%c (0x%.2x)', 1) == 1",
               data, isprint(int(uchar(*data))) ? *data : '?', *data);
#endif
        return 1;
    }

    qint64 bytesToRead = qMin(qint64(d->readBuffer.size()), maxSize);
    qint64 readSoFar = 0;
    while (readSoFar < bytesToRead) {
        const char *ptr = d->readBuffer.readPointer();
        int bytesToReadFromThisBlock = qMin(int(bytesToRead - readSoFar),
                                            d->readBuffer.nextDataBlockSize());
        memcpy(data + readSoFar, ptr, bytesToReadFromThisBlock);
        readSoFar += bytesToReadFromThisBlock;
        d->readBuffer.free(bytesToReadFromThisBlock);
    }

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::readData(%p \"%s\", %lli) == %lld",
           data, qt_prettyDebug(data, qMin<qint64>(32, readSoFar), readSoFar).data(),
           maxSize, readSoFar);
#endif
    return readSoFar;
}

/*! \reimp
*/
qint64 QAbstractSocket::readLineData(char *data, qint64 maxlen)
{
    return QIODevice::readLineData(data, maxlen);
}

/*! \reimp
*/
qint64 QAbstractSocket::writeData(const char *data, qint64 size)
{
    Q_D(QAbstractSocket);
    if (d->state == QAbstractSocket::UnconnectedState) {
        d->socketError = QAbstractSocket::UnknownSocketError;
        setErrorString(tr("Socket is not connected"));
        return -1;
    }

    if (!d->isBuffered) {
        qint64 written = d->socketEngine->write(data, size);
        if (written < 0) {
            d->socketError = d->socketEngine->error();
            setErrorString(d->socketEngine->errorString());
        } else if (!d->writeBuffer.isEmpty()) {
            d->socketEngine->setWriteNotificationEnabled(true);
        }

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::writeData(%p \"%s\", %lli) == %lli", data,
           qt_prettyDebug(data, qMin((int)size, 32), size).data(),
           size, written);
#endif
        if (written >= 0)
            emit bytesWritten(written);
        return written;
    }

    char *ptr = d->writeBuffer.reserve(size);
    if (size == 1)
        *ptr = *data;
    else
        memcpy(ptr, data, size);

    qint64 written = size;

    if (d->socketEngine && !d->writeBuffer.isEmpty())
        d->socketEngine->setWriteNotificationEnabled(true);

#if defined (QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::writeData(%p \"%s\", %lli) == %lli", data,
           qt_prettyDebug(data, qMin((int)size, 32), size).data(),
           size, written);
#endif
    return written;
}

/*!
    \since 4.1

    Sets the port on the local side of a connection to \a port.

    You can call this function in a subclass of QAbstractSocket to
    change the return value of the localPort() function after a
    connection has been established. This feature is commonly used by
    proxy connections for virtual connection settings.

    Note that this function does not bind the local port of the socket
    prior to a connection (e.g., QUdpSocket::bind()).

    \sa localAddress(), setLocalAddress(), setPeerPort()
*/
void QAbstractSocket::setLocalPort(quint16 port)
{
    Q_D(QAbstractSocket);
    d->localPort = port;
}

/*!
    \since 4.1

    Sets the address on the local side of a connection to
    \a address.

    You can call this function in a subclass of QAbstractSocket to
    change the return value of the localAddress() function after a
    connection has been established. This feature is commonly used by
    proxy connections for virtual connection settings.

    Note that this function does not bind the local address of the socket
    prior to a connection (e.g., QUdpSocket::bind()).

    \sa localAddress(), setLocalPort(), setPeerAddress()
*/
void QAbstractSocket::setLocalAddress(const QHostAddress &address)
{
    Q_D(QAbstractSocket);
    d->localAddress = address;
}

/*!
    \since 4.1

    Sets the port of the remote side of the connection to
    \a port.

    You can call this function in a subclass of QAbstractSocket to
    change the return value of the peerPort() function after a
    connection has been established. This feature is commonly used by
    proxy connections for virtual connection settings.

    \sa peerPort(), setPeerAddress(), setLocalPort()
*/
void QAbstractSocket::setPeerPort(quint16 port)
{
    Q_D(QAbstractSocket);
    d->peerPort = port;
}

/*!
    \since 4.1

    Sets the address of the remote side of the connection
    to \a address.

    You can call this function in a subclass of QAbstractSocket to
    change the return value of the peerAddress() function after a
    connection has been established. This feature is commonly used by
    proxy connections for virtual connection settings.

    \sa peerAddress(), setPeerPort(), setLocalAddress()
*/
void QAbstractSocket::setPeerAddress(const QHostAddress &address)
{
    Q_D(QAbstractSocket);
    d->peerAddress = address;
}

/*!
    \since 4.1

    Sets the host name of the remote peer to \a name.

    You can call this function in a subclass of QAbstractSocket to
    change the return value of the peerName() function after a
    connection has been established. This feature is commonly used by
    proxy connections for virtual connection settings.

    \sa peerName()
*/
void QAbstractSocket::setPeerName(const QString &name)
{
    Q_D(QAbstractSocket);
    d->peerName = name;
}

/*!
    Disconnects the socket's connection with the host.

    \sa abort()
*/
void QAbstractSocket::close()
{
    Q_D(QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::close()");
#endif
    QIODevice::close();
    if (d->state != UnconnectedState) {
        d->closeCalled = true;
        disconnectFromHost();
    }

    d->localPort = 0;
    d->peerPort = 0;
    d->localAddress.clear();
    d->peerAddress.clear();
    d->peerName.clear();
    d->cachedSocketDescriptor = -1;
}

/*!
    Attempts to close the socket. If there is pending data waiting to
    be written, QAbstractSocket will enter ClosingState and wait
    until all data has been written. Eventually, it will enter
    UnconnectedState and emit the disconnected() signal.

    \sa connectToHost()
*/
void QAbstractSocket::disconnectFromHost()
{
    QMetaObject::invokeMethod(this, "disconnectFromHostImplementation",
                              Qt::DirectConnection);
}

/*!
    \since 4.1

    Contains the implementation of disconnectFromHost().
*/
void QAbstractSocket::disconnectFromHostImplementation()
{
    Q_D(QAbstractSocket);
#if defined(QABSTRACTSOCKET_DEBUG)
    qDebug("QAbstractSocket::disconnectFromHost()");
#endif

    if (d->state == UnconnectedState) {
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() was called on an unconnected socket");
#endif
        return;
    }

    if (d->state == ConnectingState) {
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() but we're still connecting");
#endif
        d->pendingClose = true;
        return;
    }

#ifdef QT3_SUPPORT
    emit connectionClosed(); // compat signal
#endif

    // Disable and delete read notification
    if (d->socketEngine)
        d->socketEngine->setReadNotificationEnabled(false);

    // Perhaps emit closing()
    if (d->state != ClosingState) {
        d->state = ClosingState;
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() emits stateChanged()(ClosingState)");
#endif
        emit stateChanged(d->state);
    } else {
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() return from delayed close");
#endif
    }

    // Wait for pending data to be written.
    if (d->socketEngine && d->socketEngine->isValid() && d->writeBuffer.size() > 0) {
        d->socketEngine->setWriteNotificationEnabled(true);

#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() delaying disconnect");
#endif
        return;
    } else {
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() disconnecting immediately");
#endif
    }

    d->resetSocketLayer();
    d->state = UnconnectedState;
    emit stateChanged(d->state);
    emit readChannelFinished();       // we got an EOF

#ifdef QT3_SUPPORT
    emit delayedCloseFinished(); // compat signal
#endif
    emit disconnected();

    d->localPort = 0;
    d->peerPort = 0;
    d->localAddress.clear();
    d->peerAddress.clear();

#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() disconnected!");
#endif

    if (d->closeCalled) {
#if defined(QABSTRACTSOCKET_DEBUG)
        qDebug("QAbstractSocket::disconnectFromHost() closed!");
#endif
        d->readBuffer.clear();
        d->writeBuffer.clear();
        QIODevice::close();
    }
}

/*!
    Returns the size of the internal read buffer. This limits the
    amount of data that the client can receive before you call read()
    or readAll().

    A read buffer size of 0 (the default) means that the buffer has
    no size limit, ensuring that no data is lost.

    \sa setReadBufferSize(), read()
*/
qint64 QAbstractSocket::readBufferSize() const
{
    return d_func()->readBufferMaxSize;
}

/*!
    Sets the size of QAbstractSocket's internal read buffer to be \a
    size bytes.

    If the buffer size is limited to a certain size, QAbstractSocket
    won't buffer more than this size of data. Exceptionally, a buffer
    size of 0 means that the read buffer is unlimited and all
    incoming data is buffered. This is the default.

    This option is useful if you only read the data at certain points
    in time (e.g., in a real-time streaming application) or if you
    want to protect your socket against receiving too much data,
    which may eventually cause your application to run out of memory.

    Only QTcpSocket uses QAbstractSocket's internal buffer; QUdpSocket
    does not use any buffering at all, but rather relies on the
    implicit buffering provided by the operating system.
    Because of this, calling this function on QUdpSocket has no
    effect.

    \sa readBufferSize(), read()
*/
void QAbstractSocket::setReadBufferSize(qint64 size)
{
    Q_D(QAbstractSocket);

#ifndef QT_NO_OPENSSL
    // Manual polymorphism; setReadBufferSize() isn't virtual, but QSslSocket overloads
    // it.
    if (QSslSocket *socket = qobject_cast<QSslSocket *>(this)) {
        socket->setReadBufferSize(size);
        return;
    }
#endif

    if (d->readBufferMaxSize == size)
        return;
    d->readBufferMaxSize = size;
    if (!d->readSocketNotifierCalled && d->socketEngine) {
        // ensure that the read notification is enabled if we've now got
        // room in the read buffer
        // but only if we're not inside canReadNotification -- that will take care on its own
        if (size == 0 || d->readBuffer.size() < size)
            d->socketEngine->setReadNotificationEnabled(true);
    }
}

/*!
    Returns the state of the socket.

    \sa error()
*/
QAbstractSocket::SocketState QAbstractSocket::state() const
{
    return d_func()->state;
}

/*!
    Sets the state of the socket to \a state.

    \sa state()
*/
void QAbstractSocket::setSocketState(SocketState state)
{
    d_func()->state = state;
}

/*!
    Returns the socket type (TCP, UDP, or other).

    \sa QTcpSocket, QUdpSocket
*/
QAbstractSocket::SocketType QAbstractSocket::socketType() const
{
    return d_func()->socketType;
}

/*!
    Returns the type of error that last occurred.

    \sa state(), errorString()
*/
QAbstractSocket::SocketError QAbstractSocket::error() const
{
    return d_func()->socketError;
}

/*!
    Sets the type of error that last occurred to \a socketError.

    \sa setSocketState(), setErrorString()
*/
void QAbstractSocket::setSocketError(SocketError socketError)
{
    d_func()->socketError = socketError;
}

#ifndef QT_NO_NETWORKPROXY
/*!
    \since 4.1

    Sets the explicit network proxy for this socket to \a networkProxy.

    To disable the use of a proxy for this socket, use the
    QNetworkProxy::NoProxy proxy type:

    \snippet doc/src/snippets/code/src.network.socket.qabstractsocket.cpp 3


    \sa proxy(), QNetworkProxy
*/
void QAbstractSocket::setProxy(const QNetworkProxy &networkProxy)
{
    Q_D(QAbstractSocket);
    if (!d->proxy)
        d->proxy = new QNetworkProxy();
    *d->proxy = networkProxy;
}

/*!
    \since 4.1

    Returns the network proxy for this socket.
    By default QNetworkProxy::DefaultProxy is used.

    \sa setProxy(), QNetworkProxy
*/
QNetworkProxy QAbstractSocket::proxy() const
{
    Q_D(const QAbstractSocket);
    if (d->proxy)
        return *d->proxy;
    return QNetworkProxy();
}
#endif // QT_NO_NETWORKPROXY

#ifdef QT3_SUPPORT
/*! \enum QAbstractSocket::Error
    \compat

    Use QAbstractSocket::SocketError instead.

    \value ErrConnectionRefused Use QAbstractSocket::ConnectionRefusedError instead.
    \value ErrHostNotFound Use QAbstractSocket::HostNotFoundError instead.
    \value ErrSocketRead Use QAbstractSocket::UnknownSocketError instead.
*/

/*!
    \typedef QAbstractSocket::State
    \compat

    Use QAbstractSocket::SocketState instead.

    \table
    \header \o Qt 3 enum value \o Qt 4 enum value
    \row \o \c Idle            \o \l UnconnectedState
    \row \o \c HostLookup      \o \l HostLookupState
    \row \o \c Connecting      \o \l ConnectingState
    \row \o \c Connected       \o \l ConnectedState
    \row \o \c Closing         \o \l ClosingState
    \row \o \c Connection      \o \l ConnectedState
    \endtable
*/

/*!
    \fn int QAbstractSocket::socket() const

    Use socketDescriptor() instead.
*/

/*!
    \fn void QAbstractSocket::setSocket(int socket)

    Use setSocketDescriptor() instead.
*/

/*!
    \fn Q_ULONG QAbstractSocket::waitForMore(int msecs, bool *timeout = 0) const

    Use waitForReadyRead() instead.

    \oldcode
        bool timeout;
        Q_ULONG numBytes = socket->waitForMore(30000, &timeout);
    \newcode
        qint64 numBytes = 0;
        if (socket->waitForReadyRead(msecs))
            numBytes = socket->bytesAvailable();
        bool timeout = (error() == QAbstractSocket::SocketTimeoutError);
    \endcode

    \sa waitForReadyRead(), bytesAvailable(), error(), SocketTimeoutError
*/

/*!
    \fn void QAbstractSocket::connectionClosed()

    Use closed() instead.
*/

/*!
    \fn void QAbstractSocket::delayedCloseFinished()

    Use closed() instead.
*/
#endif // QT3_SUPPORT

#ifndef QT_NO_DEBUG_STREAM
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        debug << "QAbstractSocket::ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        debug << "QAbstractSocket::RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        debug << "QAbstractSocket::HostNotFoundError";
        break;
    case QAbstractSocket::SocketAccessError:
        debug << "QAbstractSocket::SocketAccessError";
        break;
    case QAbstractSocket::SocketResourceError:
        debug << "QAbstractSocket::SocketResourceError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        debug << "QAbstractSocket::SocketTimeoutError";
        break;
    case QAbstractSocket::DatagramTooLargeError:
        debug << "QAbstractSocket::DatagramTooLargeError";
        break;
    case QAbstractSocket::NetworkError:
        debug << "QAbstractSocket::NetworkError";
        break;
    case QAbstractSocket::AddressInUseError:
        debug << "QAbstractSocket::AddressInUseError";
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        debug << "QAbstractSocket::SocketAddressNotAvailableError";
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        debug << "QAbstractSocket::UnsupportedSocketOperationError";
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        debug << "QAbstractSocket::UnfinishedSocketOperationError";
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        debug << "QAbstractSocket::ProxyAuthenticationRequiredError";
        break;
    case QAbstractSocket::UnknownSocketError:
        debug << "QAbstractSocket::UnknownSocketError";
        break;
    default:
        debug << "QAbstractSocket::SocketError(" << int(error) << ")";
        break;
    }
    return debug;
}

Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        debug << "QAbstractSocket::UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        debug << "QAbstractSocket::HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        debug << "QAbstractSocket::ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        debug << "QAbstractSocket::ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        debug << "QAbstractSocket::BoundState";
        break;
    case QAbstractSocket::ListeningState:
        debug << "QAbstractSocket::ListeningState";
        break;
    case QAbstractSocket::ClosingState:
        debug << "QAbstractSocket::ClosingState";
        break;
    default:
        debug << "QAbstractSocket::SocketState(" << int(state) << ")";
        break;
    }
    return debug;
}
#endif

QT_END_NAMESPACE

#include "moc_qabstractsocket.cpp"
