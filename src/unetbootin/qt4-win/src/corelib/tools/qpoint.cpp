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

#include "qpoint.h"
#include "qdatastream.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

/*!
    \class QPoint
    \ingroup multimedia

    \brief The QPoint class defines a point in the plane using integer
    precision.

    A point is specified by a x coordinate and an y coordinate which
    can be accessed using the x() and y() functions. The isNull()
    function returns true if both x and y are set to 0. The
    coordinates can be set (or altered) using the setX() and setY()
    functions, or alternatively the rx() and ry() functions which
    return references to the coordinates (allowing direct
    manipulation).

    Given a point \e p, the following statements are all equivalent:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 0

    A QPoint object can also be used as a vector: Addition and
    subtraction are defined as for vectors (each component is added
    separately). A QPoint object can also be divided or multiplied by
    an \c int or a \c qreal.

    In addition, the QPoint class provides the manhattanLength()
    function which gives an inexpensive approximation of the length of
    the QPoint object interpreted as a vector. Finally, QPoint objects
    can be streamed as well as compared.

    \sa QPointF, QPolygon
*/


/*****************************************************************************
  QPoint member functions
 *****************************************************************************/

/*!
    \fn QPoint::QPoint()

    Constructs a null point, i.e. with coordinates (0, 0)

    \sa isNull()
*/

/*!
    \fn QPoint::QPoint(int x, int y)

    Constructs a point with the given coordinates (\a x, \a  y).

    \sa setX(), setY()
*/

/*!
    \fn bool QPoint::isNull() const

    Returns true if both the x and y coordinates are set to 0,
    otherwise returns false.
*/

/*!
    \fn int QPoint::x() const

    Returns the x coordinate of this point.

    \sa setX(), rx()
*/

/*!
    \fn int QPoint::y() const

    Returns the y coordinate of this point.

    \sa setY(), ry()
*/

/*!
    \fn void QPoint::setX(int x)

    Sets the x coordinate of this point to the given \a x coordinate.

    \sa x() setY()
*/

/*!
    \fn void QPoint::setY(int y)

    Sets the y coordinate of this point to the given \a y coordinate.

    \sa y() setX()
*/


/*!
    \fn int &QPoint::rx()

    Returns a reference to the x coordinate of this point.

    Using a reference makes it possible to directly manipulate x. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 1

    \sa x() setX()
*/

/*!
    \fn int &QPoint::ry()

    Returns a reference to the y coordinate of this point.

    Using a reference makes it possible to directly manipulate y. For
    example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 2

    \sa y(), setY()
*/


/*!
    \fn QPoint &QPoint::operator+=(const QPoint &point)

    Adds the given \a point to this point and returns a reference to
    this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 3

    \sa operator-=()
*/

/*!
    \fn QPoint &QPoint::operator-=(const QPoint &point)

    Subtracts the given \a point from this point and returns a
    reference to this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 4

    \sa operator+=()
*/

/*!
    \fn QPoint &QPoint::operator*=(qreal factor)

    Multiplies this point's coordinates by the given \a factor, and
    returns a reference to this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 5

    Note that the result is rounded to the nearest integer as points are held as
    integers. Use QPointF for floating point accuracy.

    \sa operator/=()
*/


/*!
    \fn bool operator==(const QPoint &p1, const QPoint &p2)
    \relates QPoint

    Returns true if \a p1 and \a p2 are equal; otherwise returns
    false.
*/

/*!
    \fn bool operator!=(const QPoint &p1, const QPoint &p2)
    \relates QPoint

    Returns true if \a p1 and \a p2 are not equal; otherwise returns false.
*/

/*!
    \fn const QPoint operator+(const QPoint &p1, const QPoint &p2)
    \relates QPoint

    Returns a QPoint object that is the sum of the given points, \a p1
    and \a p2; each component is added separately.

    \sa QPoint::operator+=()
*/

/*!
    \fn const QPoint operator-(const QPoint &p1, const QPoint &p2)
    \relates QPoint

    Returns a QPoint object that is formed by subtracting \a p2 from
    \a p1; each component is subtracted separately.

    \sa QPoint::operator-=()
*/

/*!
    \fn const QPoint operator*(const QPoint &point, qreal factor)
    \relates QPoint

    Returns a copy of the given \a point multiplied by the given \a factor.

    Note that the result is rounded to the nearest integer as points
    are held as integers. Use QPointF for floating point accuracy.

    \sa QPoint::operator*=()
*/

