/****************************************************************************
** Meta object code from reading C++ file 'PhononBackend.h'
**
** Created: Thu Jun 25 06:15:09 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/PhononBackend.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PhononBackend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhononBackend[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      33,   15,   14,   14, 0x08,
      75,   15,   14,   14, 0x08,
     121,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PhononBackend[] = {
    "PhononBackend\0\0newState,oldState\0"
    "stateChanged(Phonon::State,Phonon::State)\0"
    "metaStateChanged(Phonon::State,Phonon::State)\0"
    "play()\0"
};

const QMetaObject PhononBackend::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PhononBackend,
      qt_meta_data_PhononBackend, 0 }
};

const QMetaObject *PhononBackend::metaObject() const
{
    return &staticMetaObject;
}

void *PhononBackend::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhononBackend))
        return static_cast<void*>(const_cast< PhononBackend*>(this));
    return QObject::qt_metacast(_clname);
}

int PhononBackend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 1: metaStateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 2: play(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
