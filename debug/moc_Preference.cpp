/****************************************************************************
** Meta object code from reading C++ file 'Preference.h'
**
** Created: Fri Jun 26 21:58:08 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Preference.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Preference.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserPreference[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      34,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   15,   15,   15, 0x08,
      65,   15,   15,   15, 0x08,
      80,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UserPreference[] = {
    "UserPreference\0\0SaveAllElements()\0"
    "ResetAllElements()\0okClicked()\0"
    "applyClicked()\0cancelClicked()\0"
};

const QMetaObject UserPreference::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_UserPreference,
      qt_meta_data_UserPreference, 0 }
};

const QMetaObject *UserPreference::metaObject() const
{
    return &staticMetaObject;
}

void *UserPreference::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserPreference))
        return static_cast<void*>(const_cast< UserPreference*>(this));
    return QDialog::qt_metacast(_clname);
}

int UserPreference::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SaveAllElements(); break;
        case 1: ResetAllElements(); break;
        case 2: okClicked(); break;
        case 3: applyClicked(); break;
        case 4: cancelClicked(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void UserPreference::SaveAllElements()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void UserPreference::ResetAllElements()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
