/****************************************************************************
** Meta object code from reading C++ file 'Users.h'
**
** Created: Fri Jun 26 21:57:56 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Users.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Users.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Users[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,    7,    6,    6, 0x05,
      42,   36,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   61,    6,    6, 0x0a,
      87,    7,    6,    6, 0x0a,
     115,  111,    6,    6, 0x0a,
     156,  138,    6,    6, 0x0a,
     191,    6,    6,    6, 0x0a,
     199,  197,    6,    6, 0x0a,
     236,  228,    6,    6, 0x0a,
     268,  262,    6,    6, 0x09,
     313,  307,    6,    6, 0x09,
     344,  197,    6,    6, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Users[] = {
    "Users\0\0command\0sendCommand(Command)\0"
    "input\0sendInput(QString)\0regExp\0"
    "setRegExp(QString)\0receiveCommand(Command)\0"
    "url\0setConfiguration(QUrl)\0name,lobbyTabType\0"
    "currentTabChanged(QString,QString)\0"
    "inv()\0u\0onMyBattleStateChanged(User)\0"
    "isReady\0onReadyStateChanged(bool)\0"
    "point\0customContextMenuRequestedSlot(QPoint)\0"
    "index\0doubleClickedSlot(QModelIndex)\0"
    "joinSameBattle(User)\0"
};

const QMetaObject Users::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_Users,
      qt_meta_data_Users, 0 }
};

const QMetaObject *Users::metaObject() const
{
    return &staticMetaObject;
}

void *Users::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Users))
        return static_cast<void*>(const_cast< Users*>(this));
    return QTreeView::qt_metacast(_clname);
}

int Users::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 1: sendInput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: setRegExp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: receiveCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 4: setConfiguration((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 5: currentTabChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: inv(); break;
        case 7: onMyBattleStateChanged((*reinterpret_cast< User(*)>(_a[1]))); break;
        case 8: onReadyStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: customContextMenuRequestedSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 10: doubleClickedSlot((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 11: joinSameBattle((*reinterpret_cast< User(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Users::sendCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Users::sendInput(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
