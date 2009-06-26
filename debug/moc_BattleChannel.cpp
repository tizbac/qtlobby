/****************************************************************************
** Meta object code from reading C++ file 'BattleChannel.h'
**
** Created: Fri Jun 26 21:58:03 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/BattleChannel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BattleChannel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BattleChannel[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      23,   15,   14,   14, 0x05,
      54,   47,   14,   14, 0x05,
      83,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,  107,   14,   14, 0x0a,
     143,  135,   14,   14, 0x0a,
     167,   14,   14,   14, 0x0a,
     191,   14,   14,   14, 0x0a,
     218,  208,   14,   14, 0x0a,
     255,  247,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BattleChannel[] = {
    "BattleChannel\0\0isReady\0readyStateChanged(bool)\0"
    "sample\0playSample(SampleCollection)\0"
    "newTrayMessage(QString)\0input\0"
    "receiveInput(QString)\0command\0"
    "receiveCommand(Command)\0updateMapImage(QString)\0"
    "fillModOptions()\0isChecked\0"
    "onReadyCheckBoxChanged(bool)\0checked\0"
    "onStartScriptPushButtonClicked(bool)\0"
};

const QMetaObject BattleChannel::staticMetaObject = {
    { &AbstractChannel::staticMetaObject, qt_meta_stringdata_BattleChannel,
      qt_meta_data_BattleChannel, 0 }
};

const QMetaObject *BattleChannel::metaObject() const
{
    return &staticMetaObject;
}

void *BattleChannel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BattleChannel))
        return static_cast<void*>(const_cast< BattleChannel*>(this));
    return AbstractChannel::qt_metacast(_clname);
}

int BattleChannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractChannel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readyStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: playSample((*reinterpret_cast< SampleCollection(*)>(_a[1]))); break;
        case 2: newTrayMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: receiveInput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: receiveCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 5: updateMapImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: fillModOptions(); break;
        case 7: onReadyCheckBoxChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: onStartScriptPushButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void BattleChannel::readyStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BattleChannel::playSample(SampleCollection _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BattleChannel::newTrayMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
