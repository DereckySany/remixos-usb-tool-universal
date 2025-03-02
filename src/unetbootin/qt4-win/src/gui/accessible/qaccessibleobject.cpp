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

#include "qaccessibleobject.h"

#ifndef QT_NO_ACCESSIBILITY

#include "qapplication.h"
#include "qwidget.h"
#include "qpointer.h"
#include "qmetaobject.h"
#include "qvarlengtharray.h"

QT_BEGIN_NAMESPACE

class QAccessibleObjectPrivate
{
public:
    QPointer<QObject> object;

    QList<QByteArray> actionList() const;
};

QList<QByteArray> QAccessibleObjectPrivate::actionList() const
{
    QList<QByteArray> actionList;

    if (!object)
        return actionList;

    const QMetaObject *mo = object->metaObject();
    Q_ASSERT(mo);

    QByteArray defaultAction = QMetaObject::normalizedSignature(
        mo->classInfo(mo->indexOfClassInfo("DefaultSlot")).value());

    for (int i = 0; i < mo->methodCount(); ++i) {
        const QMetaMethod member = mo->method(i);
        if (member.methodType() != QMetaMethod::Slot && member.access() != QMetaMethod::Public)
            continue;

        if (!qstrcmp(member.tag(), "QACCESSIBLE_SLOT")) {
            if (member.signature() == defaultAction)
                actionList.prepend(defaultAction);
            else
                actionList << member.signature();
        }
    }

    return actionList;
}

/*!
    \class QAccessibleObject
    \brief The QAccessibleObject class implements parts of the
    QAccessibleInterface for QObjects.

    \ingroup accessibility

    This class is mainly provided for convenience. All subclasses of
    the QAccessibleInterface that provide implementations of non-widget objects
    should use this class as their base class.

    \sa QAccessible, QAccessibleWidget
*/

/*!
    Creates a QAccessibleObject for \a object.
*/
QAccessibleObject::QAccessibleObject(QObject *object)
{
    d = new QAccessibleObjectPrivate;
    d->object = object;
}

/*!
    Destroys the QAccessibleObject.

    This only happens when a call to release() decrements the internal
    reference counter to zero.
*/
QAccessibleObject::~QAccessibleObject()
{
    delete d;
}

/*!
    \reimp
*/
QObject *QAccessibleObject::object() const
{
#ifndef QT_NO_DEBUG
    if (!d->object)
        qWarning("QAccessibleInterface is invalid. Crash pending...");
#endif
    return d->object;
}

/*!
    \reimp
*/
bool QAccessibleObject::isValid() const
{
    return !d->object.isNull();
}

/*! \reimp */
QRect QAccessibleObject::rect(int) const
{
    return QRect();
}

/*! \reimp */
void QAccessibleObject::setText(Text, int, const QString &)
{
}

/*! \reimp */
int QAccessibleObject::userActionCount(int) const
{
    return 0;
}

/*! \reimp */
bool QAccessibleObject::doAction(int, int, const QVariantList &)
{
    return false;
}

static const char * const action_text[][5] =
{
    // Name, Description, Value, Help, Accelerator
    { "Press", "", "", "", "Space" },
    { "SetFocus", "Passes focus to this widget", "", "", "" },
    { "Increase", "", "", "", "" },
    { "Decrease", "", "", "", "" },
    { "Accept", "", "", "", "" },
    { "Cancel", "", "", "", "" },
    { "Select", "", "", "", "" },
    { "ClearSelection", "", "", "", "" },
    { "RemoveSelection", "", "", "", "" },
    { "ExtendSelection", "", "", "", "" },
    { "AddToSelection", "", "", "", "" }
};

/*! \reimp */
QString QAccessibleObject::actionText(int action, Text t, int child) const
{
    if (child || action > FirstStandardAction || action < LastStandardAction || t > Accelerator)
        return QString();

    return QString::fromLatin1(action_text[-(action - FirstStandardAction)][t]);
}


/*!
    \class QAccessibleApplication
    \brief The QAccessibleApplication class implements the QAccessibleInterface for QApplication.

    \internal

    \ingroup accessibility
*/

/*!
    Creates a QAccessibleApplication for the QApplication object referenced by qApp.
*/
QAccessibleApplication::QAccessibleApplication()
: QAccessibleObject(qApp)
{
}

// all toplevel widgets except popups and the desktop
static QWidgetList topLevelWidgets()
{
    QWidgetList list;
    const QWidgetList tlw(qApp->topLevelWidgets());
    for (int i = 0; i < tlw.count(); ++i) {
        QWidget *w = tlw.at(i);
        if (!(w->windowType() == Qt::Popup) && !(w->windowType() == Qt::Desktop))
            list.append(w);
    }

    return list;
}

/*! \reimp */
int QAccessibleApplication::childCount() const
{
    return topLevelWidgets().count();
}

/*! \reimp */
int QAccessibleApplication::indexOfChild(const QAccessibleInterface *child) const
{
    if (!child->object()->isWidgetType())
        return -1;

    const QWidgetList tlw(topLevelWidgets());
    int index = tlw.indexOf(static_cast<QWidget*>(child->object()));
    if (index != -1)
        ++index;
    return index;
}

