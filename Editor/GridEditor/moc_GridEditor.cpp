/****************************************************************************
** Meta object code from reading C++ file 'GridEditor.h'
**
** Created: Mon Nov 26 15:56:10 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GridEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GridEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GridEditor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      26,   11,   11,   11, 0x0a,
      38,   11,   11,   11, 0x0a,
      50,   11,   11,   11, 0x0a,
      60,   11,   11,   11, 0x0a,
      69,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GridEditor[] = {
    "GridEditor\0\0changeState()\0importXml()\0"
    "exportXml()\0newGrid()\0rename()\0"
    "newEditor(int)\0"
};

const QMetaObject GridEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GridEditor,
      qt_meta_data_GridEditor, 0 }
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
    return QMainWindow::qt_metacast(_clname);
}

int GridEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeState(); break;
        case 1: importXml(); break;
        case 2: exportXml(); break;
        case 3: newGrid(); break;
        case 4: rename(); break;
        case 5: newEditor((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
