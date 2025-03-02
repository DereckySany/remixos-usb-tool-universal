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

#ifndef QPAINTENGINE_RASTER_P_H
#define QPAINTENGINE_RASTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qpaintengineex_p.h"
#include "QtGui/qpainterpath.h"
#include "private/qdatabuffer_p.h"
#include "private/qdrawhelper_p.h"
#include "private/qpaintengine_p.h"
#include "private/qrasterizer_p.h"
#include "private/qstroker_p.h"
#include "private/qpainter_p.h"
#include "private/qtextureglyphcache_p.h"

#include <stdlib.h>

QT_BEGIN_NAMESPACE

class QOutlineMapper;
class QRasterPaintEnginePrivate;
class QRasterBuffer;
class QClipData;
class QCustomRasterPaintDevice;

class QRasterPaintEngineState : public QPainterState
{
public:
    QRasterPaintEngineState(QRasterPaintEngineState &other);
    QRasterPaintEngineState();
    ~QRasterPaintEngineState();


    QPen lastPen;
    QSpanData penData;
    QStrokerOps *stroker;
    uint strokeFlags;

    QBrush lastBrush;
    QSpanData brushData;
    uint fillFlags;

    uint pixmapFlags;
    int intOpacity;

    qreal txscale;

    QClipData *clip;
//     QRect clipRect;
//     QRegion clipRegion;

//     QPainter::RenderHints hints;
//     QPainter::CompositionMode compositionMode;

    uint dirty;

    struct Flags {
        uint has_clip_ownership : 1;        // should delete the clip member..
        uint fast_pen : 1;                  // cosmetic 1-width pens, using midpoint drawlines
        uint non_complex_pen : 1;           // can use rasterizer, rather than stroker
        uint antialiased : 1;
        uint bilinear : 1;
        uint fast_text : 1;
        uint int_xform : 1;
        uint tx_noshear : 1;
        uint fast_images : 1;
    };

    union {
        Flags flags;
        uint flag_bits;
    };
};




/*******************************************************************************
 * QRasterPaintEngine
 */
class
#ifdef Q_WS_QWS
Q_GUI_EXPORT
#endif
QRasterPaintEngine : public QPaintEngineEx
{
    Q_DECLARE_PRIVATE(QRasterPaintEngine)
public:

    QRasterPaintEngine(QPaintDevice *device);
    ~QRasterPaintEngine();
    bool begin(QPaintDevice *device);
    bool end();

    void penChanged();
    void brushChanged();
    void brushOriginChanged();
    void opacityChanged();
    void compositionModeChanged();
    void renderHintsChanged();
    void transformChanged();
    void clipEnabledChanged();

    void setState(QPainterState *s);
    QPainterState *createState(QPainterState *orig) const;
    inline QRasterPaintEngineState *state() {
        return static_cast<QRasterPaintEngineState *>(QPaintEngineEx::state());
    }
    inline const QRasterPaintEngineState *state() const {
        return static_cast<const QRasterPaintEngineState *>(QPaintEngineEx::state());
    }

    void updateBrush(const QBrush &brush);
    void updatePen(const QPen &pen);

    void updateMatrix(const QTransform &matrix);

    void drawPath(const QPainterPath &path);
    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);
    void fillPath(const QPainterPath &path, QSpanData *fillData);
    void fillPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);

    void drawEllipse(const QRectF &rect);

    void fillRect(const QRectF &rect, const QBrush &brush);
    void fillRect(const QRectF &rect, const QColor &color);

    void drawRects(const QRect  *rects, int rectCount);
    void drawRects(const QRectF *rects, int rectCount);

    void drawPixmap(const QPointF &p, const QPixmap &pm);
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawImage(const QPointF &p, const QImage &img);
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                   Qt::ImageConversionFlags falgs = Qt::AutoColor);
    void drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &sr);
    void drawTextItem(const QPointF &p, const QTextItem &textItem);

    void drawLines(const QLine *line, int lineCount);
    void drawLines(const QLineF *line, int lineCount);

    void drawPoints(const QPointF *points, int pointCount);
    void drawPoints(const QPoint *points, int pointCount);

    void stroke(const QVectorPath &path, const QPen &pen);
    void fill(const QVectorPath &path, const QBrush &brush);

    void strokePolygonCosmetic(const QPoint *pts, int pointCount, PolygonDrawMode mode);
    void strokePolygonCosmetic(const QPointF *pt, int pointCount, PolygonDrawMode mode);

    void clip(const QVectorPath &path, Qt::ClipOperation op);
    void clip(const QRect &rect, Qt::ClipOperation op);
    void clip(const QRegion &region, Qt::ClipOperation op);
    void clip(const QPainterPath &path, Qt::ClipOperation op);

    enum ClipType {
        RectClip,
        ComplexClip
    };
    ClipType clipType() const;
    QRect clipBoundingRect() const;