/*!
    \fn const QPoint operator*(qreal factor, const QPoint &point)
    \overload
    \relates QPoint

    Returns a copy of the given \a point multiplied by the given \a factor.
*/

/*!
    \fn const QPoint operator-(const QPoint &point)
    \overload
    \relates QPoint

    Returns a QPoint object that is formed by changing the sign of
    both components of the given \a point.

    Equivalent to \c{QPoint(0,0) - point}.
*/

/*!
    \fn QPoint &QPoint::operator/=(qreal divisor)
    \overload

    Divides both x and y by the given \a divisor, and returns a reference to this
    point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 6

    Note that the result is rounded to the nearest integer as points are held as
    integers. Use QPointF for floating point accuracy.

    \sa operator*=()
*/

/*!
    \fn const QPoint operator/(const QPoint &point, qreal divisor)
    \relates QPoint

    Returns the QPoint formed by dividing both components of the given \a point
    by the given \a divisor.

    Note that the result is rounded to the nearest integer as points are held as
    integers. Use QPointF for floating point accuracy.

    \sa QPoint::operator/=()
*/

/*****************************************************************************
  QPoint stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QPoint &point)
    \relates QPoint

    Writes the given \a point to the given \a stream and returns a
    reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator<<(QDataStream &s, const QPoint &p)
{
    if (s.version() == 1)
        s << (qint16)p.x() << (qint16)p.y();
    else
        s << (qint32)p.x() << (qint32)p.y();
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QPoint &point)
    \relates QPoint

    Reads a point from the given \a stream into the given \a point
    and returns a reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator>>(QDataStream &s, QPoint &p)
{
    if (s.version() == 1) {
        qint16 x, y;
        s >> x;  p.rx() = x;
        s >> y;  p.ry() = y;
    }
    else {
        qint32 x, y;
        s >> x;  p.rx() = x;
        s >> y;  p.ry() = y;
    }
    return s;
}

#endif // QT_NO_DATASTREAM
/*!
    Returns the sum of the absolute values of x() and y(),
    traditionally known as the "Manhattan length" of the vector from
    the origin to the point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 7

    This is a useful, and quick to calculate, approximation to the
    true length:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 8

    The tradition of "Manhattan length" arises because such distances
    apply to travelers who can only travel on a rectangular grid, like
    the streets of Manhattan.
*/
int QPoint::manhattanLength() const
{
    return qAbs(x())+qAbs(y());
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QPoint &p) {
    dbg.nospace() << "QPoint(" << p.x() << ',' << p.y() << ')';
    return dbg.space();
}

QDebug operator<<(QDebug d, const QPointF &p)
{
    d.nospace() << "QPointF(" << p.x() << ", " << p.y() << ")";
    return d;
}
#endif

/*!
    \class QPointF
    \ingroup multimedia

    \brief The QPointF class defines a point in the plane using
    floating point precision.

    A point is specified by a x coordinate and an y coordinate which
    can be accessed using the x() and y() functions. The coordinates
    of the point are specified using floating point numbers for
    accuracy. The isNull() function returns true if both x and y are
    set to 0.0. The coordinates can be set (or altered) using the setX()
    and setY() functions, or alternatively the rx() and ry() functions which
    return references to the coordinates (allowing direct
    manipulation).

    Given a point \e p, the following statements are all equivalent:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 9

    A QPointF object can also be used as a vector: Addition and
    subtraction are defined as for vectors (each component is added
    separately). A QPointF object can also be divided or multiplied by
    an \c int or a \c qreal.

    In addition, the QPointF class provides a constructor converting a
    QPoint object into a QPointF object, and a corresponding toPoint()
    function which returns a QPoint copy of \e this point. Finally,
    QPointF objects can be streamed as well as compared.

    \sa QPoint, QPolygonF
*/

/*!
    \fn QPointF::QPointF()

    Constructs a null point, i.e. with coordinates (0.0, 0.0)

    \sa isNull()
*/

/*!
    \fn QPointF::QPointF(const QPoint &point)

    Constructs a copy of the given \a point.

    \sa toPoint()
*/

/*!
    \fn QPointF::QPointF(qreal x, qreal y)

    Constructs a point with the given coordinates (\a x, \a y).

    \sa setX(), setY()
*/

/*!
    \fn bool QPointF::isNull() const

    Returns true if both the x and y coordinates are set to 0.0,
    otherwise returns false.
*/

