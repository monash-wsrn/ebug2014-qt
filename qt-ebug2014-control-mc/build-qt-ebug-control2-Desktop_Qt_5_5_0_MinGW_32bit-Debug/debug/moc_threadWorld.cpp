/****************************************************************************
** Meta object code from reading C++ file 'threadWorld.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt-ebug2014-control2/threadWorld.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'threadWorld.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ThreadWorld_t {
    QByteArrayData data[20];
    char stringdata0[307];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThreadWorld_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThreadWorld_t qt_meta_stringdata_ThreadWorld = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ThreadWorld"
QT_MOC_LITERAL(1, 12, 19), // "uiWorldRenderUpdate"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 3), // "img"
QT_MOC_LITERAL(4, 37, 17), // "uiRobotListUpdate"
QT_MOC_LITERAL(5, 55, 10), // "listRobots"
QT_MOC_LITERAL(6, 66, 19), // "uiCommsInListUpdate"
QT_MOC_LITERAL(7, 86, 11), // "listCommsIn"
QT_MOC_LITERAL(8, 98, 20), // "uiCommsOutListUpdate"
QT_MOC_LITERAL(9, 119, 15), // "uiCommsInMsgNew"
QT_MOC_LITERAL(10, 135, 6), // "strMsg"
QT_MOC_LITERAL(11, 142, 16), // "uiCommsOutMsgNew"
QT_MOC_LITERAL(12, 159, 15), // "uiWorldEventNew"
QT_MOC_LITERAL(13, 175, 17), // "strWorldEventInfo"
QT_MOC_LITERAL(14, 193, 3), // "run"
QT_MOC_LITERAL(15, 197, 22), // "uiCommsInAddConnection"
QT_MOC_LITERAL(16, 220, 25), // "uiCommsInDeleteConnection"
QT_MOC_LITERAL(17, 246, 9), // "idCommsIn"
QT_MOC_LITERAL(18, 256, 23), // "uiCommsOutAddConnection"
QT_MOC_LITERAL(19, 280, 26) // "uiCommsOutDeleteConnection"

    },
    "ThreadWorld\0uiWorldRenderUpdate\0\0img\0"
    "uiRobotListUpdate\0listRobots\0"
    "uiCommsInListUpdate\0listCommsIn\0"
    "uiCommsOutListUpdate\0uiCommsInMsgNew\0"
    "strMsg\0uiCommsOutMsgNew\0uiWorldEventNew\0"
    "strWorldEventInfo\0run\0uiCommsInAddConnection\0"
    "uiCommsInDeleteConnection\0idCommsIn\0"
    "uiCommsOutAddConnection\0"
    "uiCommsOutDeleteConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThreadWorld[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       8,    1,   83,    2, 0x06 /* Public */,
       9,    1,   86,    2, 0x06 /* Public */,
      11,    1,   89,    2, 0x06 /* Public */,
      12,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   95,    2, 0x0a /* Public */,
      15,    0,   96,    2, 0x0a /* Public */,
      16,    1,   97,    2, 0x0a /* Public */,
      18,    0,  100,    2, 0x0a /* Public */,
      19,    1,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,

       0        // eod
};

void ThreadWorld::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThreadWorld *_t = static_cast<ThreadWorld *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->uiWorldRenderUpdate((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->uiRobotListUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->uiCommsInListUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->uiCommsOutListUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->uiCommsInMsgNew((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->uiCommsOutMsgNew((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->uiWorldEventNew((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->run(); break;
        case 8: _t->uiCommsInAddConnection(); break;
        case 9: _t->uiCommsInDeleteConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->uiCommsOutAddConnection(); break;
        case 11: _t->uiCommsOutDeleteConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ThreadWorld::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiWorldRenderUpdate)) {
                *result = 0;
            }
        }
        {
            typedef void (ThreadWorld::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiRobotListUpdate)) {
                *result = 1;
            }
        }
        {
            typedef void (ThreadWorld::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiCommsInListUpdate)) {
                *result = 2;
            }
        }
        {
            typedef void (ThreadWorld::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiCommsOutListUpdate)) {
                *result = 3;
            }
        }
        {
            typedef void (ThreadWorld::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiCommsInMsgNew)) {
                *result = 4;
            }
        }
        {
            typedef void (ThreadWorld::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiCommsOutMsgNew)) {
                *result = 5;
            }
        }
        {
            typedef void (ThreadWorld::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadWorld::uiWorldEventNew)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject ThreadWorld::staticMetaObject = {
    { &ThreadableQObject::staticMetaObject, qt_meta_stringdata_ThreadWorld.data,
      qt_meta_data_ThreadWorld,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ThreadWorld::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThreadWorld::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ThreadWorld.stringdata0))
        return static_cast<void*>(const_cast< ThreadWorld*>(this));
    return ThreadableQObject::qt_metacast(_clname);
}

int ThreadWorld::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ThreadableQObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ThreadWorld::uiWorldRenderUpdate(QImage _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ThreadWorld::uiRobotListUpdate(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ThreadWorld::uiCommsInListUpdate(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ThreadWorld::uiCommsOutListUpdate(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ThreadWorld::uiCommsInMsgNew(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ThreadWorld::uiCommsOutMsgNew(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ThreadWorld::uiWorldEventNew(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
