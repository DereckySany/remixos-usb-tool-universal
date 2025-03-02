/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Designer of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDESIGNER_PROPERTYCOMMAND_H
#define QDESIGNER_PROPERTYCOMMAND_H

#include "qdesigner_formwindowcommand_p.h"

#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QPair>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QDesignerPropertySheetExtension;

namespace qdesigner_internal {

class QDesignerIntegration;

enum SpecialProperty {
        SP_None, SP_ObjectName, SP_LayoutName, SP_SpacerName,SP_WindowTitle,
        SP_MinimumSize, SP_MaximumSize, SP_Geometry, SP_Icon,SP_CurrentTabName,
        SP_AutoDefault, SP_Alignment
};

//Determine special property
enum SpecialProperty getSpecialProperty(const QString& propertyName);


// A helper class for applying properties to objects.
// Can be used for Set commands (setValue(), restoreOldValue()) or
// Reset Commands     restoreDefaultValue(), restoreOldValue()).
class PropertyHelper {
public:
    // A pair of Value and changed flag
    typedef QPair<QVariant, bool> Value;

    enum ObjectType {OT_Object, OT_FreeAction, OT_AssociatedAction, OT_Widget};

    PropertyHelper(QObject* object,
                   SpecialProperty specialProperty,
                   QDesignerPropertySheetExtension *sheet,
                   int index);

    QObject *object() const { return m_object; }
    SpecialProperty specialProperty() const { return m_specialProperty; }
    // set a new value
    Value setValue(QDesignerFormWindowInterface *fw, const QVariant &value, bool changed, unsigned subPropertyMask);

    // restore old value
    Value restoreOldValue(QDesignerFormWindowInterface *fw);
    // set default value
    Value restoreDefaultValue(QDesignerFormWindowInterface *fw);

    inline QVariant oldValue() const
    { return m_oldValue.first; }

    inline void setOldValue(const QVariant &oldValue)
    { m_oldValue.first = oldValue; }

    // required updates for this property (bit mask)
    enum UpdateMask { UpdatePropertyEditor=1, UpdateObjectInspector=2 };
    unsigned updateMask() const;

    // can be merged into one command (that is, object and name match)
    bool canMerge(const PropertyHelper &other) const;
    QDesignerIntegration *integration(QDesignerFormWindowInterface *fw) const;

    static void triggerActionChanged(QAction *a);

private:
    // Apply the value and update. Returns corrected value
    Value applyValue(QDesignerFormWindowInterface *fw, const QVariant &oldValue, Value newValue);

    static void checkApplyWidgetValue(QDesignerFormWindowInterface *fw, QWidget* w,
                                      SpecialProperty specialProperty, QVariant &v);

    void updateObject(QDesignerFormWindowInterface *fw, const QVariant &oldValue, const QVariant &newValue);
    QVariant findDefaultValue(QDesignerFormWindowInterface *fw) const;
    void ensureUniqueObjectName(QDesignerFormWindowInterface *fw, QObject *object) const;
    SpecialProperty m_specialProperty;

    QPointer<QObject> m_object;
    ObjectType m_objectType;
    QPointer<QWidget> m_parentWidget;

    QDesignerPropertySheetExtension *m_propertySheet;
    int m_index;

    Value m_oldValue;
};

// Base class for commands that can be applied to several widgets

class QDESIGNER_SHARED_EXPORT PropertyListCommand : public QDesignerFormWindowCommand {
public:
    typedef QList<QObject *> ObjectList;

    explicit PropertyListCommand(QDesignerFormWindowInterface *formWindow);

    QObject* object(int index = 0) const;

    QVariant oldValue(int index = 0) const;

    void setOldValue(const QVariant &oldValue, int index = 0);

    // Calls restoreDefaultValue() and update()
    virtual void undo();

protected:
    typedef QList<PropertyHelper> PropertyHelperList;


    // add an object
    bool add(QObject *object, const QString &propertyName);

