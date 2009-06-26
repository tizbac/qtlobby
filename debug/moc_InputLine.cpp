/****************************************************************************
** Meta object code from reading C++ file 'InputLine.h'
**
** Created: Fri Jun 26 21:57:59 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/InputLine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InputLine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InputLine[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   36,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_InputLine[] = {
    "InputLine\0\0input\0sendInput(QString)\0"
    "event\0keyPressEvent(QKeyEvent*)\0"
};

const QMetaObject InputLine::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_InputLine,
      qt_meta_data_InputLine, 0 }
};

const QMetaObject *InputLine::metaObject() const
{
    return &staticMetaObject;
}

void *InputLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InputLine))
        return static_cast<void*>(const_cast< InputLine*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int InputLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendInput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void InputLine::sendInput(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
