/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QDRAWHELPER_P_H
#define QDRAWHELPER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qglobal.h"
#include "QtGui/qcolor.h"
#include "QtGui/qpainter.h"
#include "QtGui/qimage.h"
#ifndef QT_FT_BEGIN_HEADER
#define QT_FT_BEGIN_HEADER
#define QT_FT_END_HEADER
#endif
#include "private/qrasterdefs_p.h"

#ifdef Q_WS_QWS
#include "QtGui/qscreen_qws.h"
#endif

QT_BEGIN_NAMESPACE

#if defined(Q_CC_MSVC) && _MSCVER <= 1300 && !defined(Q_CC_INTEL)
#define Q_STATIC_TEMPLATE_SPECIALIZATION static
#else
#define Q_STATIC_TEMPLATE_SPECIALIZATION
#endif

/*******************************************************************************
 * QSpan
 *
 * duplicate definition of FT_Span
 */
typedef QT_FT_Span QSpan;

struct QSolidData;
struct QTextureData;
struct QGradientData;
struct QLinearGradientData;
struct QRadialGradientData;
struct QConicalGradientData;
struct QSpanData;
class QGradient;
class QRasterBuffer;

typedef QT_FT_SpanFunc ProcessSpans;
typedef void (*BitmapBlitFunc)(QRasterBuffer *rasterBuffer,
                               int x, int y, quint32 color,
                               const uchar *bitmap,
                               int mapWidth, int mapHeight, int mapStride);
typedef void (*AlphamapBlitFunc)(QRasterBuffer *rasterBuffer,
                                 int x, int y, quint32 color,
                                 const uchar *bitmap,
                                 int mapWidth, int mapHeight, int mapStride);
typedef void (*RectFillFunc)(QRasterBuffer *rasterBuffer,
                             int x, int y, int width, int height,
                             quint32 color);

struct DrawHelper {
    ProcessSpans blendColor;
    ProcessSpans blendGradient;
    BitmapBlitFunc bitmapBlit;
    AlphamapBlitFunc alphamapBlit;
    RectFillFunc fillRect;
};

extern DrawHelper qDrawHelper[QImage::NImageFormats];
void qBlendTexture(int count, const QSpan *spans, void *userData);
#ifdef Q_WS_QWS
extern DrawHelper qDrawHelperCallback[QImage::NImageFormats];
void qBlendTextureCallback(int count, const QSpan *spans, void *userData);
#endif

typedef void QT_FASTCALL (*CompositionFunction)(uint *dest, const uint *src, int length, uint const_alpha);
typedef void QT_FASTCALL (*CompositionFunctionSolid)(uint *dest, int length, uint color, uint const_alpha);

void qInitDrawhelperAsm();

class QRasterPaintEngine;

struct QSolidData
{
    uint color;
};

struct QLinearGradientData
{
    struct {
        qreal x;
        qreal y;
    } origin;
    struct {
        qreal x;
        qreal y;
    } end;
};

struct QRadialGradientData
{
    struct {
        qreal x;
        qreal y;
    } center;
    struct {
        qreal x;
        qreal y;
    } focal;
    qreal radius;
};

struct QConicalGradientData
{
    struct {
        qreal x;
        qreal y;
    } center;
    qreal angle;
};

struct QGradientData
{
    QGradient::Spread spread;

    union {
        QLinearGradientData linear;
        QRadialGradientData radial;
        QConicalGradientData conical;
    };

#ifdef Q_WS_QWS
#define GRADIENT_STOPTABLE_SIZE 256
#else
#define GRADIENT_STOPTABLE_SIZE 1024
#endif

    uint* colorTable; //[GRADIENT_STOPTABLE_SIZE];

    uint alphaColor : 1;
};

struct QTextureData
{
    const uchar *imageData;
    const uchar *scanLine(int y) const { return imageData + y*bytesPerLine; }

    int width;
    int height;
    // clip rect
    int x1;
    int y1;
    int x2;
    int y2;
    int bytesPerLine;
    QImage::Format format;
    const QVector<QRgb> *colorTable;
    bool hasAlpha;
    enum Type {
        Plain,
        Tiled
    };
    Type type;
    int const_alpha;
};

struct QSpanData
{
    QSpanData() : tempImage(0) {}
    ~QSpanData() { delete tempImage; }

    QRasterBuffer *rasterBuffer;
#ifdef Q_WS_QWS
    QRasterPaintEngine *rasterEngine;
#endif
    ProcessSpans blend;
    ProcessSpans unclipped_blend;
    BitmapBlitFunc bitmapBlit;
    AlphamapBlitFunc alphamapBlit;
    RectFillFunc fillRect;
    qreal m11, m12, m13, m21, m22, m23, dx, dy;   // inverse xform matrix
    enum Type {
        None,
        Solid,
        LinearGradient,
        RadialGradient,
        ConicalGradient,
        Texture
    } type : 8;
    int txop : 8;
    int fast_matrix : 1;
    bool bilinear;
    QImage *tempImage;
    union {
        QSolidData solid;
        QGradientData gradient;
        QTextureData texture;
    };

    void init(QRasterBuffer *rb, QRasterPaintEngine *pe = 0);
    void setup(const QBrush &brush, int alpha);
    void setupMatrix(const QTransform &matrix, int bilinear);
    void initTexture(const QImage *image, int alpha, QTextureData::Type = QTextureData::Plain, const QRect &sourceRect = QRect());
    void adjustSpanMethods();
};

static inline uint BYTE_MUL(uint x, uint a) {
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return x;
}

static inline uint PREMUL(uint x) {
    uint a = x >> 24;
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff) * a;
    x = (x + ((x >> 8) & 0xff) + 0x80);
    x &= 0xff00;
    x |= t | (a << 24);
    return x;
}

#define INV_PREMUL(p)                                   \
    (qAlpha(p) == 0 ? 0 :                               \
    ((qAlpha(p) << 24)                                  \
     | (((255*qRed(p))/ qAlpha(p)) << 16)               \
     | (((255*qGreen(p)) / qAlpha(p))  << 8)            \
     | ((255*qBlue(p)) / qAlpha(p))))

template <class DST, class SRC>
inline DST qt_colorConvert(SRC color, DST dummy)
{
    Q_UNUSED(dummy);
    return DST(color);
}


template <>
inline quint32 qt_colorConvert(quint16 color, quint32 dummy)
{
    Q_UNUSED(dummy);
    const int r = (color & 0xf800);
    const int g = (color & 0x07e0);
    const int b = (color & 0x001f);
    const int tr = (r >> 8) | (r >> 13);
    const int tg = (g >> 3) | (g >> 9);
    const int tb = (b << 3) | (b >> 2);

    return qRgb(tr, tg, tb);
}

