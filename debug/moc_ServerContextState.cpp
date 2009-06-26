/****************************************************************************
** Meta object code from reading C++ file 'ServerContextState.h'
**
** Created: Fri Jun 26 21:57:45 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/ServerContextState.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ServerContextState.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServerContextState[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      60,   19,   19,   19, 0x05,
      85,   19,   19,   19, 0x05,
     112,   19,   19,   19, 0x05,
     130,   19,   19,   19, 0x05,
     162,   19,   19,   19, 0x05,
     194,   19,   19,   19, 0x05,
     225,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     256,   19,   19,   19, 0x0a,
     279,  271,   19,   19, 0x0a,
     308,  300,   19,   19, 0x0a,
     332,   19,   19,   19, 0x0a,
     375,   19,   19,   19, 0x0a,
     418,   19,   19,   19, 0x0a,
     440,   19,   19,   19, 0x0a,
     458,   19,   19,   19, 0x0a,
     486,   19,  481,   19, 0x0a,
     511,  505,   19,   19, 0x0a,
     561,   19,   19,   19, 0x0a,
     579,   19,   19,   19, 0x0a,
     595,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ServerContextState[] = {
    "ServerContextState\0\0"
    "connectionStateChanged(ConnectionState)\0"
    "serverContextConnected()\0"
    "serverContextReConnected()\0logWrite(QString)\0"
    "renameLoginNameSuccess(QString)\0"
    "renameLoginNameFailure(QString)\0"
    "changePasswordSuccess(QString)\0"
    "changePasswordFailure(QString)\0"
    "authenticate()\0message\0sendMessage(QString)\0"
    "command\0receiveCommand(Command)\0"
    "stateChanged(QAbstractSocket::SocketState)\0"
    "displayError(QAbstractSocket::SocketError)\0"
    "establishConnection()\0forceDisconnect()\0"
    "setConfiguration(QUrl)\0QUrl\0"
    "getConfiguration()\0state\0"
    "connectionStateChanged(ServerContextState::State)\0"
    "acceptAgreement()\0denyAgreement()\0"
    "ping()\0"
};

const QMetaObject ServerContextState::staticMetaObject = {
    { &NetworkInterface::staticMetaObject, qt_meta_stringdata_ServerContextState,
      qt_meta_data_ServerContextState, 0 }
};

const QMetaObject *ServerContextState::metaObject() const
{
    return &staticMetaObject;
}

void *ServerContextState::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerContextState))
        return static_cast<void*>(const_cast< ServerContextState*>(this));
    return NetworkInterface::qt_metacast(_clname);
}

int ServerContextState::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NetworkInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectionStateChanged((*reinterpret_cast< ConnectionState(*)>(_a[1]))); break;
        case 1: serverContextConnected(); break;
        case 2: serverContextReConnected(); break;
        case 3: logWrite((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: renameLoginNameSuccess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: renameLoginNameFailure((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: changePasswordSuccess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: changePasswordFailure((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: authenticate(); break;
        case 9: sendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: receiveCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 11: stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 12: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 13: establishConnection(); break;
        case 14: forceDisconnect(); break;
        case 15: setConfiguration((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 16: { QUrl _r = getConfiguration();
            if (_a[0]) *reinterpret_cast< QUrl*>(_a[0]) = _r; }  break;
        case 17: connectionStateChanged((*reinterpret_cast< ServerContextState::State(*)>(_a[1]))); break;
        case 18: acceptAgreement(); break;
        case 19: denyAgreement(); break;
        case 20: ping(); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void ServerContextState::connectionStateChanged(ConnectionState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServerContextState::serverContextConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ServerContextState::serverContextReConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ServerContextState::logWrite(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ServerContextState::renameLoginNameSuccess(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ServerContextState::renameLoginNameFailure(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ServerContextState::changePasswordSuccess(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ServerContextState::changePasswordFailure(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
