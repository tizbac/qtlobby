/****************************************************************************
** Meta object code from reading C++ file 'Battles.h'
**
** Created: Fri Jun 26 21:58:05 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Battles.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Battles.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Battles[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,    9,    8,    8, 0x05,
      40,   38,    8,    8, 0x05,
      69,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      84,   77,    8,    8, 0x0a,
     103,    9,    8,    8, 0x0a,
     131,  127,    8,    8, 0x0a,
     166,  154,    8,    8, 0x0a,
     207,  198,    8,    8, 0x0a,
     241,  235,    8,    8, 0x09,
     282,  276,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Battles[] = {
    "Battles\0\0command\0sendCommand(Command)\0"
    ",\0wantJoinBattle(uint,QString)\0start()\0"
    "regExp\0setRegExp(QString)\0"
    "receiveCommand(Command)\0url\0"
    "setConfiguration(QUrl)\0id,password\0"
    "joinBattleCommand(uint,QString)\0"
    "username\0setCurrentUsername(QString)\0"
    "point\0customContextMenuRequested(QPoint)\0"
    "index\0doubleClicked(QModelIndex)\0"
};

const QMetaObject Battles::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_Battles,
      qt_meta_data_Battles, 0 }
};

const QMetaObject *Battles::metaObject() const
{
    return &staticMetaObject;
}

void *Battles::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Battles))
        return static_cast<void*>(const_cast< Battles*>(this));
    return QTreeView::qt_metacast(_clname);
}

int Battles::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 1: wantJoinBattle((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: start(); break;
        case 3: setRegExp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: receiveCommand((*reinterpret_cast< Command(*)>(_a[1]))); break;
        case 5: setConfiguration((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 6: joinBattleCommand((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: setCurrentUsername((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Battles::sendCommand(Command _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Battles::wantJoinBattle(unsigned int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Battles::start()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
