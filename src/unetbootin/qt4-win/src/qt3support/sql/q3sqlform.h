/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt3Support module of the Qt Toolkit.
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

#ifndef Q3SQLFORM_H
#define Q3SQLFORM_H

#include <QtCore/qobject.h>
#include <QtCore/qmap.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Qt3Support)

#ifndef QT_NO_SQL_FORM

class QSqlField;
class QSqlRecord;
class Q3SqlEditorFactory;
class Q3SqlPropertyMap;
class QWidget;
class Q3SqlFormPrivate;

class Q_COMPAT_EXPORT Q3SqlForm : public QObject
{
    Q_OBJECT
public:
    Q3SqlForm(QObject * parent = 0);
    ~Q3SqlForm();

    virtual void insert(QWidget * widget, const QString& field);
    virtual void remove(const QString& field);
    int         count() const;

    QWidget *   widget(int i) const;
    QSqlField * widgetToField(QWidget * widget) const;
    QWidget *   fieldToWidget(QSqlField * field) const;

    void        installPropertyMap(Q3SqlPropertyMap * map);

    virtual void setRecord(QSqlRecord* buf);

public Q_SLOTS:
    virtual void readField(QWidget * widget);
    virtual void writeField(QWidget * widget);
    virtual void readFields();
    virtual void writeFields();

    virtual void clear();
    virtual void clearValues();

protected:
    virtual void insert(QWidget * widget, QSqlField * field);
    virtual void remove(QWidget * widget);
    void clearMap();

private:
    Q_DISABLE_COPY(Q3SqlForm)

    virtual void sync();
    Q3SqlFormPrivate* d;
};

#endif // QT_NO_SQL_FORM

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3SQLFORM_H
