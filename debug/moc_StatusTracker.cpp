/****************************************************************************
** Meta object code from reading C++ file 'StatusTracker.h'
**
** Created: Fri Jun 26 21:57:47 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/StatusTracker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StatusTracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StatusTracker[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_StatusTracker[] = {
    "StatusTracker\0"
};

const QMetaObject StatusTracker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StatusTracker,
      qt_meta_data_StatusTracker, 0 }
};

const QMetaObject *StatusTracker::metaObject() const
{
    return &staticMetaObject;
}

void *StatusTracker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatusTracker))
        return static_cast<void*>(const_cast< StatusTracker*>(this));
    return QObject::qt_metacast(_clname);
}

int StatusTracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
