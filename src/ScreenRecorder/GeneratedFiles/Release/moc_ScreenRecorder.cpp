/****************************************************************************
** Meta object code from reading C++ file 'ScreenRecorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ScreenRecorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScreenRecorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ScreenRecorder_t {
    QByteArrayData data[11];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScreenRecorder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScreenRecorder_t qt_meta_stringdata_ScreenRecorder = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ScreenRecorder"
QT_MOC_LITERAL(1, 15, 18), // "checkVideoIsRecord"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 18), // "checkAudioIsRecord"
QT_MOC_LITERAL(4, 54, 16), // "checkLogIsRecord"
QT_MOC_LITERAL(5, 71, 17), // "checkFileIsRecord"
QT_MOC_LITERAL(6, 89, 17), // "checkRtmpIsRecord"
QT_MOC_LITERAL(7, 107, 6), // "record"
QT_MOC_LITERAL(8, 114, 5), // "close"
QT_MOC_LITERAL(9, 120, 11), // "setLogLevel"
QT_MOC_LITERAL(10, 132, 14) // "setVideoConfig"

    },
    "ScreenRecorder\0checkVideoIsRecord\0\0"
    "checkAudioIsRecord\0checkLogIsRecord\0"
    "checkFileIsRecord\0checkRtmpIsRecord\0"
    "record\0close\0setLogLevel\0setVideoConfig"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScreenRecorder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    0,   61,    2, 0x0a /* Public */,
       5,    0,   62,    2, 0x0a /* Public */,
       6,    0,   63,    2, 0x0a /* Public */,
       7,    0,   64,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    0,   66,    2, 0x0a /* Public */,
      10,    0,   67,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ScreenRecorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScreenRecorder *_t = static_cast<ScreenRecorder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->checkVideoIsRecord(); break;
        case 1: _t->checkAudioIsRecord(); break;
        case 2: _t->checkLogIsRecord(); break;
        case 3: _t->checkFileIsRecord(); break;
        case 4: _t->checkRtmpIsRecord(); break;
        case 5: _t->record(); break;
        case 6: { bool _r = _t->close();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->setLogLevel(); break;
        case 8: _t->setVideoConfig(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ScreenRecorder::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ScreenRecorder.data,
    qt_meta_data_ScreenRecorder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ScreenRecorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenRecorder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScreenRecorder.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenRecorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