    // Init from a list and make sure referenceObject is added first to obtain the right property group
    bool initList(const ObjectList &list, const QString &apropertyName, QObject *referenceObject = 0);

    // set a new value, return update mask
    unsigned setValue(QVariant value, bool changed, unsigned subPropertyMask);

    // restore old value,  return update mask
    unsigned  restoreOldValue();
    // set default value,  return update mask
    unsigned  restoreDefaultValue();

    // update designer
    void update(unsigned updateMask);

    // check if lists are aequivalent for command merging (same widgets and props)
    bool canMergeLists(const PropertyHelperList& other) const;

    PropertyHelperList& propertyHelperList() { return m_propertyHelperList; }
    const PropertyHelperList& propertyHelperList() const { return m_propertyHelperList; }

    const QString propertyName() const;
    SpecialProperty specialProperty() const;

    // Helper struct describing a property used for checking whether
    // properties of different widgets are equivalent
    struct PropertyDescription {
    public:
        PropertyDescription();
        PropertyDescription(const QString &propertyName, QDesignerPropertySheetExtension *propertySheet, int index);
        bool equals(const PropertyDescription &p) const;
        void debug() const;

        QString m_propertyName;
        QString m_propertyGroup;
        QVariant::Type m_propertyType;
        SpecialProperty m_specialProperty;
    };
    const PropertyDescription &propertyDescription() const { return  m_propertyDescription; }

private:
    PropertyDescription m_propertyDescription;
    PropertyHelperList m_propertyHelperList;
};

class QDESIGNER_SHARED_EXPORT SetPropertyCommand: public PropertyListCommand
{

public:
    typedef QList<QObject *> ObjectList;

    explicit SetPropertyCommand(QDesignerFormWindowInterface *formWindow);

    bool init(QObject *object, const QString &propertyName, const QVariant &newValue);
    bool init(const ObjectList &list, const QString &propertyName, const QVariant &newValue,
              QObject *referenceObject = 0, bool enableSubPropertyHandling = true);


    inline QVariant newValue() const
    { return m_newValue; }

    inline void setNewValue(const QVariant &newValue)
    { m_newValue = newValue; }

    int id() const;
    bool mergeWith(const QUndoCommand *other);

    virtual void redo();
private:
    unsigned subPropertyMask(const QVariant &newValue, QObject *referenceObject);
    void setDescription();
    QVariant m_newValue;
    unsigned m_subPropertyMask;
};

class QDESIGNER_SHARED_EXPORT ResetPropertyCommand: public PropertyListCommand
{

public:
    typedef QList<QObject *> ObjectList;

    explicit ResetPropertyCommand(QDesignerFormWindowInterface *formWindow);

    bool init(QObject *object, const QString &propertyName);
    bool init(const ObjectList &list, const QString &propertyName, QObject *referenceObject = 0);

    virtual void redo();

protected:
    virtual bool mergeWith(const QUndoCommand *) { return false; }

private:
    void setDescription();
    QString m_propertyName;
};


class QDESIGNER_SHARED_EXPORT AddDynamicPropertyCommand: public QDesignerFormWindowCommand
{

public:
    explicit AddDynamicPropertyCommand(QDesignerFormWindowInterface *formWindow);

    bool init(const QList<QObject *> &selection, QObject *current, const QString &propertyName, const QVariant &value);

    virtual void redo();
    virtual void undo();
private:
    void setDescription();
    QString m_propertyName;
    QList<QObject *> m_selection;
    QVariant m_value;
};

class QDESIGNER_SHARED_EXPORT RemoveDynamicPropertyCommand: public QDesignerFormWindowCommand
{

public:
    explicit RemoveDynamicPropertyCommand(QDesignerFormWindowInterface *formWindow);

    bool init(const QList<QObject *> &selection, QObject *current, const QString &propertyName);

    virtual void redo();
    virtual void undo();
private:
    void setDescription();
    QString m_propertyName;
    QMap<QObject *, QPair<QVariant, bool> > m_objectToValueAndChanged;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_PROPERTYCOMMAND_H
