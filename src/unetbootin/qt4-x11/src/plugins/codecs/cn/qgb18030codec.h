/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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

// Contributed by James Su <suzhe@gnuchina.org>

#ifndef QGB18030CODEC_H
#define QGB18030CODEC_H

#include <QtCore/qtextcodec.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TEXTCODEC

class QGb18030Codec : public QTextCodec {
public:
    QGb18030Codec();

    static QByteArray _name() { return "GB18030"; }
    static QList<QByteArray> _aliases() { return QList<QByteArray>(); }
    static int _mibEnum() { return 114; }

    QByteArray name() const { return _name(); }
    QList<QByteArray> aliases() const { return _aliases(); }
    int mibEnum() const { return _mibEnum(); }

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};

class QGbkCodec : public QGb18030Codec {
public:
    QGbkCodec();

    static QByteArray _name();
    static QList<QByteArray> _aliases();
    static int _mibEnum();

    QByteArray name() const { return _name(); }
    QList<QByteArray> aliases() const { return _aliases(); }
    int mibEnum() const { return _mibEnum(); }

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};

class QGb2312Codec : public QGb18030Codec {
public:
    QGb2312Codec();

    static QByteArray _name();
    static QList<QByteArray> _aliases() { return QList<QByteArray>(); }
    static int _mibEnum();

    QByteArray name() const { return _name(); }
    int mibEnum() const { return _mibEnum(); }

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};

#ifdef Q_WS_X11

class QFontGb2312Codec : public QTextCodec
{
public:
    QFontGb2312Codec();

    static QByteArray _name();
    static QList<QByteArray> _aliases() { return QList<QByteArray>(); }
    static int _mibEnum();

    QByteArray name() const { return _name(); }
    int mibEnum() const { return _mibEnum(); }

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};


class QFontGbkCodec : public QTextCodec
{
public:
    QFontGbkCodec();

    static QByteArray _name();
    static QList<QByteArray> _aliases() { return QList<QByteArray>(); }
    static int _mibEnum();

    QByteArray name() const { return _name(); }
    QList<QByteArray> aliases() const { return _aliases(); }
    int mibEnum() const { return _mibEnum(); }

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};

class QFontGb18030_0Codec : public QTextCodec
{
public:
    QFontGb18030_0Codec();

    static QByteArray _name();
    static QList<QByteArray> _aliases() { return QList<QByteArray>(); }
    static int _mibEnum();

    QByteArray name() const { return _name(); }
    QList<QByteArray> aliases() const { return _aliases(); }
    int mibEnum() const { return _mibEnum(); }

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;
};
#endif // Q_WS_X11

#endif // QT_NO_TEXTCODEC

QT_END_NAMESPACE

#endif // QGB18030CODEC_H
