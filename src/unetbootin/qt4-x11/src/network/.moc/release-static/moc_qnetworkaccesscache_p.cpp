/****************************************************************************
** Meta object code from reading C++ file 'qnetworkaccesscache_p.h'
**
** Created: Thu Mar 5 18:42:42 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../access/qnetworkaccesscache_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qnetworkaccesscache_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QNetworkAccessCache[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QNetworkAccessCache[] = {
    "QNetworkAccessCache\0\0"
    "entryReady(QNetworkAccessCache::CacheableObject*)\0"
};

const QMetaObject QNetworkAccessCache::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QNetworkAccessCache,
      qt_meta_data_QNetworkAccessCache, 0 }
};

const QMetaObject *QNetworkAccessCache::metaObject() const
{
    return &staticMetaObject;
}

void *QNetworkAccessCache::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QNetworkAccessCache))
        return static_cast<void*>(const_cast< QNetworkAccessCache*>(this));
    return QObject::qt_metacast(_clname);
}

int QNetworkAccessCache::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: entryReady((*reinterpret_cast< QNetworkAccessCache::CacheableObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QNetworkAccessCache::entryReady(QNetworkAccessCache::CacheableObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
