/****************************************************************************
** Meta object code from reading C++ file 'AudioBackend.h'
**
** Created: Fri Jun 26 21:58:08 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/AudioBackend.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioBackend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AudioBackend[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   14,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AudioBackend[] = {
    "AudioBackend\0\0s\0playSample(SampleCollection)\0"
};

const QMetaObject AudioBackend::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AudioBackend,
      qt_meta_data_AudioBackend, 0 }
};

const QMetaObject *AudioBackend::metaObject() const
{
    return &staticMetaObject;
}

void *AudioBackend::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AudioBackend))
        return static_cast<void*>(const_cast< AudioBackend*>(this));
    return QObject::qt_metacast(_clname);
}

int AudioBackend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: playSample((*reinterpret_cast< SampleCollection(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
