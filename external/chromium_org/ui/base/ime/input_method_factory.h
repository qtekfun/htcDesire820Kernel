// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_FACTORY_H_
#define UI_BASE_IME_INPUT_METHOD_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/ime/input_method_initializer.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

template <typename T> struct DefaultSingletonTraits;

namespace ui {
namespace internal {
class InputMethodDelegate;
}  

class InputMethod;

class UI_EXPORT InputMethodFactory {
 public:
  
  
  
  
  static InputMethodFactory* GetInstance();

  
  
  
  
  static void SetInstance(InputMethodFactory* instance);

  virtual ~InputMethodFactory() {}

  
  virtual scoped_ptr<InputMethod> CreateInputMethod(
      internal::InputMethodDelegate* delegate,
      gfx::AcceleratedWidget widget) = 0;

 private:
  static void ClearInstance();

  friend UI_EXPORT void ShutdownInputMethod();
  friend UI_EXPORT void ShutdownInputMethodForTesting();
};

class DefaultInputMethodFactory : public InputMethodFactory {
 public:
  
  static DefaultInputMethodFactory* GetInstance();

  
  virtual scoped_ptr<InputMethod> CreateInputMethod(
      internal::InputMethodDelegate* delegate,
      gfx::AcceleratedWidget widget) OVERRIDE;

 private:
  DefaultInputMethodFactory() {}

  friend struct DefaultSingletonTraits<DefaultInputMethodFactory>;

  DISALLOW_COPY_AND_ASSIGN(DefaultInputMethodFactory);
};

class MockInputMethodFactory : public InputMethodFactory {
 public:
  
  static MockInputMethodFactory* GetInstance();

  
  virtual scoped_ptr<InputMethod> CreateInputMethod(
      internal::InputMethodDelegate* delegate,
      gfx::AcceleratedWidget widget) OVERRIDE;

 private:
  MockInputMethodFactory() {}

  friend struct DefaultSingletonTraits<MockInputMethodFactory>;

  DISALLOW_COPY_AND_ASSIGN(MockInputMethodFactory);
};

UI_EXPORT scoped_ptr<InputMethod> CreateInputMethod(
    internal::InputMethodDelegate* delegate,
    gfx::AcceleratedWidget widget);

UI_EXPORT void SetUpInputMethodFactoryForTesting();

#if defined(OS_WIN)
UI_EXPORT InputMethod* GetSharedInputMethod();

namespace internal {
void DestroySharedInputMethod();
}  
#endif

}  

#endif  
