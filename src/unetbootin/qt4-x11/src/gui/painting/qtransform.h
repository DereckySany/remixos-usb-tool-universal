/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QTRANSFORM_H
#define QTRANSFORM_H

#include <QtGui/qmatrix.h>
#include <QtGui/qpainterpath.h>
#include <QtGui/qpolygon.h>
#include <QtGui/qregion.h>
#include <QtGui/qwindowdefs.h>
#include <QtCore/qline.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QVariant;

class Q_GUI_EXPORT QTransform
{
    Q_ENUMS(TransformationType)
public:
    enum TransformationType {
        TxNone      = 0x00,
        TxTranslate = 0x01,
        TxScale     = 0x02,
        TxRotate    = 0x04,
        TxShear     = 0x08,
        TxProject   = 0x10
    };

    QTransform();
    QTransform(qreal h11, qreal h12, qreal h13,
               qreal h21, qreal h22, qreal h23,
               qreal h31, qreal h32, qreal h33 = 1.0);
    QTransform(qreal h11, qreal h12, qreal h21,
               qreal h22, qreal dx, qreal dy);
    explicit QTransform(const QMatrix &mtx);

    bool isAffine() const;
    bool isIdentity() const;
    bool isInvertible() const;
    bool isScaling() const;
    bool isRotating() const;
    bool isTranslating() const;

    TransformationType type() const;

    inline qreal determinant() const;
    qreal det() const;

    qreal m11() const;
    qreal m12() const;
    qreal m13() const;
    qreal m21() const;
    qreal m22() const;
    qreal m23() const;
    qreal m31() const;
    qreal m32() const;
    qreal m33() const;
    qreal dx() const;
    qreal dy() const;

    void setMatrix(qreal m11, qreal m12, qreal m13,
                   qreal m21, qreal m22, qreal m23,
                   qreal m31, qreal m32, qreal m33);

    QTransform inverted(bool *invertible = 0) const;
    QTransform adjoint() const;
    QTransform transposed() const;

    QTransform &translate(qreal dx, qreal dy);
    QTransform &scale(qreal sx, qreal sy);
    QTransform &shear(qreal sh, qreal sv);
    QTransform &rotate(qreal a, Qt::Axis axis = Qt::ZAxis);
    QTransform &rotateRadians(qreal a, Qt::Axis axis = Qt::ZAxis);

    static bool squareToQuad(const QPolygonF &square, QTransform &result);
    static bool quadToSquare(const QPolygonF &quad, QTransform &result);
    static bool quadToQuad(const QPolygonF &one,
                           const QPolygonF &two,
                           QTransform &result);

    bool operator==(const QTransform &) const;
    bool operator!=(const QTransform &) const;

    QTransform &operator*=(const QTransform &);
    QTransform operator*(const QTransform &o) const;

    QTransform &operator=(const QTransform &);

    operator QVariant() const;

    void reset();
    QPoint       map(const QPoint &p) const;
    QPointF      map(const QPointF &p) const;
    QLine        map(const QLine &l) const;
    QLineF       map(const QLineF &l) const;
    QPolygonF    map(const QPolygonF &a) const;
    QPolygon     map(const QPolygon &a) const;
    QRegion      map(const QRegion &r) const;
    QPainterPath map(const QPainterPath &p) const;
    QPolygon     mapToPolygon(const QRect &r) const;
    QRect mapRect(const QRect &) const;
    QRectF mapRect(const QRectF &) const;
    void map(int x, int y, int *tx, int *ty) const;
    void map(qreal x, qreal y, qreal *tx, qreal *ty) const;

    const QMatrix &toAffine() const;

    QTransform &operator*=(qreal div);
    QTransform &operator/=(qreal div);
    QTransform &operator+=(qreal div);
    QTransform &operator-=(qreal div);

    static QTransform fromTranslate(qreal dx, qreal dy);
    static QTransform fromScale(qreal dx, qreal dy);

private:
    QMatrix affine;
    qreal   m_13;
    qreal   m_23;
    qreal   m_33;

    mutable uint m_type : 5;
    mutable uint m_dirty : 5;

    class Private;
    Private *d;
};
Q_DECLARE_TYPEINFO(QTransform, Q_MOVABLE_TYPE);

/******* inlines *****/
inline bool QTransform::isAffine() const
{
    return type() < TxProject;
}
inline bool QTransform::isIdentity() const
{
    return type() == TxNone;
}

inline bool QTransform::isInvertible() const
{
    return !qFuzzyCompare(determinant() + 1, 1);
}

