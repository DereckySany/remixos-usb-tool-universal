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

#ifndef QFONT_H
#define QFONT_H

#include <QtGui/qwindowdefs.h>
#include <QtCore/qstring.h>

#if defined(Q_WS_X11) || defined(Q_WS_QWS)
typedef struct FT_FaceRec_* FT_Face;
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QFontPrivate;                                     /* don't touch */
class QStringList;
class QVariant;
class Q3TextFormatCollection;

class Q_GUI_EXPORT QFont
{
    Q_GADGET
    Q_ENUMS(StyleStrategy)
public:
    enum StyleHint {
        Helvetica,  SansSerif = Helvetica,
        Times,      Serif = Times,
        Courier,    TypeWriter = Courier,
        OldEnglish, Decorative = OldEnglish,
        System,
        AnyStyle
    };

    enum StyleStrategy {
        PreferDefault    = 0x0001,
        PreferBitmap     = 0x0002,
        PreferDevice     = 0x0004,
        PreferOutline    = 0x0008,
        ForceOutline     = 0x0010,
        PreferMatch      = 0x0020,
        PreferQuality    = 0x0040,
        PreferAntialias  = 0x0080,
        NoAntialias      = 0x0100,
        OpenGLCompatible = 0x0200,
        NoFontMerging    = 0x8000
    };

    enum Weight {
        Light    = 25,
        Normal   = 50,
        DemiBold = 63,
        Bold     = 75,
        Black    = 87
    };

    enum Style {
        StyleNormal,
        StyleItalic,
        StyleOblique
    };

    enum Stretch {
        UltraCondensed =  50,
        ExtraCondensed =  62,
        Condensed      =  75,
        SemiCondensed  =  87,
        Unstretched    = 100,
        SemiExpanded   = 112,
        Expanded       = 125,
        ExtraExpanded  = 150,
        UltraExpanded  = 200
    };

    enum Capitalization {
        MixedCase,
        AllUppercase,
        AllLowercase,
        SmallCaps,
        Capitalize
    };

    enum SpacingType {
        PercentageSpacing,
        AbsoluteSpacing
    };

    enum ResolveProperties {
        FamilyResolved         = 0x0001,
        SizeResolved           = 0x0002,
        StyleHintResolved      = 0x0004,
        StyleStrategyResolved  = 0x0008,
        WeightResolved         = 0x0010,
        StyleResolved          = 0x0020,
        UnderlineResolved      = 0x0040,
        OverlineResolved       = 0x0080,
        StrikeOutResolved      = 0x0100,
        FixedPitchResolved     = 0x0200,
        StretchResolved        = 0x0400,
        KerningResolved        = 0x0800,
        CapitalizationResolved = 0x1000,
        LetterSpacingResolved  = 0x2000,
        WordSpacingResolved    = 0x4000,
        AllPropertiesResolved  = 0x7fff
    };

    QFont();
    QFont(const QString &family, int pointSize = -1, int weight = -1, bool italic = false);
    QFont(const QFont &, QPaintDevice *pd);
    QFont(const QFont &);
    ~QFont();

    QString family() const;
    void setFamily(const QString &);

    int pointSize() const;
    void setPointSize(int);
    qreal pointSizeF() const;
    void setPointSizeF(qreal);

    int pixelSize() const;
    void setPixelSize(int);

    int weight() const;
    void setWeight(int);

    inline bool bold() const;
    inline void setBold(bool);

    void setStyle(Style style);
    Style style() const;

    inline bool italic() const;
    inline void setItalic(bool b);

    bool underline() const;
    void setUnderline(bool);

    bool overline() const;
    void setOverline(bool);

    bool strikeOut() const;
    void setStrikeOut(bool);

    bool fixedPitch() const;
    void setFixedPitch(bool);

    bool kerning() const;
    void setKerning(bool);

    StyleHint styleHint() const;
    StyleStrategy styleStrategy() const;
    void setStyleHint(StyleHint, StyleStrategy = PreferDefault);
    void setStyleStrategy(StyleStrategy s);

