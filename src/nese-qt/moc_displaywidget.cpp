/****************************************************************************
** Meta object code from reading C++ file 'displaywidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "displaywidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'displaywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtFrontend__DisplayWidget_t
{
  QByteArrayData data[6];
  char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len)                                                                                  \
  Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(                                                             \
    len, qptrdiff(offsetof(qt_meta_stringdata_QtFrontend__DisplayWidget_t, stringdata0) + ofs -                        \
                  idx * sizeof(QByteArrayData)))
static const qt_meta_stringdata_QtFrontend__DisplayWidget_t qt_meta_stringdata_QtFrontend__DisplayWidget = {
  {
    QT_MOC_LITERAL(0, 0, 25),  // "QtFrontend::DisplayWidget"
    QT_MOC_LITERAL(1, 26, 12), // "onKeyPressed"
    QT_MOC_LITERAL(2, 39, 0),  // ""
    QT_MOC_LITERAL(3, 40, 10), // "QKeyEvent*"
    QT_MOC_LITERAL(4, 51, 5),  // "event"
    QT_MOC_LITERAL(5, 57, 13)  // "onKeyReleased"

  },
  "QtFrontend::DisplayWidget\0onKeyPressed\0"
  "\0QKeyEvent*\0event\0onKeyReleased"};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtFrontend__DisplayWidget[] = {

  // content:
  7,     // revision
  0,     // classname
  0, 0,  // classinfo
  2, 14, // methods
  0, 0,  // properties
  0, 0,  // enums/sets
  0, 0,  // constructors
  0,     // flags
  2,     // signalCount

  // signals: name, argc, parameters, tag, flags
  1, 1, 24, 2, 0x06 /* Public */, 5, 1, 27, 2, 0x06 /* Public */,

  // signals: parameters
  QMetaType::Void, 0x80000000 | 3, 4, QMetaType::Void, 0x80000000 | 3, 4,

  0 // eod
};

void QtFrontend::DisplayWidget::qt_static_metacall(QObject* _o, QMetaObject::Call _c, int _id, void** _a)
{
  if (_c == QMetaObject::InvokeMetaMethod)
  {
    Q_ASSERT(staticMetaObject.cast(_o));
    DisplayWidget* _t = static_cast<DisplayWidget*>(_o);
    Q_UNUSED(_t)
    switch (_id)
    {
      case 0:
        _t->onKeyPressed((*reinterpret_cast<QKeyEvent*(*)>(_a[1])));
        break;
      case 1:
        _t->onKeyReleased((*reinterpret_cast<QKeyEvent*(*)>(_a[1])));
        break;
      default:;
    }
  }
  else if (_c == QMetaObject::IndexOfMethod)
  {
    int* result = reinterpret_cast<int*>(_a[0]);
    void** func = reinterpret_cast<void**>(_a[1]);
    {
      typedef void (DisplayWidget::*_t)(QKeyEvent*);
      if (*reinterpret_cast<_t*>(func) == static_cast<_t>(&DisplayWidget::onKeyPressed))
      {
        *result = 0;
        return;
      }
    }
    {
      typedef void (DisplayWidget::*_t)(QKeyEvent*);
      if (*reinterpret_cast<_t*>(func) == static_cast<_t>(&DisplayWidget::onKeyReleased))
      {
        *result = 1;
        return;
      }
    }
  }
}

const QMetaObject QtFrontend::DisplayWidget::staticMetaObject = {
  {&QGLWidget::staticMetaObject, qt_meta_stringdata_QtFrontend__DisplayWidget.data,
   qt_meta_data_QtFrontend__DisplayWidget, qt_static_metacall, Q_NULLPTR, Q_NULLPTR}};

const QMetaObject* QtFrontend::DisplayWidget::metaObject() const
{
  return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void* QtFrontend::DisplayWidget::qt_metacast(const char* _clname)
{
  if (!_clname)
    return Q_NULLPTR;
  if (!strcmp(_clname, qt_meta_stringdata_QtFrontend__DisplayWidget.stringdata0))
    return static_cast<void*>(const_cast<DisplayWidget*>(this));
  if (!strcmp(_clname, "Display"))
    return static_cast<Display*>(const_cast<DisplayWidget*>(this));
  return QGLWidget::qt_metacast(_clname);
}

int QtFrontend::DisplayWidget::qt_metacall(QMetaObject::Call _c, int _id, void** _a)
{
  _id = QGLWidget::qt_metacall(_c, _id, _a);
  if (_id < 0)
    return _id;
  if (_c == QMetaObject::InvokeMetaMethod)
  {
    if (_id < 2)
      qt_static_metacall(this, _c, _id, _a);
    _id -= 2;
  }
  else if (_c == QMetaObject::RegisterMethodArgumentMetaType)
  {
    if (_id < 2)
      *reinterpret_cast<int*>(_a[0]) = -1;
    _id -= 2;
  }
  return _id;
}

// SIGNAL 0
void QtFrontend::DisplayWidget::onKeyPressed(QKeyEvent* _t1)
{
  void* _a[] = {Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1))};
  QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtFrontend::DisplayWidget::onKeyReleased(QKeyEvent* _t1)
{
  void* _a[] = {Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1))};
  QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
