/****************************************************************************
** Meta object code from reading C++ file 'GridEditor.h'
**
** Created: Mon Nov 12 18:41:38 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GridEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GridEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GridEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,
      40,   11,   11,   11, 0x0a,
      53,   11,   11,   11, 0x0a,
      64,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GridEditor[] = {
    "GridEditor\0\0change_state()\0import_xml()\0"
    "export_xml()\0new_grid()\0rename()\0"
};

void GridEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GridEditor *_t = static_cast<GridEditor *>(_o);
        switch (_id) {
        case 0: _t->change_state(); break;
        case 1: _t->import_xml(); break;
        case 2: _t->export_xml(); break;
        case 3: _t->new_grid(); break;
        case 4: _t->rename(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData GridEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GridEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GridEditor,
      qt_meta_data_GridEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GridEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GridEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GridEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GridEditor))
        return static_cast<void*>(const_cast< GridEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int GridEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
