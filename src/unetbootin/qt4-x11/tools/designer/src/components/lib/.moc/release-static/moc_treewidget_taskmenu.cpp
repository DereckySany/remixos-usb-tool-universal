/****************************************************************************
** Meta object code from reading C++ file 'treewidget_taskmenu.h'
**
** Created: Thu Mar 5 20:52:26 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../taskmenu/treewidget_taskmenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'treewidget_taskmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qdesigner_internal__TreeWidgetTaskMenu[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      40,   39,   39,   39, 0x08,
      52,   39,   39,   39, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qdesigner_internal__TreeWidgetTaskMenu[] = {
    "qdesigner_internal::TreeWidgetTaskMenu\0"
    "\0editItems()\0updateSelection()\0"
};

const QMetaObject qdesigner_internal::TreeWidgetTaskMenu::staticMetaObject = {
    { &QDesignerTaskMenu::staticMetaObject, qt_meta_stringdata_qdesigner_internal__TreeWidgetTaskMenu,
      qt_meta_data_qdesigner_internal__TreeWidgetTaskMenu, 0 }
};

const QMetaObject *qdesigner_internal::TreeWidgetTaskMenu::metaObject() const
{
    return &staticMetaObject;
}

void *qdesigner_internal::TreeWidgetTaskMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qdesigner_internal__TreeWidgetTaskMenu))
        return static_cast<void*>(const_cast< TreeWidgetTaskMenu*>(this));
    return QDesignerTaskMenu::qt_metacast(_clname);
}

int qdesigner_internal::TreeWidgetTaskMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDesignerTaskMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editItems(); break;
        case 1: updateSelection(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
