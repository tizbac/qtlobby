/****************************************************************************
** Meta object code from reading C++ file 'CommandAssigner.h'
**
** Created: Fri Jun 26 21:57:48 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/CommandAssigner.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommandAssigner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CommandAssigner[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      60,   52,   16,   16, 0x05,
      89,   81,   16,   16, 0x05,
     114,   81,   16,   16, 0x05,
     135,   81,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     158,   52,   16,   16, 0x0a,
     182,   81,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CommandAssigner[] = {
    "CommandAssigner\0\0serverContextStateCommand(Command)\0"
    "message\0sendMessage(QString)\0command\0"
    "lobbyTabCommand(Command)\0userCommand(Command)\0"
    "battleCommand(Command)\0receiveMessage(QString)\0"
    "sendCommand(Command)\0"
};

const QMetaObject CommandAssigner::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CommandAssigner,
      qt_meta_data_CommandAssigner, 0 }
};

const QMetaObject *CommandAssigner::metaObject() const
{
    return &staticMetaObject;
}

void *CommandAssigner::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommandAssigner))
        return static_cast<void*>(const_cast< CommandAssigner*>(this));
    return QObject::qt_metacast(_clname);
}

int CommandAssigner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: serverContextStateCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 1: sendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: lobbyTabCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 3: userCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 4: battleCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 5: receiveMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: sendCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CommandAssigner::serverContextStateCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CommandAssigner::sendMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CommandAssigner::lobbyTabCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CommandAssigner::userCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CommandAssigner::battleCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