    int stretch() const;
    void setStretch(int);

    qreal letterSpacing() const;
    SpacingType letterSpacingType() const;
    void setLetterSpacing(SpacingType type, qreal spacing);

    qreal wordSpacing() const;
    void setWordSpacing(qreal spacing);

    void setCapitalization(Capitalization);
    Capitalization capitalization() const;

    // is raw mode still needed?
    bool rawMode() const;
    void setRawMode(bool);

    // dupicated from QFontInfo
    bool exactMatch() const;

    QFont &operator=(const QFont &);
    bool operator==(const QFont &) const;
    bool operator!=(const QFont &) const;
    bool operator<(const QFont &) const;
    operator QVariant() const;
    bool isCopyOf(const QFont &) const;


#ifdef Q_WS_WIN
    HFONT handle() const;
#else // !Q_WS_WIN
    Qt::HANDLE handle() const;
#endif // Q_WS_WIN
#ifdef Q_WS_MAC
    quint32 macFontID() const;
#endif
#if defined(Q_WS_X11) || defined(Q_WS_QWS)
    FT_Face freetypeFace() const;
#endif

    // needed for X11
    void setRawName(const QString &);
    QString rawName() const;

    QString key() const;

    QString toString() const;
    bool fromString(const QString &);

    static QString substitute(const QString &);
    static QStringList substitutes(const QString &);
    static QStringList substitutions();
    static void insertSubstitution(const QString&, const QString &);
    static void insertSubstitutions(const QString&, const QStringList &);
    static void removeSubstitution(const QString &);
    static void initialize();
    static void cleanup();
#ifndef Q_WS_QWS
    static void cacheStatistics();
#endif

    QString defaultFamily() const;
    QString lastResortFamily() const;
    QString lastResortFont() const;

    QFont resolve(const QFont &) const;
    inline uint resolve() const { return resolve_mask; }
    inline void resolve(uint mask) { resolve_mask = mask; }

#ifdef QT3_SUPPORT
    static QT3_SUPPORT QFont defaultFont();
    static QT3_SUPPORT void setDefaultFont(const QFont &);
    QT3_SUPPORT void setPixelSizeFloat(qreal);
    QT3_SUPPORT qreal pointSizeFloat() const { return pointSizeF(); }
    QT3_SUPPORT void setPointSizeFloat(qreal size) { setPointSizeF(size); }
#endif

private:
    QFont(QFontPrivate *);

    void detach();

#if defined(Q_WS_MAC)
    void macSetFont(QPaintDevice *);
#elif defined(Q_WS_X11)
    void x11SetScreen(int screen = -1);
    int x11Screen() const;
#endif

    friend class QFontPrivate;
    friend class QFontMetrics;
    friend class QFontMetricsF;
    friend class QFontInfo;
    friend class QPainter;
    friend class QPSPrintEngineFont;
    friend class QApplication;
    friend class QWidget;
    friend class QWidgetPrivate;
    friend class Q3TextFormatCollection;
    friend class QTextLayout;
    friend class QTextEngine;
    friend class QStackTextEngine;
    friend class QTextLine;
    friend struct QScriptLine;
    friend class QGLContext;
    friend class QWin32PaintEngine;
    friend class QAlphaPaintEngine;
    friend class QPainterPath;
    friend class QTextItemInt;
    friend class QPicturePaintEngine;

#ifndef QT_NO_DATASTREAM
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QFont &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QFont &);
#endif

    QFontPrivate *d;
    uint resolve_mask;
};


inline bool QFont::bold() const
{ return weight() > Normal; }


inline void QFont::setBold(bool enable)
{ setWeight(enable ? Bold : Normal); }

inline bool QFont::italic() const
{
    return (style() != StyleNormal);
}

inline void QFont::setItalic(bool b) {
    setStyle(b ? StyleItalic : StyleNormal);
}


/*****************************************************************************
  QFont stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QFont &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QFont &);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QFont &);
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif // QFONT_H
