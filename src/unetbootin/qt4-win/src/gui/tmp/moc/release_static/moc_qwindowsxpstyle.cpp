/****************************************************************************
** Meta object code from reading C++ file 'qwindowsxpstyle.h'
**
** Created: Fri May 30 22:33:28 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../styles/qwindowsxpstyle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwindowsxpstyle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QWindowsXPStyle[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      50,   23,   17,   16, 0x09,
     148,  128,   17,   16, 0x29,

       0        // eod
};

static const char qt_meta_stringdata_QWindowsXPStyle[] = {
    "QWindowsXPStyle\0\0QIcon\0"
    "standardIcon,option,widget\0"
    "standardIconImplementation(StandardPixmap,const QStyleOption*,const QW"
    "idget*)\0"
    "standardIcon,option\0"
    "standardIconImplementation(StandardPixmap,const QStyleOption*)\0"
};

const QMetaObject QWindowsXPStyle::staticMetaObject = {
    { &QWindowsStyle::staticMetaObject, qt_meta_stringdata_QWindowsXPStyle,
      qt_meta_data_QWindowsXPStyle, 0 }
};

const QMetaObject *QWindowsXPStyle::metaObject() const
{
    return &staticMetaObject;
}

void *QWindowsXPStyle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QWindowsXPStyle))
	return static_cast<void*>(const_cast< QWindowsXPStyle*>(this));
    return QWindowsStyle::qt_metacast(_clname);
}

int QWindowsXPStyle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWindowsStyle::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QIcon _r = standardIconImplementation((*reinterpret_cast< StandardPixmap(*)>(_a[1])),(*reinterpret_cast< const QStyleOption*(*)>(_a[2])),(*reinterpret_cast< const QWidget*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QIcon*>(_a[0]) = _r; }  break;
        case 1: { QIcon _r = standardIconImplementation((*reinterpret_cast< StandardPixmap(*)>(_a[1])),(*reinterpret_cast< const QStyleOption*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QIcon*>(_a[0]) = _r; }  break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
