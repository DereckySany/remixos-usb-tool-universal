/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "qfilesystemwatcher.h"
#include "qfilesystemwatcher_inotify_p.h"

#ifndef QT_NO_FILESYSTEMWATCHER

#include <qdebug.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qsocketnotifier.h>
#include <qvarlengtharray.h>

#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#if defined(QT_NO_INOTIFY)
#include <linux/types.h>

#if defined(__i386__)
# define __NR_inotify_init      291
# define __NR_inotify_add_watch 292
# define __NR_inotify_rm_watch  293
#elif defined(__x86_64__)
# define __NR_inotify_init      253
# define __NR_inotify_add_watch 254
# define __NR_inotify_rm_watch  255
#elif defined(__powerpc__) || defined(__powerpc64__)
# define __NR_inotify_init      275
# define __NR_inotify_add_watch 276
# define __NR_inotify_rm_watch  277
#elif defined (__ia64__)
# define __NR_inotify_init      1277
# define __NR_inotify_add_watch 1278
# define __NR_inotify_rm_watch  1279
#elif defined (__s390__) || defined (__s390x__)
# define __NR_inotify_init      284
# define __NR_inotify_add_watch 285
# define __NR_inotify_rm_watch  286
#elif defined (__alpha__)
# define __NR_inotify_init      444
# define __NR_inotify_add_watch 445
# define __NR_inotify_rm_watch  446
#elif defined (__sparc__) || defined (__sparc64__)
# define __NR_inotify_init      151
# define __NR_inotify_add_watch 152
# define __NR_inotify_rm_watch  156
#elif defined (__arm__)
# define __NR_inotify_init      316
# define __NR_inotify_add_watch 317
# define __NR_inotify_rm_watch  318
#elif defined (__SH4__)
# define __NR_inotify_init      290
# define __NR_inotify_add_watch 291
# define __NR_inotify_rm_watch  292
#elif defined (__SH5__)
# define __NR_inotify_init      318
# define __NR_inotify_add_watch 319
# define __NR_inotify_rm_watch  320
#elif defined (__mips__)
# define __NR_inotify_init      284
# define __NR_inotify_add_watch 285
# define __NR_inotify_rm_watch  286
#elif defined (__hppa__)
# define __NR_inotify_init      269
# define __NR_inotify_add_watch 270
# define __NR_inotify_rm_watch  271
#elif defined (__avr32__)
# define __NR_inotify_init	240
# define __NR_inotify_add_watch	241
# define __NR_inotify_rm_watch	242
#else
# error "This architecture is not supported. Please talk to qt-bugs@trolltech.com"
#endif

QT_BEGIN_NAMESPACE

#ifdef QT_LSB
// ### the LSB doesn't standardize syscall, need to wait until glib2.4 is standardized
static inline int syscall(...) { return -1; }
#endif

static inline int inotify_init()
{
    return syscall(__NR_inotify_init);
}

static inline int inotify_add_watch(int fd, const char *name, __u32 mask)
{
    return syscall(__NR_inotify_add_watch, fd, name, mask);
}

static inline int inotify_rm_watch(int fd, __u32 wd)
{
    return syscall(__NR_inotify_rm_watch, fd, wd);
}

// the following struct and values are documented in linux/inotify.h
extern "C" {

struct inotify_event {
        __s32           wd;
        __u32           mask;
        __u32           cookie;
        __u32           len;
        char            name[0];
};

#define IN_ACCESS               0x00000001
#define IN_MODIFY               0x00000002
#define IN_ATTRIB               0x00000004
#define IN_CLOSE_WRITE          0x00000008
#define IN_CLOSE_NOWRITE        0x00000010
#define IN_OPEN                 0x00000020
#define IN_MOVED_FROM           0x00000040
#define IN_MOVED_TO             0x00000080
#define IN_CREATE               0x00000100
#define IN_DELETE               0x00000200
#define IN_DELETE_SELF          0x00000400
#define IN_MOVE_SELF            0x00000800
#define IN_UNMOUNT              0x00002000
#define IN_Q_OVERFLOW           0x00004000

#define IN_CLOSE                (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE)
#define IN_MOVE                 (IN_MOVED_FROM | IN_MOVED_TO)
}

QT_END_NAMESPACE

// --------- inotify.h end ----------

#else /* QT_NO_INOTIFY */

#include <sys/inotify.h>

#endif

QT_BEGIN_NAMESPACE

QInotifyFileSystemWatcherEngine *QInotifyFileSystemWatcherEngine::create()
{
    int fd = inotify_init();
    if (fd <= 0)
        return 0;
    return new QInotifyFileSystemWatcherEngine(fd);
}

