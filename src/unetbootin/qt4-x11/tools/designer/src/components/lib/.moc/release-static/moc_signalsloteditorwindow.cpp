/****************************************************************************
** Meta object code from reading C++ file 'signalsloteditorwindow.h'
**
** Created: Thu Mar 5 20:52:02 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../signalsloteditor/signalsloteditorwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signalsloteditorwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qdesigner_internal__SignalSlotEditorWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      49,   44,   43,   43, 0x0a,
     104,  100,   43,   43, 0x08,
     145,  139,   43,   43, 0x08,
     214,  180,   43,   43, 0x08,
     288,   43,   43,   43, 0x08,
     304,   43,   43,   43, 0x08,
     323,   43,   43,   43, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qdesigner_internal__SignalSlotEditorWindow[] = {
    "qdesigner_internal::SignalSlotEditorWindow\0"
    "\0form\0setActiveFormWindow(QDesignerFormWindowInterface*)\0"
    "con\0updateDialogSelection(Connection*)\0"
    "index\0updateEditorSelection(QModelIndex)\0"
    "formWindow,object,newName,oldName\0"
    "objectNameChanged(QDesignerFormWindowInterface*,QObject*,QString,QStri"
    "ng)\0"
    "addConnection()\0removeConnection()\0"
    "updateUi()\0"
};

const QMetaObject qdesigner_internal::SignalSlotEditorWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_qdesigner_internal__SignalSlotEditorWindow,
      qt_meta_data_qdesigner_internal__SignalSlotEditorWindow, 0 }
};

const QMetaObject *qdesigner_internal::SignalSlotEditorWindow::metaObject() const
{
    return &staticMetaObject;
}

void *qdesigner_internal::SignalSlotEditorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qdesigner_internal__SignalSlotEditorWindow))
        return static_cast<void*>(const_cast< SignalSlotEditorWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int qdesigner_internal::SignalSlotEditorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setActiveFormWindow((*reinterpret_cast< QDesignerFormWindowInterface*(*)>(_a[1]))); break;
        case 1: updateDialogSelection((*reinterpret_cast< Connection*(*)>(_a[1]))); break;
        case 2: updateEditorSelection((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: objectNameChanged((*reinterpret_cast< QDesignerFormWindowInterface*(*)>(_a[1])),(*reinterpret_cast< QObject*(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 4: addConnection(); break;
        case 5: removeConnection(); break;
        case 6: updateUi(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