/*! \reimp */
int QAccessibleApplication::childAt(int x, int y) const
{
    const QWidgetList tlw(topLevelWidgets());
    for (int i = 0; i < tlw.count(); ++i) {
        QWidget *w = tlw.at(i);
        if (w->frameGeometry().contains(x,y))
            return i+1;
    }
    return -1;
}

/*! \reimp */
QAccessible::Relation QAccessibleApplication::relationTo(int child, const
        QAccessibleInterface *other, int otherChild) const
{
    QObject *o = other ? other->object() : 0;
    if (!o)
        return Unrelated;

    if(o == object()) {
        if (child && !otherChild)
            return Child;
        if (!child && otherChild)
            return Ancestor;
        if (!child && !otherChild)
            return Self;
    }

    QWidgetList tlw(topLevelWidgets());
    if (tlw.contains(qobject_cast<QWidget*>(o)))
        return Ancestor;

    for (int i = 0; i < tlw.count(); ++i) {
        QWidget *w = tlw.at(i);
        QObjectList cl = qFindChildren<QObject *>(w, QString());
        if (cl.contains(o))
            return Ancestor;
    }

    return Unrelated;
}

/*! \reimp */
int QAccessibleApplication::navigate(RelationFlag relation, int entry,
                                     QAccessibleInterface **target) const
{
    if (!target)
        return -1;

    *target = 0;
    QObject *targetObject = 0;

    switch (relation) {
    case Self:
        targetObject = object();
        break;
    case Child:
        if (entry > 0 && entry <= childCount()) {
            const QWidgetList tlw(topLevelWidgets());
            if (tlw.count() >= entry)
                targetObject = tlw.at(entry-1);
        } else {
            return -1;
        }
        break;
    case FocusChild:
        targetObject = qApp->activeWindow();
        break;
    default:
        break;
    }
    *target = QAccessible::queryAccessibleInterface(targetObject);
    return *target ? 0 : -1;
}

/*! \reimp */
QString QAccessibleApplication::text(Text t, int) const
{
    switch (t) {
    case Name:
        if (qApp->activeWindow())
            return qApp->activeWindow()->windowTitle();
        break;
    case Description:
        return qApp->applicationFilePath();
    default:
        break;
    }
    return QString();
}

/*! \reimp */
QAccessible::Role QAccessibleApplication::role(int) const
{
    return Application;
}

/*! \reimp */
QAccessible::State QAccessibleApplication::state(int) const
{
    return qApp->activeWindow() ? Focused : Normal;
}

/*! \reimp */
int QAccessibleApplication::userActionCount(int) const
{
    return 1;
}

/*! \reimp */
bool QAccessibleApplication::doAction(int action, int child, const QVariantList &param)
{
    if (action == 0 || action == 1) {
        QWidget *w = 0;
        w = qApp->activeWindow();
        if (!w)
            w = topLevelWidgets().at(0);
        if (!w)
            return false;
        w->activateWindow();
        return true;
    }
    return QAccessibleObject::doAction(action, child, param);
}

/*! \reimp */
QString QAccessibleApplication::actionText(int action, Text text, int child) const
{
    QString str;
    if ((action == 0 || action == 1) && !child) switch (text) {
    case Name:
        return QApplication::tr("Activate");
    case Description:
        return QApplication::tr("Activates the program's main window");
    default:
        break;
    }
    return QAccessibleObject::actionText(action, text, child);
}

// ### Qt 5: remove me - binary compatibility hack
QAccessibleObjectEx::QAccessibleObjectEx(QObject *object)
{
    d = new QAccessibleObjectPrivate;
    d->object = object;
}
bool QAccessibleObjectEx::isValid() const
{ return reinterpret_cast<const QAccessibleObject *>(this)->QAccessibleObject::isValid(); }
QObject *QAccessibleObjectEx::object() const
{ return reinterpret_cast<const QAccessibleObject *>(this)->QAccessibleObject::object(); }
QRect QAccessibleObjectEx::rect(int child) const
{ return reinterpret_cast<const QAccessibleObject *>(this)->QAccessibleObject::rect(child); }
void QAccessibleObjectEx::setText(Text t, int child, const QString &text)
{ reinterpret_cast<QAccessibleObject *>(this)->QAccessibleObject::setText(t, child, text); }
int QAccessibleObjectEx::userActionCount(int child) const
{ return reinterpret_cast<const QAccessibleObject *>(this)->QAccessibleObject::userActionCount(child); }
bool QAccessibleObjectEx::doAction(int action, int child, const QVariantList &params)
{ return reinterpret_cast<QAccessibleObject *>(this)->QAccessibleObject::doAction(action, child, params); }
QString QAccessibleObjectEx::actionText(int action, Text t, int child) const
{ return reinterpret_cast<const QAccessibleObject *>(this)->QAccessibleObject::actionText(action, t, child); }
QAccessibleObjectEx::~QAccessibleObjectEx()
{ delete d; }

QT_END_NAMESPACE

#endif //QT_NO_ACCESSIBILITY
