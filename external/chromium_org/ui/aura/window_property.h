// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_PROPERTY_H_
#define UI_AURA_WINDOW_PROPERTY_H_

#include "base/basictypes.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/window.h"


namespace aura {
namespace {

template<typename T>
class WindowPropertyCaster {
 public:
  static int64 ToInt64(T x) { return static_cast<int64>(x); }
  static T FromInt64(int64 x) { return static_cast<T>(x); }
};
template<typename T>
class WindowPropertyCaster<T*> {
 public:
  static int64 ToInt64(T* x) { return reinterpret_cast<int64>(x); }
  static T* FromInt64(int64 x) { return reinterpret_cast<T*>(x); }
};
template<>
class WindowPropertyCaster<bool> {
 public:
  static int64 ToInt64(bool x) { return static_cast<int64>(x); }
  static bool FromInt64(int64 x) { return x != 0; }
};

}  

template<typename T>
struct WindowProperty {
  T default_value;
  const char* name;
  Window::PropertyDeallocator deallocator;
};

template<typename T>
void Window::SetProperty(const WindowProperty<T>* property, T value) {
  int64 old = SetPropertyInternal(
      property,
      property->name,
      value == property->default_value ? NULL : property->deallocator,
      WindowPropertyCaster<T>::ToInt64(value),
      WindowPropertyCaster<T>::ToInt64(property->default_value));
  if (property->deallocator &&
      old != WindowPropertyCaster<T>::ToInt64(property->default_value)) {
    (*property->deallocator)(old);
  }
}

template<typename T>
T Window::GetProperty(const WindowProperty<T>* property) const {
  return WindowPropertyCaster<T>::FromInt64(GetPropertyInternal(
      property, WindowPropertyCaster<T>::ToInt64(property->default_value)));
}

template<typename T>
void Window::ClearProperty(const WindowProperty<T>* property) {
  SetProperty(property, property->default_value);
}

}  

#define DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(EXPORT, T)  \
    template EXPORT void aura::Window::SetProperty(         \
        const aura::WindowProperty<T >*, T);                \
    template EXPORT T aura::Window::GetProperty(            \
        const aura::WindowProperty<T >*) const;             \
    template EXPORT void aura::Window::ClearProperty(       \
        const aura::WindowProperty<T >*);
#define DECLARE_WINDOW_PROPERTY_TYPE(T)  \
    DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(, T)

#define DEFINE_WINDOW_PROPERTY_KEY(TYPE, NAME, DEFAULT) \
  COMPILE_ASSERT(sizeof(TYPE) <= sizeof(int64), property_type_too_large);     \
  namespace {                                                                 \
    const aura::WindowProperty<TYPE> NAME ## _Value = {DEFAULT, #NAME, NULL}; \
  }                                                                           \
  const aura::WindowProperty<TYPE>* const NAME = & NAME ## _Value;

#define DEFINE_LOCAL_WINDOW_PROPERTY_KEY(TYPE, NAME, DEFAULT) \
  COMPILE_ASSERT(sizeof(TYPE) <= sizeof(int64), property_type_too_large);     \
  namespace {                                                                 \
    const aura::WindowProperty<TYPE> NAME ## _Value = {DEFAULT, #NAME, NULL}; \
    const aura::WindowProperty<TYPE>* const NAME = & NAME ## _Value;          \
  }

#define DEFINE_OWNED_WINDOW_PROPERTY_KEY(TYPE, NAME, DEFAULT)   \
  namespace {                                                   \
    void Deallocator ## NAME (int64 p) {                        \
      enum { type_must_be_complete = sizeof(TYPE) };            \
      delete aura::WindowPropertyCaster<TYPE*>::FromInt64(p); \
    }                                                           \
    const aura::WindowProperty<TYPE*> NAME ## _Value =          \
        {DEFAULT,#NAME,&Deallocator ## NAME};                   \
  }                                                             \
  const aura::WindowProperty<TYPE*>* const NAME = & NAME ## _Value;

#endif  
