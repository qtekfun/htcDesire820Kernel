// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_REMOTE_INPUT_METHOD_WIN_H_
#define UI_BASE_IME_REMOTE_INPUT_METHOD_WIN_H_

#include <Windows.h>

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

namespace ui {
namespace internal {
class InputMethodDelegate;
class RemoteInputMethodDelegateWin;
}  

class InputMethod;
struct CompositionText;


bool IsRemoteInputMethodWinRequired(gfx::AcceleratedWidget widget);

UI_EXPORT scoped_ptr<InputMethod> CreateRemoteInputMethodWin(
    internal::InputMethodDelegate* delegate);

class UI_EXPORT RemoteInputMethodPrivateWin {
 public:
  RemoteInputMethodPrivateWin();

  
  
  
  
  
  static RemoteInputMethodPrivateWin* Get(InputMethod* input_method);

  
  
  virtual void SetRemoteDelegate(
      internal::RemoteInputMethodDelegateWin* delegate) = 0;

  
  
  
  virtual void OnCandidatePopupChanged(bool visible) = 0;

  
  
  
  
  virtual void OnInputSourceChanged(LANGID langid, bool is_ime) = 0;

  
  
  
  
  virtual void OnCompositionChanged(
      const CompositionText& composition_text) = 0;

  
  
  
  
  
  
  virtual void OnTextCommitted(const base::string16& text) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(RemoteInputMethodPrivateWin);
};

}  

#endif  