inline bool QTransform::isScaling() const
{
    return type() >= TxScale;
}
inline bool QTransform::isRotating() const
{
    return type() >= TxRotate;
}

inline bool QTransform::isTranslating() const
{
    return type() >= TxTranslate;
}

inline qreal QTransform::determinant() const
{
    return affine._m11*(m_33*affine._m22-affine._dy*m_23) -
        affine._m21*(m_33*affine._m12-affine._dy*m_13)+affine._dx*(m_23*affine._m12-affine._m22*m_13);
}
inline qreal QTransform::det() const
{
    return determinant();
}
inline qreal QTransform::m11() const
{
    return affine._m11;
}
inline qreal QTransform::m12() const
{
    return affine._m12;
}
inline qreal QTransform::m13() const
{
    return m_13;
}
inline qreal QTransform::m21() const
{
    return affine._m21;
}
inline qreal QTransform::m22() const
{
    return affine._m22;
}
inline qreal QTransform::m23() const
{
    return m_23;
}
inline qreal QTransform::m31() const
{
    return affine._dx;
}
inline qreal QTransform::m32() const
{
    return affine._dy;
}
inline qreal QTransform::m33() const
{
    return m_33;
}
inline qreal QTransform::dx() const
{
    return affine._dx;
}
inline qreal QTransform::dy() const
{
    return affine._dy;
}

inline QTransform &QTransform::operator*=(qreal num)
{
    affine._m11 *= num;
    affine._m12 *= num;
    m_13        *= num;
    affine._m21 *= num;
    affine._m22 *= num;
    m_23        *= num;
    affine._dx  *= num;
    affine._dy  *= num;
    m_33        *= num;
    m_dirty     |= TxScale;
    return *this;
}
inline QTransform &QTransform::operator/=(qreal div)
{
    div = 1/div;
    return operator*=(div);
}
inline QTransform &QTransform::operator+=(qreal num)
{
    affine._m11 += num;
    affine._m12 += num;
    m_13        += num;
    affine._m21 += num;
    affine._m22 += num;
    m_23        += num;
    affine._dx  += num;
    affine._dy  += num;
    m_33        += num;
    m_dirty     |= TxProject;
    return *this;
}
inline QTransform &QTransform::operator-=(qreal num)
{
    affine._m11 -= num;
    affine._m12 -= num;
    m_13        -= num;
    affine._m21 -= num;
    affine._m22 -= num;
    m_23        -= num;
    affine._dx  -= num;
    affine._dy  -= num;
    m_33        -= num;
    m_dirty     |= TxProject;
    return *this;
}

/****** stream functions *******************/
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QTransform &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QTransform &);

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QTransform &);
#endif
/****** end stream functions *******************/

// mathematical semantics
Q_GUI_EXPORT_INLINE QPoint operator*(const QPoint &p, const QTransform &m)
{ return m.map(p); }
Q_GUI_EXPORT_INLINE QPointF operator*(const QPointF &p, const QTransform &m)
{ return m.map(p); }
Q_GUI_EXPORT_INLINE QLineF operator*(const QLineF &l, const QTransform &m)
{ return m.map(l); }
Q_GUI_EXPORT_INLINE QLine operator*(const QLine &l, const QTransform &m)
{ return m.map(l); }
Q_GUI_EXPORT_INLINE QPolygon operator *(const QPolygon &a, const QTransform &m)
{ return m.map(a); }
Q_GUI_EXPORT_INLINE QPolygonF operator *(const QPolygonF &a, const QTransform &m)
{ return m.map(a); }
Q_GUI_EXPORT_INLINE QRegion operator *(const QRegion &r, const QTransform &m)
{ return m.map(r); }
Q_GUI_EXPORT_INLINE QPainterPath operator *(const QPainterPath &p, const QTransform &m)
{ return m.map(p); }

Q_GUI_EXPORT_INLINE QTransform operator *(const QTransform &a, qreal n)
{ QTransform t(a); t *= n; return t; }
Q_GUI_EXPORT_INLINE QTransform operator /(const QTransform &a, qreal n)
{ QTransform t(a); t /= n; return t; }
Q_GUI_EXPORT_INLINE QTransform operator +(const QTransform &a, qreal n)
{ QTransform t(a); t += n; return t; }
Q_GUI_EXPORT_INLINE QTransform operator -(const QTransform &a, qreal n)
{ QTransform t(a); t -= n; return t; }

QT_END_NAMESPACE

QT_END_HEADER

#endif
