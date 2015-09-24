// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_INPUT_WINDOW_DIALOG_H_
#define CHROME_BROWSER_UI_INPUT_WINDOW_DIALOG_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"

class InputWindowDialog {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual bool IsValid(const std::wstring& text) = 0;

    
    virtual void InputAccepted(const std::wstring& text) = 0;

    
    virtual void InputCanceled() = 0;
  };

  
  
  static InputWindowDialog* Create(gfx::NativeWindow parent,
                                   const std::wstring& window_title,
                                   const std::wstring& label,
                                   const std::wstring& contents,
                                   Delegate* delegate);

  
  virtual void Show() = 0;

  
  virtual void Close() = 0;

 protected:
  InputWindowDialog() {}
  virtual ~InputWindowDialog() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(InputWindowDialog);
};

#endif  
