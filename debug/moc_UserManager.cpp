/****************************************************************************
** Meta object code from reading C++ file 'UserManager.h'
**
** Created: Fri Jun 26 21:58:02 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/UserManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UserManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserManager[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   13,   12,   12, 0x08,
      59,   13,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UserManager[] = {
    "UserManager\0\0,,\0"
    "rowsAboutToBeInserted(QModelIndex,int,int)\0"
    "rowsInserted(QModelIndex,int,int)\0"
};

const QMetaObject UserManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UserManager,
      qt_meta_data_UserManager, 0 }
};

const QMetaObject *UserManager::metaObject() const
{
    return &staticMetaObject;
}

void *UserManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserManager))
        return static_cast<void*>(const_cast< UserManager*>(this));
    return QObject::qt_metacast(_clname);
}

int UserManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rowsAboutToBeInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
