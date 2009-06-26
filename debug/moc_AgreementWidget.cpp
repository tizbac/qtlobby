/****************************************************************************
** Meta object code from reading C++ file 'AgreementWidget.h'
**
** Created: Fri Jun 26 21:57:58 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/AgreementWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AgreementWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AgreementWidget[] = {

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

static const char qt_meta_stringdata_AgreementWidget[] = {
    "AgreementWidget\0"
};

const QMetaObject AgreementWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AgreementWidget,
      qt_meta_data_AgreementWidget, 0 }
};

const QMetaObject *AgreementWidget::metaObject() const
{
    return &staticMetaObject;
}

void *AgreementWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AgreementWidget))
        return static_cast<void*>(const_cast< AgreementWidget*>(this));
    if (!strcmp(_clname, "Ui::agreementWidget"))
        return static_cast< Ui::agreementWidget*>(const_cast< AgreementWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int AgreementWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
