/****************************************************************************
** Meta object code from reading C++ file 'NetworkInterface.h'
**
** Created: Fri Jun 26 21:57:47 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/NetworkInterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetworkInterface[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      26,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   52,   17,   17, 0x0a,
      77,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NetworkInterface[] = {
    "NetworkInterface\0\0message\0"
    "incommingMessage(QString)\0out\0"
    "socketWrite(QString)\0socketRead()\0"
};

const QMetaObject NetworkInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkInterface,
      qt_meta_data_NetworkInterface, 0 }
};

const QMetaObject *NetworkInterface::metaObject() const
{
    return &staticMetaObject;
}

void *NetworkInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkInterface))
        return static_cast<void*>(const_cast< NetworkInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: incommingMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: socketWrite((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: socketRead(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetworkInterface::incommingMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