template <>
inline quint16 qt_colorConvert(quint32 color, quint16 dummy)
{
    Q_UNUSED(dummy);
    const int r = qRed(color) << 8;
    const int g = qGreen(color) << 3;
    const int b = qBlue(color) >> 3;

    return (r & 0xf800) | (g & 0x07e0)| (b & 0x001f);
}

class quint32p
{
public:
    inline quint32p(quint32 v) : data(PREMUL(v)) {}

    inline operator quint32() const { return data; }

    inline operator quint16() const
    {
        return qt_colorConvert<quint16, quint32>(data, 0);
    }

    static inline quint32p fromRawData(quint32 v)
    {
        quint32p p;
        p.data = v;
        return p;
    }

private:
    quint32p() {}
    quint32 data;
} Q_PACKED;

class qabgr8888
{
public:
    inline qabgr8888(quint32 v)
    {
        data = qRgba(qBlue(v), qGreen(v), qRed(v), qAlpha(v));
    }

    inline bool operator==(const qabgr8888 &v) const { return data == v.data; }

private:
    quint32 data;
} Q_PACKED;

class qrgb565;

class qargb8565
{
public:
    static inline bool hasAlpha() { return true; }

    inline qargb8565() {}
    inline qargb8565(quint32 v);
    inline explicit qargb8565(quint32p v);
    inline qargb8565(const qargb8565 &v);
    inline qargb8565(const qrgb565 &v);

    inline operator quint32() const;
    inline operator quint16() const;

    inline quint8 alpha() const { return data[0]; }
    inline qargb8565 truncedAlpha() {
        data[0] &= 0xf8;
        data[1] &= 0xdf;
        return *this;
    }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 3; }
    static inline quint8 ialpha(quint8 a) { return 0x20 - alpha(a); }

    inline qargb8565 byte_mul(quint8 a) const;
    inline qargb8565 operator+(qargb8565 v) const;
    inline bool operator==(const qargb8565 &v) const;

    inline quint32 rawValue() const;

private:
    friend class qrgb565;

    quint8 data[3];
} Q_PACKED;

class qrgb565
{
public:
    static inline bool hasAlpha() { return false; }

    qrgb565(int v = 0) : data(v) {}

    inline explicit qrgb565(quint32p v);
    inline explicit qrgb565(quint32 v);
    inline explicit qrgb565(qargb8565 v);

    inline operator quint32() const;
    inline operator quint16() const;

    inline qrgb565 operator+(qrgb565 v) const;

    inline quint8 alpha() const { return 0xff; }
    inline qrgb565 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 3; }
    static inline quint8 ialpha(quint8 a) { return 0x20 - alpha(a); }

    inline qrgb565 byte_mul(quint8 a) const;

    inline bool operator==(const qrgb565 &v) const;
    inline quint16 rawValue() const { return data; }

private:
    friend class qargb8565;

    quint16 data;
} Q_PACKED;

qargb8565::qargb8565(quint32 v)
{
    *this = qargb8565(quint32p(v));
}

qargb8565::qargb8565(quint32p v)
{
    data[0] = qAlpha(v);
    const int r = qRed(v);
    const int g = qGreen(v);
    const int b = qBlue(v);
    data[1] = ((g << 3) & 0xe0) | (b >> 3);
    data[2] = (r & 0xf8) | (g >> 5);
}

qargb8565::qargb8565(const qargb8565 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
}

qargb8565::qargb8565(const qrgb565 &v)
{
    data[0] = 0xff;
    data[1] = v.data & 0xff;
    data[2] = v.data >> 8;
}

qargb8565::operator quint32() const
{
    const quint16 rgb = (data[2] << 8) | data[1];
    const int r = (rgb & 0xf800);
    const int g = (rgb & 0x07e0);
    const int b = (rgb & 0x001f);
    const int tr = (r >> 8) | (r >> 13);
    const int tg = (g >> 3) | (g >> 9);
    const int tb = (b << 3) | (b >> 2);
    return qRgba(tr, tg, tb, data[0]);
}

qargb8565::operator quint16() const
{
    return (data[2] << 8) | data[1];
}

qargb8565 qargb8565::operator+(qargb8565 v) const
{
    qargb8565 t;
    t.data[0] = data[0] + v.data[0];
    const quint16 rgb =  ((data[2] + v.data[2]) << 8)
                         + (data[1] + v.data[1]);
    t.data[1] = rgb & 0xff;
    t.data[2] = rgb >> 8;
    return t;
}

qargb8565 qargb8565::byte_mul(quint8 a) const
{
    qargb8565 result;
    result.data[0] = (data[0] * a) >> 5;

    const quint16 x = (data[2] << 8) | data[1];
    const quint16 t = ((((x & 0x07e0) >> 5) * a) & 0x07e0) |
                      ((((x & 0xf81f) * a) >> 5) & 0xf81f);
    result.data[1] = t & 0xff;
    result.data[2] = t >> 8;
    return result;
}

bool qargb8565::operator==(const qargb8565 &v) const
{
    return data[0] == v.data[0]
        && data[1] == v.data[1]
        && data[2] == v.data[2];
}

quint32 qargb8565::rawValue() const
{
    return (data[2] << 16) | (data[1] << 8) | data[0];
}

qrgb565::qrgb565(quint32p v)
{
    *this = qrgb565(quint32(v));
}

qrgb565::qrgb565(quint32 v)
{
    const int r = qRed(v) << 8;
    const int g = qGreen(v) << 3;
    const int b = qBlue(v) >> 3;

    data = (r & 0xf800) | (g & 0x07e0)| (b & 0x001f);
}

qrgb565::qrgb565(qargb8565 v)
{
    data = (v.data[2] << 8) | v.data[1];
}

qrgb565::operator quint32() const
{
    const int r = (data & 0xf800);
    const int g = (data & 0x07e0);
    const int b = (data & 0x001f);
    const int tr = (r >> 8) | (r >> 13);
    const int tg = (g >> 3) | (g >> 9);
    const int tb = (b << 3) | (b >> 2);
    return qRgb(tr, tg, tb);
}

qrgb565::operator quint16() const
{
    return data;
}

qrgb565 qrgb565::operator+(qrgb565 v) const
{
    qrgb565 t;
    t.data = data + v.data;
    return t;
}

qrgb565 qrgb565::byte_mul(quint8 a) const
{
    qrgb565 result;
    result.data = ((((data & 0x07e0) >> 5) * a) & 0x07e0) |
                  ((((data & 0xf81f) * a) >> 5) & 0xf81f);
    return result;
}

bool qrgb565::operator==(const qrgb565 &v) const
{
    return data == v.data;
}

class qbgr565
{
public:
    inline qbgr565(quint16 v)
    {
        data = ((v & 0x001f) << 11) |
               (v & 0x07e0) |
               ((v & 0xf800) >> 11);
    }