/*!
    \fn qreal QPointF::x() const

    Returns the x-coordinate of this point.

    \sa setX(), rx()
*/

/*!
    \fn qreal QPointF::y() const

    Returns the y-coordinate of this point.

    \sa setY(), ry()
*/

/*!
    \fn void QPointF::setX(qreal x)

    Sets the x coordinate of this point to the given \a x coordinate.

    \sa x() setY()
*/

/*!
    \fn void QPointF::setY(qreal y)

    Sets the y coordinate of this point to the given \a y coordinate.

    \sa  y(), setX()
*/

/*!
    \fn qreal& QPointF::rx()

    Returns a reference to the x coordinate of this point.

    Using a reference makes it possible to directly manipulate x. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 10

    \sa x(), setX()
*/

/*!
    \fn qreal& QPointF::ry()

    Returns a reference to the y coordinate of this point.

    Using a reference makes it possible to directly manipulate y. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 11

    \sa y() setY()
*/

/*!
    \fn QPointF& QPointF::operator+=(const QPointF &point)

    Adds the given \a point to this point and returns a reference to
    this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 12

    \sa operator-=()
*/

/*!
    \fn QPointF& QPointF::operator-=(const QPointF &point)

    Subtracts the given \a point from this point and returns a reference
    to this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 13

    \sa operator+=()
*/

/*!
    \fn QPointF& QPointF::operator*=(qreal factor)

    Multiplies this point's coordinates by the given \a factor, and
    returns a reference to this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 14

    \sa operator/=()
*/

/*!
    \fn QPointF& QPointF::operator/=(qreal divisor)

    Divides both x and y by the given \a divisor, and returns a reference
    to this point. For example:

    \snippet doc/src/snippets/code/src.corelib.tools.qpoint.cpp 15

    \sa operator*=()
*/

/*!
    \fn const QPointF operator+(const QPointF &p1, const QPointF &p2)
    \relates QPointF

    Returns a QPointF object that is the sum of the given points, \a p1
    and \a p2; each component is added separately.

    \sa QPointF::operator+=()
*/

/*!
    \fn const QPointF operator-(const QPointF &p1, const QPointF &p2)
    \relates QPointF

    Returns a QPointF object that is formed by subtracting \a p2 from \a p1;
    each component is subtracted separately.

    \sa QPointF::operator-=()
*/

/*!
    \fn const QPointF operator*(const QPointF &point, qreal factor)
    \relates QPointF

    Returns a copy of the given \a point,  multiplied by the given \a factor.

    \sa QPointF::operator*=()
*/

/*!
    \fn const QPointF operator*(qreal factor, const QPointF &point)
    \relates QPointF

    \overload

    Returns a copy of the given \a point, multiplied by the given \a factor.
*/

/*!
    \fn const QPointF operator-(const QPointF &point)
    \relates QPointF
    \overload

    Returns a QPointF object that is formed by changing the sign of
    both components of the given \a point.

    Equivalent to \c {QPointF(0,0) - point}.
*/

/*!
    \fn const QPointF operator/(const QPointF &point, qreal divisor)
    \relates QPointF

    Returns the QPointF object formed by dividing both components of
    the given \a point by the given \a divisor.

    \sa QPointF::operator/=()
*/

/*!
    \fn QPoint QPointF::toPoint() const

    Rounds the coordinates of this point to the nearest integer, and
    returns a QPoint object with the rounded coordinates.

    \sa QPointF()
*/

/*!
    \fn bool operator==(const QPointF &p1, const QPointF &p2)
    \relates QPointF

    Returns true if \a p1 is equal to \a p2; otherwise returns false.
*/

/*!
    \fn bool operator!=(const QPointF &p1, const QPointF &p2);
    \relates QPointF

    Returns true if \a p1 is not equal to \a p2; otherwise returns false.
*/

#ifndef QT_NO_DATASTREAM
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QPointF &point)
    \relates QPointF

    Writes the given \a point to the given \a stream and returns a
    reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator<<(QDataStream &s, const QPointF &p)
{
    s << double(p.x()) << double(p.y());
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QPointF &point)
    \relates QPointF

    Reads a point from the given \a stream into the given \a point
    and returns a reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator>>(QDataStream &s, QPointF &p)
{
    double x, y;
    s >> x;
    s >> y;
    p.setX(qreal(x));
    p.setY(qreal(y));
    return s;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE
