/****************************************************************************
** Meta object code from reading C++ file 'LobbyTabs.h'
**
** Created: Fri Jun 26 21:57:53 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/LobbyTabs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LobbyTabs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LobbyTabs[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      19,   11,   10,   10, 0x05,
      58,   40,   10,   10, 0x05,
     105,   93,   10,   10, 0x05,
     134,  126,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     174,  158,   10,   10, 0x0a,
     214,   11,   10,   10, 0x0a,
     244,  238,   10,   10, 0x0a,
     266,   11,   10,   10, 0x0a,
     297,  291,   10,   10, 0x0a,
     324,   10,   10,   10, 0x0a,
     335,  126,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LobbyTabs[] = {
    "LobbyTabs\0\0command\0sendCommand(Command)\0"
    "name,lobbyTabType\0currentTabChanged(QString,QString)\0"
    "isBattleTab\0hideBattleList(bool)\0"
    "isReady\0readyStateChanged(bool)\0"
    "connectionState\0connectionStateChanged(ConnectionState)\0"
    "receiveCommand(Command)\0input\0"
    "receiveInput(QString)\0sendCommandSlot(Command)\0"
    "index\0currentTabChangedSlot(int)\0"
    "closeTab()\0onReadyStateChanged(bool)\0"
};

const QMetaObject LobbyTabs::staticMetaObject = {
    { &AbstractStateClient::staticMetaObject, qt_meta_stringdata_LobbyTabs,
      qt_meta_data_LobbyTabs, 0 }
};

const QMetaObject *LobbyTabs::metaObject() const
{
    return &staticMetaObject;
}

void *LobbyTabs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LobbyTabs))
        return static_cast<void*>(const_cast< LobbyTabs*>(this));
    return AbstractStateClient::qt_metacast(_clname);
}

int LobbyTabs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractStateClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 1: currentTabChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: hideBattleList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: readyStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: connectionStateChanged((*reinterpret_cast< ConnectionState(*)>(_a[1]))); break;
        case 5: receiveCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 6: receiveInput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: sendCommandSlot((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 8: currentTabChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: closeTab(); break;
        case 10: onReadyStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void LobbyTabs::sendCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LobbyTabs::currentTabChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LobbyTabs::hideBattleList(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LobbyTabs::readyStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