#ifdef Q_NO_USING_KEYWORD
    inline void drawEllipse(const QRect &rect) { QPaintEngineEx::drawEllipse(rect); }
#else
    using QPaintEngineEx::drawPolygon;
    using QPaintEngineEx::drawEllipse;
#endif

    void releaseBuffer();

    QSize size() const;

#ifndef QT_NO_DEBUG
    void saveBuffer(const QString &s) const;
#endif

#ifdef Q_WS_MAC
    void setCGContext(CGContextRef ref);
    CGContextRef getCGContext() const;
#endif

#ifdef Q_WS_WIN
    void setDC(HDC hdc);
    HDC getDC() const;
    void releaseDC(HDC hdc) const;
#endif

    void alphaPenBlt(const void* src, int bpl, int depth, int rx,int ry,int w,int h);

    Type type() const { return Raster; }

    QPoint coordinateOffset() const;

#if defined(Q_WS_QWS) && !defined(QT_NO_RASTERCALLBACKS)
    virtual void drawColorSpans(const QSpan *spans, int count, uint color);
    virtual void drawBufferSpan(const uint *buffer, int bufsize,
                                int x, int y, int length, uint const_alpha);
#endif

protected:
    QRasterPaintEngine(QRasterPaintEnginePrivate &d, QPaintDevice *);
private:
    friend struct QSpanData;
    void init();

    void fillRect(const QRectF &rect, QSpanData *data);
    void drawBitmap(const QPointF &pos, const QPixmap &image, QSpanData *fill);

    void drawCachedGlyphs(const QPointF &p, const QTextItemInt &ti);

    inline void ensureBrush(const QBrush &brush) {
        if (!qbrush_fast_equals(state()->lastBrush, brush) || state()->fillFlags)
            updateBrush(brush);
    }
    inline void ensureBrush() { ensureBrush(state()->brush); }

    inline void ensurePen(const QPen &pen) {
        if (!qpen_fast_equals(state()->lastPen, pen) || state()->strokeFlags)
            updatePen(pen);
    }
    inline void ensurePen() { ensurePen(state()->pen); }

    void updateOutlineMapper();
    inline void ensureOutlineMapper();

    void updateState();
    inline void ensureState() {
        if (state()->dirty)
            updateState();
    }
};


/*******************************************************************************
 * QRasterPaintEnginePrivate
 */
