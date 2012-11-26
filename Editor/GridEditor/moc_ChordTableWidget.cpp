/****************************************************************************
** Meta object code from reading C++ file 'ChordTableWidget.h'
**
** Created: Mon Nov 26 15:31:08 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ChordTableWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChordTableWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChordTableWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   18,   17,   17, 0x0a,
      68,   17,   17,   17, 0x0a,
      81,   17,   17,   17, 0x0a,
     103,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ChordTableWidget[] = {
    "ChordTableWidget\0\0chord,column\0"
    "fill_selection(QTreeWidgetItem*,int)\0"
    "insert_row()\0delete_selected_row()\0"
    "copy_down_rows()\0"
};

const QMetaObject ChordTableWidget::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_ChordTableWidget,
      qt_meta_data_ChordTableWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChordTableWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChordTableWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChordTableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChordTableWidget))
        return static_cast<void*>(const_cast< ChordTableWidget*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int ChordTableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fill_selection((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: insert_row(); break;
        case 2: delete_selected_row(); break;
        case 3: copy_down_rows(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
