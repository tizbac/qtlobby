/****************************************************************************
** Meta object code from reading C++ file 'mapElementWidget.h'
**
** Created: Fri Jun 26 21:58:11 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mapElementWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapElementWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mapElementWidget[] = {

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

static const char qt_meta_stringdata_mapElementWidget[] = {
    "mapElementWidget\0"
};

const QMetaObject mapElementWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_mapElementWidget,
      qt_meta_data_mapElementWidget, 0 }
};

const QMetaObject *mapElementWidget::metaObject() const
{
    return &staticMetaObject;
}

void *mapElementWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mapElementWidget))
        return static_cast<void*>(const_cast< mapElementWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int mapElementWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
