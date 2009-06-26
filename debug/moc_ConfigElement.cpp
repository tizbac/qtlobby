/****************************************************************************
** Meta object code from reading C++ file 'ConfigElement.h'
**
** Created: Fri Jun 26 21:58:13 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/ConfigElement.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConfigElement.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConfigElement[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x09,
      29,   14,   14,   14, 0x09,
      50,   14,   14,   14, 0x09,
      70,   68,   14,   14, 0x09,
      99,   14,   14,   14, 0x29,

       0        // eod
};

static const char qt_meta_stringdata_ConfigElement[] = {
    "ConfigElement\0\0SaveElement()\0"
    "ResetConfiguration()\0openFileBrowser()\0"
    "a\0updateExistingState(QString)\0"
    "updateExistingState()\0"
};

const QMetaObject ConfigElement::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConfigElement,
      qt_meta_data_ConfigElement, 0 }
};

const QMetaObject *ConfigElement::metaObject() const
{
    return &staticMetaObject;
}

void *ConfigElement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigElement))
        return static_cast<void*>(const_cast< ConfigElement*>(this));
    if (!strcmp(_clname, "Ui::configElement"))
        return static_cast< Ui::configElement*>(const_cast< ConfigElement*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConfigElement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SaveElement(); break;
        case 1: ResetConfiguration(); break;
        case 2: openFileBrowser(); break;
        case 3: updateExistingState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: updateExistingState(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