class
#ifdef Q_WS_QWS
Q_GUI_EXPORT
#endif
QRasterPaintEnginePrivate : public QPaintEngineExPrivate
{
    Q_DECLARE_PUBLIC(QRasterPaintEngine)
public:

    void rasterizeLine_dashed(QLineF line, qreal width,
                              int *dashIndex, qreal *dashOffset, bool *inDash);
    void rasterize(QT_FT_Outline *outline, ProcessSpans callback, QSpanData *spanData, QRasterBuffer *rasterBuffer);
    void rasterize(QT_FT_Outline *outline, ProcessSpans callback, void *userData, QRasterBuffer *rasterBuffer);
    void updateMatrixData(QSpanData *spanData, const QBrush &brush, const QTransform &brushMatrix);

    void systemStateChanged();

    void drawImage(const QPointF &pt, const QImage &img, SrcOverBlendFunc func,
                   const QRect &clip, int alpha, const QRect &sr = QRect());

    QTransform brushMatrix() const {
        Q_Q(const QRasterPaintEngine);
        const QRasterPaintEngineState *s = q->state();
        QTransform m(s->matrix);
        m.translate(s->brushOrigin.x(), s->brushOrigin.y());
        return m;
    }

    bool isUnclipped_normalized(const QRect &rect) const;
    bool isUnclipped(const QRect &rect, int penWidth) const;
    bool isUnclipped(const QRectF &rect, int penWidth) const;
    ProcessSpans getPenFunc(const QRect &rect, const QSpanData *data) const;
    ProcessSpans getPenFunc(const QRectF &rect, const QSpanData *data) const;
    ProcessSpans getBrushFunc(const QRect &rect, const QSpanData *data) const;
    ProcessSpans getBrushFunc(const QRectF &rect, const QSpanData *data) const;

#ifdef Q_WS_QWS
    void prepare(QCustomRasterPaintDevice *);
#endif

    inline const QClipData *clip() const;

    void strokeProjective(const QPainterPath &path);
    void initializeRasterizer(QSpanData *data);

    void recalculateFastImages();

    QPaintDevice *device;
    QOutlineMapper *outlineMapper;
    QRasterBuffer *rasterBuffer;

#if defined (Q_WS_WIN)
    HDC hdc;
#elif defined(Q_WS_MAC)
    CGContextRef cgContext;
#endif

    QRect deviceRect;

    QStroker basicStroker;
    QDashStroker *dashStroker;

    QT_FT_Raster *grayRaster;
    unsigned long rasterPoolSize;
    unsigned char *rasterPoolBase;

    QDataBuffer<QLineF> cachedLines;
    QSpanData image_filler;
    QSpanData image_filler_xform;
    QSpanData solid_color_filler;


    QFontEngineGlyphCache::Type glyphCacheType;

    QClipData *baseClip;

    int deviceDepth;

    uint mono_surface : 1;
    uint outlinemapper_xform_dirty : 1;

#ifdef Q_WS_WIN
    uint isPlain45DegreeRotation : 1;
#endif

    QRasterizer *rasterizer;
};


class QClipData {
public:
    QClipData(int height);
    ~QClipData();

    int clipSpanHeight;
    struct ClipLine {
        int count;
        QSpan *spans;
    } *m_clipLines;

    void initialize();

    inline ClipLine *clipLines() {
        if (!m_clipLines)
            initialize();
        return m_clipLines;
    }

    inline QSpan *spans() {
        if (!m_spans)
            initialize();
        return m_spans;
    }

    int allocated;
    int count;
    QSpan *m_spans;
    int xmin, xmax, ymin, ymax;

    QRect clipRect;
    QRegion clipRegion;

    uint enabled : 1;
    uint hasRectClip : 1;
    uint hasRegionClip : 1;

    void appendSpan(int x, int length, int y, int coverage);
    void appendSpans(const QSpan *s, int num);

    // ### Should optimize and actually kill the QSpans if the rect is
    // ### a subset of The current region. Thus the "fast" clipspan
    // ### callback can be used
    void setClipRect(const QRect &rect);
    void setClipRegion(const QRegion &region);
    void fixup();
};

inline void QClipData::appendSpan(int x, int length, int y, int coverage)
{
    Q_ASSERT(m_spans); // initialize() has to be called prior to adding spans..

    if (count == allocated) {
        allocated *= 2;
        m_spans = (QSpan *)realloc(m_spans, allocated*sizeof(QSpan));
    }
    m_spans[count].x = x;
    m_spans[count].len = length;
    m_spans[count].y = y;
    m_spans[count].coverage = coverage;
    ++count;
}