    inline bool operator==(const qbgr565 &v) const
    {
        return data == v.data;
    }

private:
    quint16 data;
} Q_PACKED;

class qrgb555;

class qargb8555
{
public:
    static inline bool hasAlpha() { return true; }

    qargb8555() {}
    inline qargb8555(quint32 v);
    inline explicit qargb8555(quint32p v);
    inline qargb8555(const qargb8555 &v);
    inline qargb8555(const qrgb555 &v);

    inline operator quint32() const;

    inline quint8 alpha() const { return data[0]; }
    inline qargb8555 truncedAlpha() { data[0] &= 0xf8; return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 3; }
    static inline quint8 ialpha(quint8 a) { return 0x20 - alpha(a); }

    inline qargb8555 operator+(qargb8555 v) const;
    inline qargb8555 byte_mul(quint8 a) const;

    inline bool operator==(const qargb8555 &v) const;

    inline quint32 rawValue() const;

private:
    friend class qrgb555;
    quint8 data[3];
} Q_PACKED;

class qrgb555
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb555(int v = 0) : data(v) {}

    inline explicit qrgb555(quint32p v) { *this = qrgb555(quint32(v)); }

    inline explicit qrgb555(quint32 v)
    {
        const int r = qRed(v) << 7;
        const int g = qGreen(v) << 2;
        const int b = qBlue(v) >> 3;

        data = (r & 0x7c00) | (g & 0x03e0) | (b & 0x001f);
    }

    inline explicit qrgb555(quint16 v)
    {
        data = ((v >> 1) & (0x7c00 | 0x03e0)) |
               (v & 0x001f);
    }

    inline explicit qrgb555(const qargb8555 &v);

    inline operator quint32() const
    {
        const int r = (data & 0x7c00);
        const int g = (data & 0x03e0);
        const int b = (data & 0x001f);
        const int tr = (r >> 7) | (r >> 12);
        const int tg = (g >> 2) | (g >> 7);
        const int tb = (b << 3) | (b >> 2);

        return qRgb(tr, tg, tb);
    }

    inline operator quint16() const
    {
        const int r = ((data & 0x7c00) << 1) & 0xf800;
        const int g = (((data & 0x03e0) << 1) | ((data >> 4) & 0x0020)) & 0x07e0;
        const int b = (data & 0x001f);

        return r | g | b;
    }

    inline qrgb555 operator+(qrgb555 v) const;
    inline qrgb555 byte_mul(quint8 a) const;

    inline quint8 alpha() const { return 0xff; }
    inline qrgb555 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 3; }
    static inline quint8 ialpha(quint8 a) { return 0x20 - alpha(a); }

    inline bool operator==(const qrgb555 &v) const { return v.data == data; }
    inline bool operator!=(const qrgb555 &v) const { return v.data != data; }

    inline quint16 rawValue() const { return data; }

private:
    friend class qargb8555;
    friend class qbgr555;
    quint16 data;

} Q_PACKED;

qrgb555::qrgb555(const qargb8555 &v)
{
    data = (v.data[2] << 8) | v.data[1];
}

qrgb555 qrgb555::operator+(qrgb555 v) const
{
    qrgb555 t;
    t.data = data + v.data;
    return t;
}

qrgb555 qrgb555::byte_mul(quint8 a) const
{
    quint16 t = (((data & 0x3e0) * a) >> 5) & 0x03e0;
    t |= (((data & 0x7c1f) * a) >> 5) & 0x7c1f;

    qrgb555 result;
    result.data = t;
    return result;
}

class qbgr555
{
public:
    inline qbgr555(quint32 v) { *this = qbgr555(qrgb555(v)); }

    inline qbgr555(qrgb555 v)
    {
        data = ((v.data & 0x001f) << 10) |
               (v.data & 0x03e0) |
               ((v.data & 0x7c00) >> 10);
    }

    inline bool operator==(const qbgr555 &v) const
    {
        return data == v.data;
    }

private:
    quint16 data;
} Q_PACKED;

qargb8555::qargb8555(quint32 v)
{
    v = quint32p(v);
    data[0] = qAlpha(v);
    const int r = qRed(v);
    const int g = qGreen(v);
    const int b = qBlue(v);
    data[1] = ((g << 2) & 0xe0) | (b >> 3);
    data[2] = ((r >> 1) & 0x7c) | (g >> 6);

}

qargb8555::qargb8555(quint32p v)
{
    data[0] = qAlpha(v);
    const int r = qRed(v);
    const int g = qGreen(v);
    const int b = qBlue(v);
    data[1] = ((g << 2) & 0xe0) | (b >> 3);
    data[2] = ((r >> 1) & 0x7c) | (g >> 6);
}

qargb8555::qargb8555(const qargb8555 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
}

qargb8555::qargb8555(const qrgb555 &v)
{
    data[0] = 0xff;
    data[1] = v.data & 0xff;
    data[2] = v.data >> 8;
}

qargb8555::operator quint32() const
{
    const quint16 rgb = (data[2] << 8) | data[1];
    const int r = (rgb & 0x7c00);
    const int g = (rgb & 0x03e0);
    const int b = (rgb & 0x001f);
    const int tr = (r >> 7) | (r >> 12);
    const int tg = (g >> 2) | (g >> 7);
    const int tb = (b << 3) | (b >> 2);

    return qRgba(tr, tg, tb, data[0]);
}

bool qargb8555::operator==(const qargb8555 &v) const
{
    return data[0] == v.data[0]
        && data[1] == v.data[1]
        && data[2] == v.data[2];
}

quint32 qargb8555::rawValue() const
{
    return (data[2] << 16) | (data[1] << 8) | data[0];
}

qargb8555 qargb8555::operator+(qargb8555 v) const
{
    qargb8555 t;
    t.data[0] = data[0] + v.data[0];
    const quint16 rgb =  ((data[2] + v.data[2]) << 8)
                         + (data[1] + v.data[1]);
    t.data[1] = rgb & 0xff;
    t.data[2] = rgb >> 8;
    return t;
}

qargb8555 qargb8555::byte_mul(quint8 a) const
{
    qargb8555 result;
    result.data[0] = (data[0] * a) >> 5;

    const quint16 x = (data[2] << 8) | data[1];
    quint16 t = (((x & 0x3e0) * a) >> 5) & 0x03e0;
    t |= (((x & 0x7c1f) * a) >> 5) & 0x7c1f;
    result.data[1] = t & 0xff;
    result.data[2] = t >> 8;
    return result;

}

class qrgb666;

class qargb6666
{
public:
    static inline bool hasAlpha() { return true; }

    inline qargb6666() {}
    inline qargb6666(quint32 v) { *this = qargb6666(quint32p(v)); }
    inline explicit qargb6666(quint32p v);
    inline qargb6666(const qargb6666 &v);
    inline qargb6666(const qrgb666 &v);

    inline operator quint32 () const;

    inline quint8 alpha() const;
    inline qargb6666 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 2; }
    static inline quint8 ialpha(quint8 a) { return (255 - a + 1) >> 2; }

    inline qargb6666 byte_mul(quint8 a) const;
    inline qargb6666 operator+(qargb6666 v) const;
    inline bool operator==(const qargb6666 &v) const;

    inline quint32 rawValue() const;

private:
    friend class qrgb666;
    quint8 data[3];

} Q_PACKED;

class qrgb666
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb666() {}
    inline qrgb666(quint32 v);
    inline qrgb666(const qargb6666 &v);

    inline operator quint32 () const;

    inline quint8 alpha() const { return 0xff; }
    inline qrgb666 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 2; }
    static inline quint8 ialpha(quint8 a) { return (255 - a + 1) >> 2; }

    inline qrgb666 operator+(qrgb666 v) const;
    inline qrgb666 byte_mul(quint8 a) const;

    inline bool operator==(const qrgb666 &v) const;
    inline bool operator!=(const qrgb666 &v) const { return !(*this == v); }

    inline quint32 rawValue() const
    {
        return (data[2] << 16) | (data[1] << 8) | data[0];
    }

private:
    friend class qargb6666;

    quint8 data[3];
} Q_PACKED;

qrgb666::qrgb666(quint32 v)
{
    const uchar b = qBlue(v);
    const uchar g = qGreen(v);
    const uchar r = qRed(v);
    const uint p = (b >> 2) | ((g >> 2) << 6) | ((r >> 2) << 12);
    data[0] = qBlue(p);
    data[1] = qGreen(p);
    data[2] = qRed(p);
}

qrgb666::qrgb666(const qargb6666 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2] & 0x03;
}

qrgb666::operator quint32 () const
{
    const uchar r = (data[2] << 6) | ((data[1] & 0xf0) >> 2) | (data[2] & 0x3);
    const uchar g = (data[1] << 4) | ((data[0] & 0xc0) >> 4) | ((data[1] & 0x0f) >> 2);
    const uchar b = (data[0] << 2) | ((data[0] & 0x3f) >> 4);
    return qRgb(r, g, b);
}

qrgb666 qrgb666::operator+(qrgb666 v) const
{
    const quint32 x1 = (data[2] << 16) | (data[1] << 8) | data[0];
    const quint32 x2 = (v.data[2] << 16) | (v.data[1] << 8) | v.data[0];
    const quint32 t = x1 + x2;
    qrgb666 r;
    r.data[0] = t & 0xff;
    r.data[1] = (t >> 8) & 0xff;
    r.data[2] = (t >> 16) & 0xff;
    return r;
}

qrgb666 qrgb666::byte_mul(quint8 a) const
{
    const quint32 x = (data[2] << 16) | (data[1] << 8) | data[0];
    const quint32 t = ((((x & 0x03f03f) * a) >> 6) & 0x03f03f) |
                      ((((x & 0x000fc0) * a) >> 6) & 0x000fc0);

    qrgb666 r;
    r.data[0] = t & 0xff;
    r.data[1] = (t >> 8) & 0xff;
    r.data[2] = (t >> 16) & 0xff;
    return r;
}

bool qrgb666::operator==(const qrgb666 &v) const
{
    return (data[0] == v.data[0] &&
            data[1] == v.data[1] &&
            data[2] == v.data[2]);
}

qargb6666::qargb6666(quint32p v)
{
    const quint8 b = qBlue(v) >> 2;
    const quint8 g = qGreen(v) >> 2;
    const quint8 r = qRed(v) >> 2;
    const quint8 a = qAlpha(v) >> 2;
    const uint p = (a << 18) | (r << 12) | (g << 6) | b;
    data[0] = qBlue(p);
    data[1] = qGreen(p);
    data[2] = qRed(p);
}

qargb6666::qargb6666(const qargb6666 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
}

qargb6666::qargb6666(const qrgb666 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = (v.data[2] | 0xfc);
}

qargb6666::operator quint32 () const
{
    const quint8 r = (data[2] << 6) | ((data[1] & 0xf0) >> 2) | (data[2] & 0x3);
    const quint8 g = (data[1] << 4) | ((data[0] & 0xc0) >> 4) | ((data[1] & 0x0f) >> 2);
    const quint8 b = (data[0] << 2) | ((data[0] & 0x3f) >> 4);
    const quint8 a = (data[2] & 0xfc) | (data[2] >> 6);
    return qRgba(r, g, b, a);
}

qargb6666 qargb6666::operator+(qargb6666 v) const
{
    const quint32 x1 = (data[2] << 16) | (data[1] << 8) | data[0];
    const quint32 x2 = (v.data[2] << 16) | (v.data[1] << 8) | v.data[0];
    const quint32 t = x1 + x2;
    qargb6666 r;
    r.data[0] = t & 0xff;
    r.data[1] = (t >> 8) & 0xff;
    r.data[2] = (t >> 16) & 0xff;
    return r;
}

quint8 qargb6666::alpha() const
{
    return (data[2] & 0xfc) | (data[2] >> 6);
}

inline qargb6666 qargb6666::byte_mul(quint8 a) const
{
    const quint32 x = (data[2] << 16) | (data[1] << 8) | data[0];
    const quint32 t = ((((x & 0x03f03f) * a) >> 6) & 0x03f03f) |
                      ((((x & 0xfc0fc0) * a) >> 6) & 0xfc0fc0);

    qargb6666 r;
    r.data[0] = t & 0xff;
    r.data[1] = (t >> 8) & 0xff;
    r.data[2] = (t >> 16) & 0xff;
    return r;
}

bool qargb6666::operator==(const qargb6666 &v) const
{
    return data[0] == v.data[0]
        && data[1] == v.data[1]
        && data[2] == v.data[2];
}

quint32 qargb6666::rawValue() const
{
    return (data[2] << 16) | (data[1] << 8) | data[0];
}

class qrgb888
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb888() {}
    inline qrgb888(quint32 v);

    inline operator quint32() const;

    inline quint8 alpha() const { return 0xff; }
    inline qrgb888 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return a; }
    static inline quint8 ialpha(quint8 a) { return 255 - a; }

    inline qrgb888 byte_mul(quint8 a) const;
    inline qrgb888 operator+(qrgb888 v) const;
    inline bool operator==(qrgb888 v) const;

    inline quint32 rawValue() const;

private:
    uchar data[3];

} Q_PACKED;

qrgb888::qrgb888(quint32 v)
{
    data[0] = qRed(v);
    data[1] = qGreen(v);
    data[2] = qBlue(v);
}

qrgb888::operator quint32() const
{
    return qRgb(data[0], data[1], data[2]);
}

qrgb888 qrgb888::operator+(qrgb888 v) const
{
    qrgb888 t = *this;
    t.data[0] += v.data[0];
    t.data[1] += v.data[1];
    t.data[2] += v.data[2];
    return t;
}

qrgb888 qrgb888::byte_mul(quint8 a) const
{
    quint32 x(*this);

    quint32 t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return qrgb888(x);
}

bool qrgb888::operator==(qrgb888 v) const
{
    return (data[0] == v.data[0] &&
            data[1] == v.data[1] &&
            data[2] == v.data[2]);
}

quint32 qrgb888::rawValue() const
{
    return (data[2] << 16) | (data[1] << 8) | data[0];
}

template <>
inline qrgb888 qt_colorConvert(quint32 color, qrgb888 dummy)
{
    Q_UNUSED(dummy);
    return qrgb888(color);
}

template <>
inline quint32 qt_colorConvert(qrgb888 color, quint32 dummy)
{
    Q_UNUSED(dummy);
    return quint32(color);
}

#ifdef QT_QWS_DEPTH_8
template <>
inline quint8 qt_colorConvert(quint32 color, quint8 dummy)
{
    Q_UNUSED(dummy);

    uchar r = ((qRed(color) & 0xf8) + 0x19) / 0x33;
    uchar g = ((qGreen(color) &0xf8) + 0x19) / 0x33;
    uchar b = ((qBlue(color) &0xf8) + 0x19) / 0x33;

    return r*6*6 + g*6 + b;
}

template <>
inline quint8 qt_colorConvert(quint16 color, quint8 dummy)
{
    Q_UNUSED(dummy);

    uchar r = (color & 0xf800) >> (11-3);
    uchar g = (color & 0x07c0) >> (6-3);
    uchar b = (color & 0x001f) << 3;

    uchar tr = (r + 0x19) / 0x33;
    uchar tg = (g + 0x19) / 0x33;
    uchar tb = (b + 0x19) / 0x33;

    return tr*6*6 + tg*6 + tb;
}

#endif // QT_QWS_DEPTH_8

// hw: endianess??
class quint24
{
public:
    inline quint24(quint32 v)
    {
        data[0] = qBlue(v);
        data[1] = qGreen(v);
        data[2] = qRed(v);
    }

    inline operator quint32 ()
    {
        return qRgb(data[2], data[1], data[0]);
    }

    inline bool operator==(const quint24 &v) const
    {
        return data[0] == v.data[0]
            && data[1] == v.data[1]
            && data[2] == v.data[2];
    }

private:
    uchar data[3];
} Q_PACKED;

template <>
inline quint24 qt_colorConvert(quint32 color, quint24 dummy)
{
    Q_UNUSED(dummy);
    return quint24(color);
}

// hw: endianess??
class quint18
{
public:
    inline quint18(quint32 v)
    {
        uchar b = qBlue(v);
        uchar g = qGreen(v);
        uchar r = qRed(v);
        uint p = (b >> 2) | ((g >> 2) << 6) | ((r >> 2) << 12);
        data[0] = qBlue(p);
        data[1] = qGreen(p);
        data[2] = qRed(p);
    }

    inline operator quint32 ()
    {
        const uchar r = (data[2] << 6) | ((data[1] & 0xf0) >> 2) | (data[2] & 0x3);
        const uchar g = (data[1] << 4) | ((data[0] & 0xc0) >> 4) | ((data[1] & 0x0f) >> 2);
        const uchar b = (data[0] << 2) | ((data[0] & 0x3f) >> 4);
        return qRgb(r, g, b);
    }

private:
    uchar data[3];
} Q_PACKED;

template <>
inline quint18 qt_colorConvert(quint32 color, quint18 dummy)
{
    Q_UNUSED(dummy);
    return quint18(color);
}

class qrgb444;

class qargb4444
{
public:
    static inline bool hasAlpha() { return true; }

    inline qargb4444() {}
    inline qargb4444(quint32 v) { *this = qargb4444(quint32p(v)); }
    inline explicit qargb4444(quint32p v);
    inline qargb4444(const qrgb444 &v);

    inline operator quint32() const;
    inline operator quint8() const;

    inline qargb4444 operator+(qargb4444 v) const;

    inline quint8 alpha() const { return ((data & 0xf000) >> 8) | ((data & 0xf000) >> 12); }
    inline qargb4444 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 4; }
    static inline quint8 ialpha(quint8 a) { return 0x10 - alpha(a); }
    inline qargb4444 byte_mul(quint8 a) const;

    inline bool operator==(const qargb4444 &v) const { return data == v.data; }

    inline quint16 rawValue() const { return data; }

private:
    friend class qrgb444;
    quint16 data;

} Q_PACKED;

class qrgb444
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb444() {}
    inline qrgb444(quint32 v);
    inline explicit qrgb444(qargb4444 v);

    inline operator quint32() const;
    inline operator quint8() const;

    inline qrgb444 operator+(qrgb444 v) const;
    inline quint8 alpha() const { return 0xff; }
    inline qrgb444 truncedAlpha() { return *this; }
    static inline quint8 alpha(quint8 a) { return (a + 1) >> 4; }
    static inline quint8 ialpha(quint8 a) { return 0x10 - alpha(a); }
    inline qrgb444 byte_mul(quint8 a) const;

    inline bool operator==(const qrgb444 &v) const { return data == v.data; }
    inline bool operator!=(const qrgb444 &v) const { return data != v.data; }

    inline quint16 rawValue() const { return data; }

private:
    friend class qargb4444;
    quint16 data;

} Q_PACKED;


qargb4444::qargb4444(quint32p color)
{
    quint32 v = color;
    v &= 0xf0f0f0f0;
    const int a = qAlpha(v) << 8;
    const int r = qRed(v) << 4;
    const int g = qGreen(v);
    const int b = qBlue(v) >> 4;

    data = a | r | g | b;
}

qargb4444::qargb4444(const qrgb444 &v)
{
    data = v.data | 0xf000;
}

qargb4444::operator quint32() const
{
    const int a = (data & 0xf000);
    const int r = (data & 0x0f00);
    const int g = (data & 0x00f0);
    const int b = (data & 0x000f);
    const int ta = (a >> 8) | (a >> 12);
    const int tr = (r >> 4) | (r >> 8);
    const int tg = g | (g >> 4);
    const int tb = (b << 4) | b;

    return qRgba(tr, tg, tb, ta);
}

qargb4444::operator quint8() const
{
    // hw: optimize!
    return qt_colorConvert<quint8, quint32>(operator quint32(), 0);
}

qargb4444 qargb4444::operator+(qargb4444 v) const
{
    qargb4444 t;
    t.data = data + v.data;
    return t;
}

qargb4444 qargb4444::byte_mul(quint8 a) const
{
    quint16 t = (((data & 0xf0f0) * a) >> 4) & 0xf0f0;
    t |= (((data & 0x0f0f) * a) >> 4) & 0x0f0f;

    qargb4444 result;
    result.data = t;
    return result;
}

qrgb444::qrgb444(quint32 v)
{
    v &= 0xf0f0f0f0;
    const int r = qRed(v) << 4;
    const int g = qGreen(v);
    const int b = qBlue(v) >> 4;

    data = r | g | b;
}

qrgb444::qrgb444(qargb4444 v)
{
    data = v.data & 0x0fff;
}

qrgb444::operator quint32() const
{
    const int r = (data & 0x0f00);
    const int g = (data & 0x00f0);
    const int b = (data & 0x000f);
    const int tr = (r >> 4) | (r >> 8);
    const int tg = g | (g >> 4);
    const int tb = (b << 4) | b;

    return qRgb(tr, tg, tb);
}

qrgb444::operator quint8() const
{
    // hw: optimize!
    return qt_colorConvert<quint8, quint32>(operator quint32(), 0);
}

qrgb444 qrgb444::operator+(qrgb444 v) const
{
    qrgb444 t;
    t.data = data + v.data;
    return t;
}

qrgb444 qrgb444::byte_mul(quint8 a) const
{
    quint16 t = (((data & 0xf0f0) * a) >> 4) & 0xf0f0;
    t |= (((data & 0x0f0f) * a) >> 4) & 0x0f0f;

    qrgb444 result;
    result.data = t;
    return result;
}

#ifdef QT_QWS_DEPTH_GENERIC

struct qrgb
{
public:
    static int bpp;
    static int len_red;
    static int len_green;
    static int len_blue;
    static int len_alpha;
    static int off_red;
    static int off_green;
    static int off_blue;
    static int off_alpha;
} Q_PACKED;

template <typename SRC>
static inline quint32 qt_convertToRgb(SRC color);

template <>
static inline quint32 qt_convertToRgb(quint32 color)
{
    const int r = qRed(color) >> (8 - qrgb::len_red);
    const int g = qGreen(color) >> (8 - qrgb::len_green);
    const int b = qBlue(color) >> (8 - qrgb::len_blue);
    const int a = qAlpha(color) >> (8 - qrgb::len_alpha);
    const quint32 v = (r << qrgb::off_red)
                      | (g << qrgb::off_green)
                      | (b << qrgb::off_blue)
                      | (a << qrgb::off_alpha);

    return v;
}

template <>
static inline quint32 qt_convertToRgb(quint16 color)
{
    return qt_convertToRgb(qt_colorConvert<quint32, quint16>(color, 0));
}

class qrgb_generic16
{
public:
    inline qrgb_generic16(quint32 color)
    {
        const int r = qRed(color) >> (8 - qrgb::len_red);
        const int g = qGreen(color) >> (8 - qrgb::len_green);
        const int b = qBlue(color) >> (8 - qrgb::len_blue);
        const int a = qAlpha(color) >> (8 - qrgb::len_alpha);
        data = (r << qrgb::off_red)
               | (g << qrgb::off_green)
               | (b << qrgb::off_blue)
               | (a << qrgb::off_alpha);
    }

    inline operator quint16 () { return data; }
    inline quint32 operator<<(int shift) const { return data << shift; }

private:
    quint16 data;
} Q_PACKED;

template <>
static inline qrgb_generic16 qt_colorConvert(quint32 color, qrgb_generic16 dummy)
{
    Q_UNUSED(dummy);
    return qrgb_generic16(color);
}

template <>
static inline qrgb_generic16 qt_colorConvert(quint16 color, qrgb_generic16 dummy)
{
    Q_UNUSED(dummy);
    return qrgb_generic16(qt_colorConvert<quint32, quint16>(color, 0));
}

#endif // QT_QWS_DEPTH_GENERIC

template <class T>
void qt_memfill(T *dest, T value, int count);

template<> inline void qt_memfill(quint32 *dest, quint32 color, int count)
{
    extern void (*qt_memfill32)(quint32 *dest, quint32 value, int count);
    qt_memfill32(dest, color, count);
}

template<> inline void qt_memfill(quint16 *dest, quint16 color, int count)
{
    extern void (*qt_memfill16)(quint16 *dest, quint16 value, int count);
    qt_memfill16(dest, color, count);
}

template<> inline void qt_memfill(quint8 *dest, quint8 color, int count)
{
    memset(dest, color, count);
}

template <class T>
inline void qt_memfill(T *dest, T value, int count)
{
    int n = (count + 7) / 8;
    switch (count & 0x07)
    {
    case 0: do { *dest++ = value;
    case 7:      *dest++ = value;
    case 6:      *dest++ = value;
    case 5:      *dest++ = value;
    case 4:      *dest++ = value;
    case 3:      *dest++ = value;
    case 2:      *dest++ = value;
    case 1:      *dest++ = value;
    } while (--n > 0);
    }
}

template <class T>
inline void qt_rectfill(T *dest, T value,
                        int x, int y, int width, int height, int stride)
{
    char *d = reinterpret_cast<char*>(dest + x) + y * stride;
    if (uint(stride) == (width * sizeof(T))) {
        qt_memfill(reinterpret_cast<T*>(d), value, width * height);
    } else {
        for (int j = 0; j < height; ++j) {
            dest = reinterpret_cast<T*>(d);
            qt_memfill(dest, value, width);
            d += stride;
        }
    }
}

template <class DST, class SRC>
inline void qt_memconvert(DST *dest, const SRC *src, int count)
{
    if (sizeof(DST) == 1) {
        while (count) {
            int n = 1;
            const SRC color = *src++;
            const DST dstColor = qt_colorConvert<DST, SRC>(color, 0);
            while (--count && (*src == color || dstColor == qt_colorConvert<DST, SRC>(*src, 0))) {
                ++n;
                ++src;
            }
            qt_memfill(dest, dstColor, n);
            dest += n;
        }
    } else {
        /* Duff's device */
        int n = (count + 7) / 8;
        switch (count & 0x07)
        {
        case 0: do { *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 7:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 6:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 5:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 4:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 3:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 2:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 1:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            } while (--n > 0);
        }
    }
}

#define QT_TRIVIAL_MEMCONVERT_IMPL(T) \
    template <> \
    inline void qt_memconvert(T *dest, const T *src, int count) \
    { \
        memcpy(dest, src, count * sizeof(T)); \
    }
QT_TRIVIAL_MEMCONVERT_IMPL(quint32)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb888)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb6666)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb666)
QT_TRIVIAL_MEMCONVERT_IMPL(quint16)
#ifdef Q_WS_QWS
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb565)
#endif
QT_TRIVIAL_MEMCONVERT_IMPL(qargb8565)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb8555)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb555)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb4444)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb444)
#undef QT_TRIVIAL_MEMCONVERT_IMPL

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
template <>
inline void qt_memconvert(qrgb666 *dest, const quint32 *src, int count)
{
    if (count < 3) {
        switch (count) {
        case 2: *dest++ = qrgb666(*src++);
        case 1: *dest = qrgb666(*src);
        }
        return;
    }

    const int align = (long(dest) & 3);
    switch (align) {
    case 1: *dest++ = qrgb666(*src++); --count;
    case 2: *dest++ = qrgb666(*src++); --count;
    case 3: *dest++ = qrgb666(*src++); --count;
    }

    quint32 *dest32 = reinterpret_cast<quint32*>(dest);
    int sourceCount = count >> 2;
    while (sourceCount--) {
        dest32[0] = ((src[1] & 0x00000c00) << 20)
                    | ((src[1] & 0x000000fc) << 22)
                    | ((src[0] & 0x00fc0000) >> 6)
                    | ((src[0] & 0x0000fc00) >> 4)
                    |  ((src[0] & 0x000000fc) >> 2);
        dest32[1] = ((src[2] & 0x003c0000) << 10)
                    | ((src[2] & 0x0000fc00) << 12)
                    | ((src[2] & 0x000000fc) << 14)
                    | ((src[1] & 0x00fc0000) >> 14)
                    | ((src[1] & 0x0000f000) >> 12);
        dest32[2] = ((src[3] & 0x00fc0000) << 2)
                    | ((src[3] & 0x0000fc00) << 4)
                    | ((src[3] & 0x000000fc) << 6)
                    | ((src[2] & 0x00c00000) >> 22);
        dest32 += 3;
        src += 4;
    }

    dest = reinterpret_cast<qrgb666*>(dest32);
    switch (count & 3) {
    case 3: *dest++ = qrgb666(*src++);
    case 2: *dest++ = qrgb666(*src++);
    case 1: *dest = qrgb666(*src);
    }
}
#endif // Q_BYTE_ORDER

template <class T>
inline void qt_rectcopy(T *dest, const T *src,
                        int x, int y, int width, int height,
                        int dstStride, int srcStride)
{
    char *d = (char*)(dest + x) + y * dstStride;
    const char *s = (char*)(src);
    for (int i = 0; i < height; ++i) {
        ::memcpy(d, s, width * sizeof(T));
        d += dstStride;
        s += srcStride;
    }
}

template <class DST, class SRC>
inline void qt_rectconvert(DST *dest, const SRC *src,
                           int x, int y, int width, int height,
                           int dstStride, int srcStride)
{
    char *d = (char*)(dest + x) + y * dstStride;
    const char *s = (char*)(src);
    for (int i = 0; i < height; ++i) {
        qt_memconvert<DST,SRC>((DST*)d, (const SRC*)s, width);
        d += dstStride;
        s += srcStride;
    }
}

#define QT_RECTCONVERT_TRIVIAL_IMPL(T)                                  \
    template <>                                                         \
    inline void qt_rectconvert(T *dest, const T *src,                   \
                               int x, int y, int width, int height,     \
                               int dstStride, int srcStride)            \
    {                                                                   \
        qt_rectcopy(dest, src, x, y, width, height, dstStride, srcStride); \
    }
QT_RECTCONVERT_TRIVIAL_IMPL(quint32)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb888)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb6666)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb666)
#ifdef Q_WS_QWS
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb565)
#endif
QT_RECTCONVERT_TRIVIAL_IMPL(qargb8565)
QT_RECTCONVERT_TRIVIAL_IMPL(quint16)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb8555)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb555)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb4444)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb444)
#undef QT_RECTCONVERT_TRIVIAL_IMPL

#ifdef QT_QWS_DEPTH_GENERIC
template <> void qt_rectconvert(qrgb *dest, const quint32 *src,
                                int x, int y, int width, int height,
                                int dstStride, int srcStride);

template <> void qt_rectconvert(qrgb *dest, const quint16 *src,
                                int x, int y, int width, int height,
                                int dstStride, int srcStride);
#endif // QT_QWS_DEPTH_GENERIC

#define QT_MEMFILL_UINT(dest, length, color)            \
    qt_memfill<quint32>(dest, color, length);

#define QT_MEMFILL_USHORT(dest, length, color) \
    qt_memfill<quint16>(dest, color, length);

#define QT_MEMCPY_REV_UINT(dest, src, length) \
do {                                          \
    /* Duff's device */                       \
    uint *_d = (uint*)(dest) + length;         \
    const uint *_s = (uint*)(src) + length;    \
    register int n = ((length) + 7) / 8;      \
    switch ((length) & 0x07)                  \
    {                                         \
    case 0: do { *--_d = *--_s;                 \
    case 7:      *--_d = *--_s;                 \
    case 6:      *--_d = *--_s;                 \
    case 5:      *--_d = *--_s;                 \
    case 4:      *--_d = *--_s;                 \
    case 3:      *--_d = *--_s;                 \
    case 2:      *--_d = *--_s;                 \
    case 1:      *--_d = *--_s;                 \
    } while (--n > 0);                        \
    }                                         \
} while (0)

#define QT_MEMCPY_USHORT(dest, src, length) \
do {                                          \
    /* Duff's device */                       \
    ushort *_d = (ushort*)(dest);         \
    const ushort *_s = (ushort*)(src);    \
    register int n = ((length) + 7) / 8;      \
    switch ((length) & 0x07)                  \
    {                                         \
    case 0: do { *_d++ = *_s++;                 \
    case 7:      *_d++ = *_s++;                 \
    case 6:      *_d++ = *_s++;                 \
    case 5:      *_d++ = *_s++;                 \
    case 4:      *_d++ = *_s++;                 \
    case 3:      *_d++ = *_s++;                 \
    case 2:      *_d++ = *_s++;                 \
    case 1:      *_d++ = *_s++;                 \
    } while (--n > 0);                        \
    }                                         \
} while (0)

static inline int qt_div_255(int x) { return (x + (x>>8) + 0x80) >> 8; }

inline ushort qConvertRgb32To16(uint c)
{
   return (((c) >> 3) & 0x001f)
       | (((c) >> 5) & 0x07e0)
       | (((c) >> 8) & 0xf800);
}

#if defined(Q_WS_QWS) || (QT_VERSION >= 0x040400)
inline quint32 qConvertRgb32To16x2(quint64 c)
{
    c = (((c) >> 3) & Q_UINT64_C(0x001f0000001f))
        | (((c) >> 5) & Q_UINT64_C(0x07e0000007e0))
        | (((c) >> 8) & Q_UINT64_C(0xf8000000f800));
    return c | (c >> 16);
}
#endif

inline QRgb qConvertRgb16To32(uint c)
{
    return 0xff000000
        | ((((c) << 3) & 0xf8) | (((c) >> 2) & 0x7))
        | ((((c) << 5) & 0xfc00) | (((c) >> 1) & 0x300))
        | ((((c) << 8) & 0xf80000) | (((c) << 3) & 0x70000));
}

inline int qRed565(quint16 rgb) {
    const int r = (rgb & 0xf800);
    return (r >> 8) | (r >> 13);
}

inline int qGreen565(quint16 rgb) {
    const int g = (rgb & 0x07e0);
    return (g >> 3) | (g >> 9);
}

inline int qBlue565(quint16 rgb) {
    const int b = (rgb & 0x001f);
    return (b << 3) | (b >> 2);
}

#if 1
static inline uint INTERPOLATE_PIXEL_256(uint x, uint a, uint y, uint b) {
    uint t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t >>= 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x &= 0xff00ff00;
    x |= t;
    return x;
}

static inline uint INTERPOLATE_PIXEL_255(uint x, uint a, uint y, uint b) {
    uint t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return x;
}
#else
// possible implementation for 64 bit
static inline uint INTERPOLATE_PIXEL_256(uint x, uint a, uint y, uint b) {
    ulong t = (((ulong(x)) | ((ulong(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t += (((ulong(y)) | ((ulong(y)) << 24)) & 0x00ff00ff00ff00ff) * b;
    t >>= 8;
    t &= 0x00ff00ff00ff00ff;
    return (uint(t)) | (uint(t >> 24));
}

static inline uint INTERPOLATE_PIXEL_255(uint x, uint a, uint y, uint b) {
    ulong t = (((ulong(x)) | ((ulong(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t += (((ulong(y)) | ((ulong(y)) << 24)) & 0x00ff00ff00ff00ff) * b;
    t = (t + ((t >> 8) & 0xff00ff00ff00ff) + 0x80008000800080);
    t &= 0x00ff00ff00ff00ff;
    return (uint(t)) | (uint(t >> 24));
}

static inline uint BYTE_MUL(uint x, uint a) {
    ulong t = (((ulong(x)) | ((ulong(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff00ff00ff) + 0x80008000800080);
    t &= 0x00ff00ff00ff00ff;
    return (uint(t)) | (uint(t >> 24));
}

static inline uint PREMUL(uint x) {
    uint a = x >> 24;
    ulong t = (((ulong(x)) | ((ulong(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff00ff00ff) + 0x80008000800080);
    t &= 0x00ff00ff00ff00ff;
    return (uint(t)) | (uint(t >> 24)) | 0xff000000;
}
#endif

const uint qt_bayer_matrix[16][16] = {
    { 0x1, 0xc0, 0x30, 0xf0, 0xc, 0xcc, 0x3c, 0xfc,
      0x3, 0xc3, 0x33, 0xf3, 0xf, 0xcf, 0x3f, 0xff},
    { 0x80, 0x40, 0xb0, 0x70, 0x8c, 0x4c, 0xbc, 0x7c,
      0x83, 0x43, 0xb3, 0x73, 0x8f, 0x4f, 0xbf, 0x7f},
    { 0x20, 0xe0, 0x10, 0xd0, 0x2c, 0xec, 0x1c, 0xdc,
      0x23, 0xe3, 0x13, 0xd3, 0x2f, 0xef, 0x1f, 0xdf},
    { 0xa0, 0x60, 0x90, 0x50, 0xac, 0x6c, 0x9c, 0x5c,
      0xa3, 0x63, 0x93, 0x53, 0xaf, 0x6f, 0x9f, 0x5f},
    { 0x8, 0xc8, 0x38, 0xf8, 0x4, 0xc4, 0x34, 0xf4,
      0xb, 0xcb, 0x3b, 0xfb, 0x7, 0xc7, 0x37, 0xf7},
    { 0x88, 0x48, 0xb8, 0x78, 0x84, 0x44, 0xb4, 0x74,
      0x8b, 0x4b, 0xbb, 0x7b, 0x87, 0x47, 0xb7, 0x77},
    { 0x28, 0xe8, 0x18, 0xd8, 0x24, 0xe4, 0x14, 0xd4,
      0x2b, 0xeb, 0x1b, 0xdb, 0x27, 0xe7, 0x17, 0xd7},
    { 0xa8, 0x68, 0x98, 0x58, 0xa4, 0x64, 0x94, 0x54,
      0xab, 0x6b, 0x9b, 0x5b, 0xa7, 0x67, 0x97, 0x57},
    { 0x2, 0xc2, 0x32, 0xf2, 0xe, 0xce, 0x3e, 0xfe,
      0x1, 0xc1, 0x31, 0xf1, 0xd, 0xcd, 0x3d, 0xfd},
    { 0x82, 0x42, 0xb2, 0x72, 0x8e, 0x4e, 0xbe, 0x7e,
      0x81, 0x41, 0xb1, 0x71, 0x8d, 0x4d, 0xbd, 0x7d},
    { 0x22, 0xe2, 0x12, 0xd2, 0x2e, 0xee, 0x1e, 0xde,
      0x21, 0xe1, 0x11, 0xd1, 0x2d, 0xed, 0x1d, 0xdd},
    { 0xa2, 0x62, 0x92, 0x52, 0xae, 0x6e, 0x9e, 0x5e,
      0xa1, 0x61, 0x91, 0x51, 0xad, 0x6d, 0x9d, 0x5d},
    { 0xa, 0xca, 0x3a, 0xfa, 0x6, 0xc6, 0x36, 0xf6,
      0x9, 0xc9, 0x39, 0xf9, 0x5, 0xc5, 0x35, 0xf5},
    { 0x8a, 0x4a, 0xba, 0x7a, 0x86, 0x46, 0xb6, 0x76,
      0x89, 0x49, 0xb9, 0x79, 0x85, 0x45, 0xb5, 0x75},
    { 0x2a, 0xea, 0x1a, 0xda, 0x26, 0xe6, 0x16, 0xd6,
      0x29, 0xe9, 0x19, 0xd9, 0x25, 0xe5, 0x15, 0xd5},
    { 0xaa, 0x6a, 0x9a, 0x5a, 0xa6, 0x66, 0x96, 0x56,
      0xa9, 0x69, 0x99, 0x59, 0xa5, 0x65, 0x95, 0x55}
};

#define ARGB_COMBINE_ALPHA(argb, alpha) \
    ((((argb >> 24) * alpha) >> 8) << 24) | (argb & 0x00ffffff)

QT_END_NAMESPACE

#endif // QDRAWHELPER_P_H
