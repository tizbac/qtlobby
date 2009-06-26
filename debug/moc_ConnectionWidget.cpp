/****************************************************************************
** Meta object code from reading C++ file 'ConnectionWidget.h'
**
** Created: Fri Jun 26 21:57:49 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/ConnectionWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConnectionWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConnectionWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      42,   17,   17,   17, 0x05,
      74,   65,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   17,   17,   17, 0x0a,
     116,   17,   17,   17, 0x0a,
     153,   17,   17,   17, 0x0a,
     189,   17,   17,   17, 0x0a,
     229,  219,   17,   17, 0x0a,
     266,   17,   17,   17, 0x0a,
     284,   17,   17,   17, 0x0a,
     306,   17,   17,   17, 0x0a,
     332,  326,   17,   17, 0x0a,
     365,   17,   17,   17, 0x0a,
     384,   17,   17,   17, 0x0a,
     406,   17,   17,   17, 0x0a,
     427,   17,   17,   17, 0x0a,
     447,   17,   17,   17, 0x0a,
     468,   17,   17,   17, 0x0a,
     489,   17,   17,   17, 0x0a,
     529,   17,   17,   17, 0x0a,
     569,   17,   17,   17, 0x0a,
     600,   17,   17,   17, 0x0a,
     637,  631,   17,   17, 0x08,
     678,   17,   17,   17, 0x08,
     696,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConnectionWidget[] = {
    "ConnectionWidget\0\0emitConfiguration(QUrl)\0"
    "establishConnection_()\0username\0"
    "usernameChanged(QString)\0show_if_wanted()\0"
    "closeDialogCheckBoxValueChanged(int)\0"
    "openDialogCheckBoxValueChanged(int)\0"
    "radioButtonValueChanged(bool)\0index,url\0"
    "modifyServerProfile(signed int,QUrl)\0"
    "logWrite(QString)\0establishConnection()\0"
    "addDefaultServers()\0index\0"
    "comboBoxCurrentIndexChanged(int)\0"
    "updateComboBoxes()\0saveModifiedProfile()\0"
    "openProfileWizzard()\0openProfileEditor()\0"
    "delSelectedProfile()\0registerNewAccount()\0"
    "renameLoginNameFeedbackSuccess(QString)\0"
    "renameLoginNameFeedbackFailure(QString)\0"
    "changePasswordSuccess(QString)\0"
    "changePasswordFailure(QString)\0state\0"
    "connectionStatusChanged(ConnectionState)\0"
    "renameLoginName()\0changePassword()\0"
};

const QMetaObject ConnectionWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ConnectionWidget,
      qt_meta_data_ConnectionWidget, 0 }
};

const QMetaObject *ConnectionWidget::metaObject() const
{
    return &staticMetaObject;
}

void *ConnectionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionWidget))
        return static_cast<void*>(const_cast< ConnectionWidget*>(this));
    if (!strcmp(_clname, "Ui::connectionWidget"))
        return static_cast< Ui::connectionWidget*>(const_cast< ConnectionWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int ConnectionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emitConfiguration((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 1: establishConnection_(); break;
        case 2: usernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: show_if_wanted(); break;
        case 4: closeDialogCheckBoxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: openDialogCheckBoxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: radioButtonValueChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: modifyServerProfile((*reinterpret_cast< signed int(*)>(_a[1])),(*reinterpret_cast< QUrl(*)>(_a[2]))); break;
        case 8: logWrite((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: establishConnection(); break;
        case 10: addDefaultServers(); break;
        case 11: comboBoxCurrentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: updateComboBoxes(); break;
        case 13: saveModifiedProfile(); break;
        case 14: openProfileWizzard(); break;
        case 15: openProfileEditor(); break;
        case 16: delSelectedProfile(); break;
        case 17: registerNewAccount(); break;
        case 18: renameLoginNameFeedbackSuccess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: renameLoginNameFeedbackFailure((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: changePasswordSuccess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: changePasswordFailure((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: connectionStatusChanged((*reinterpret_cast< ConnectionState(*)>(_a[1]))); break;
        case 23: renameLoginName(); break;
        case 24: changePassword(); break;
        default: ;
        }
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void ConnectionWidget::emitConfiguration(QUrl _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConnectionWidget::establishConnection_()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ConnectionWidget::usernameChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