inline void QClipData::appendSpans(const QSpan *s, int num)
{
    Q_ASSERT(m_spans);

    if (count + num > allocated) {
        do {
            allocated *= 2;
        } while (count + num > allocated);
        m_spans = (QSpan *)realloc(m_spans, allocated*sizeof(QSpan));
    }
    memcpy(m_spans+count, s, num*sizeof(QSpan));
    count += num;
}

#ifdef Q_WS_QWS
class Q_GUI_EXPORT QCustomRasterPaintDevice : public QPaintDevice
{
public:
    QCustomRasterPaintDevice(QWidget *w) : widget(w) {}

    int devType() const { return QInternal::CustomRaster; }

    virtual int metric(PaintDeviceMetric m) const;

    virtual void* memory() const { return 0; }

    virtual QImage::Format format() const {
        return QImage::Format_ARGB32_Premultiplied;
    }

    virtual int bytesPerLine() const;

    virtual QSize size() const {
        return static_cast<QRasterPaintEngine*>(paintEngine())->size();
    }

private:
    QWidget *widget;
};
#endif // Q_WS_QWS

/*******************************************************************************
 * QRasterBuffer
 */
class QRasterBuffer
{
public:
#if defined(Q_WS_WIN)
    QRasterBuffer()
        : m_hdc(0),
          m_bitmap(0),
          m_null_bitmap(0),
          m_text_buffer(false),
          m_width(0),
          m_height(0),
          m_buffer(0)
    {
        init();
    }

    HDC hdc() const { return m_hdc; }
#else
    QRasterBuffer() : m_width(0), m_height(0), m_buffer(0) { init(); }
#endif

    ~QRasterBuffer();

    void init();

    QImage::Format prepare(QImage *image);
    QImage::Format prepare(QPixmap *pix);
#ifdef Q_WS_QWS
    void prepare(QCustomRasterPaintDevice *device);
#endif
    void prepare(int w, int h);
    void prepareBuffer(int w, int h);

#ifdef Q_WS_WIN
    void setupHDC(bool clear_type);
    inline void setTextBuffer(bool t);
#endif

    void resetBuffer(int val=0);

    uchar *scanLine(int y) { Q_ASSERT(y>=0); Q_ASSERT(y<m_height); return m_buffer + y * bytes_per_line; }

#ifndef QT_NO_DEBUG
    QImage bufferImage() const;
#endif

    void flushToARGBImage(QImage *image) const;

    int width() const { return m_width; }
    int height() const { return m_height; }
    int bytesPerLine() const { return bytes_per_line; }
    int bytesPerPixel() const { return bytes_per_pixel; }

    uchar *buffer() const { return m_buffer; }

    bool monoDestinationWithClut;
    QRgb destColor0;
    QRgb destColor1;

    QPainter::CompositionMode compositionMode;
    QImage::Format format;
    DrawHelper *drawHelper;
    QImage colorizeBitmap(const QImage &image, const QColor &color);

private:
#if defined(Q_WS_WIN)
    HDC m_hdc;
    HBITMAP m_bitmap;
    HBITMAP m_null_bitmap;
    bool m_text_buffer;
#endif
    int m_width;
    int m_height;
    int bytes_per_line;
    int bytes_per_pixel;
    uchar *m_buffer;
};

#ifdef Q_WS_WIN
inline void QRasterBuffer::setTextBuffer(bool t) { m_text_buffer = t; }
#endif

inline void QRasterPaintEngine::ensureOutlineMapper() {
    if (d_func()->outlinemapper_xform_dirty)
        updateOutlineMapper();
}

inline const QClipData *QRasterPaintEnginePrivate::clip() const {
    Q_Q(const QRasterPaintEngine);
    if (q->state() && q->state()->clip && q->state()->clip->enabled)
        return q->state()->clip;
    return baseClip;
}


QT_END_NAMESPACE
#endif // QPAINTENGINE_RASTER_P_H