QInotifyFileSystemWatcherEngine::QInotifyFileSystemWatcherEngine(int fd)
    : inotifyFd(fd)
{
    fcntl(inotifyFd, F_SETFD, FD_CLOEXEC);

    moveToThread(this);
}

QInotifyFileSystemWatcherEngine::~QInotifyFileSystemWatcherEngine()
{
    foreach (int id, pathToID.values())
        inotify_rm_watch(inotifyFd, id < 0 ? -id : id);

    ::close(inotifyFd);
}

void QInotifyFileSystemWatcherEngine::run()
{
    QSocketNotifier sn(inotifyFd, QSocketNotifier::Read, this);
    connect(&sn, SIGNAL(activated(int)), SLOT(readFromInotify()));
    (void) exec();
}

QStringList QInotifyFileSystemWatcherEngine::addPaths(const QStringList &paths,
                                                      QStringList *files,
                                                      QStringList *directories)
{
    QMutexLocker locker(&mutex);

    QStringList p = paths;
    QMutableListIterator<QString> it(p);
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo fi(path);
        bool isDir = fi.isDir();
        if (isDir) {
            if (directories->contains(path))
                continue;
        } else {
            if (files->contains(path))
                continue;
        }

        int wd = inotify_add_watch(inotifyFd,
                                   QFile::encodeName(path),
                                   (isDir
                                    ? (0
                                       | IN_ATTRIB
                                       | IN_MOVE
                                       | IN_CREATE
                                       | IN_DELETE
                                       | IN_DELETE_SELF
                                       )
                                    : (0
                                       | IN_ATTRIB
                                       | IN_MODIFY
                                       | IN_MOVE
                                       | IN_DELETE_SELF
                                       )));
        if (wd <= 0) {
            perror("QInotifyFileSystemWatcherEngine::addPaths: inotify_add_watch failed");
            continue;
        }

        it.remove();

        int id = isDir ? -wd : wd;
        if (id < 0) {
            directories->append(path);
        } else {
            files->append(path);
        }

        pathToID.insert(path, id);
        idToPath.insert(id, path);
    }

    start();

    return p;
}

QStringList QInotifyFileSystemWatcherEngine::removePaths(const QStringList &paths,
                                                         QStringList *files,
                                                         QStringList *directories)
{
    QMutexLocker locker(&mutex);

    QStringList p = paths;
    QMutableListIterator<QString> it(p);
    while (it.hasNext()) {
        QString path = it.next();
        int id = pathToID.take(path);
        QString x = idToPath.take(id);
        if (x.isEmpty() || x != path)
            continue;

        int wd = id < 0 ? -id : id;
        // qDebug() << "removing watch for path" << path << "wd" << wd;
        inotify_rm_watch(inotifyFd, wd);

        it.remove();
        if (id < 0) {
            directories->removeAll(path);
        } else {
            files->removeAll(path);
        }
    }

    return p;
}

void QInotifyFileSystemWatcherEngine::stop()
{
    QMetaObject::invokeMethod(this, "quit");
}

void QInotifyFileSystemWatcherEngine::readFromInotify()
{
    QMutexLocker locker(&mutex);

    // qDebug() << "QInotifyFileSystemWatcherEngine::readFromInotify";

    int buffSize = 0;
    ioctl(inotifyFd, FIONREAD, &buffSize);
    QVarLengthArray<char, 4096> buffer(buffSize);
    buffSize = read(inotifyFd, buffer.data(), buffSize);
    inotify_event *ev = reinterpret_cast<inotify_event *>(buffer.data());
    inotify_event *end = reinterpret_cast<inotify_event *>(buffer.data() + buffSize);
    while (ev < end) {
        // qDebug() << "inotify event, wd" << ev->wd << "mask" << hex << ev->mask;

        int id = ev->wd;
        QString path = idToPath.value(id);
        if (path.isEmpty()) {
            // perhaps a directory?
            id = -id;
            path = idToPath.value(id);
            if (path.isEmpty()) {
                ev += sizeof(inotify_event) + ev->len;
                continue;
            }
        }

        // qDebug() << "event for path" << path;

        if ((ev->mask & (IN_DELETE_SELF | IN_UNMOUNT)) != 0) {
            pathToID.remove(path);
            idToPath.remove(id);
            inotify_rm_watch(inotifyFd, ev->wd);

            if (id < 0)
                emit directoryChanged(path, true);
            else
                emit fileChanged(path, true);
        } else {
            if (id < 0)
                emit directoryChanged(path, false);
            else
                emit fileChanged(path, false);
        }

        ev += sizeof(inotify_event) + ev->len;
    }
}

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER
