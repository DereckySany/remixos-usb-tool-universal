/****************************************************************************
** Meta object code from reading C++ file 'qabstractscrollarea_p.h'
**
** Created: Fri May 30 22:29:07 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../widgets/qabstractscrollarea_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qabstractscrollarea_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QAbstractScrollAreaFilter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QAbstractScrollAreaFilter[] = {
    "QAbstractScrollAreaFilter\0"
};

const QMetaObject QAbstractScrollAreaFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QAbstractScrollAreaFilter,
      qt_meta_data_QAbstractScrollAreaFilter, 0 }
};

const QMetaObject *QAbstractScrollAreaFilter::metaObject() const
{
    return &staticMetaObject;
}

void *QAbstractScrollAreaFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAbstractScrollAreaFilter))
	return static_cast<void*>(const_cast< QAbstractScrollAreaFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int QAbstractScrollAreaFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
