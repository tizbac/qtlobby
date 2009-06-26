/****************************************************************************
** Meta object code from reading C++ file 'AbstractStateClient.h'
**
** Created: Fri Jun 26 21:57:48 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/AbstractStateClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AbstractStateClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AbstractStateClient[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AbstractStateClient[] = {
    "AbstractStateClient\0\0"
    "connectionStateChanged(ConnectionState)\0"
};

const QMetaObject AbstractStateClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AbstractStateClient,
      qt_meta_data_AbstractStateClient, 0 }
};

const QMetaObject *AbstractStateClient::metaObject() const
{
    return &staticMetaObject;
}

void *AbstractStateClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractStateClient))
        return static_cast<void*>(const_cast< AbstractStateClient*>(this));
    return QObject::qt_metacast(_clname);
}

int AbstractStateClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectionStateChanged((*reinterpret_cast< ConnectionState(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
