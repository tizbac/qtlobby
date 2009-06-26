/****************************************************************************
** Meta object code from reading C++ file 'AbstractChannel.h'
**
** Created: Fri Jun 26 21:58:01 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/AbstractChannel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AbstractChannel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AbstractChannel[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x0a,
      53,   45,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AbstractChannel[] = {
    "AbstractChannel\0\0input\0receiveInput(QString)\0"
    "command\0receiveCommand(Command)\0"
};

const QMetaObject AbstractChannel::staticMetaObject = {
    { &AbstractLobbyTab::staticMetaObject, qt_meta_stringdata_AbstractChannel,
      qt_meta_data_AbstractChannel, 0 }
};

const QMetaObject *AbstractChannel::metaObject() const
{
    return &staticMetaObject;
}

void *AbstractChannel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractChannel))
        return static_cast<void*>(const_cast< AbstractChannel*>(this));
    return AbstractLobbyTab::qt_metacast(_clname);
}

int AbstractChannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractLobbyTab::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: receiveInput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: receiveCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
