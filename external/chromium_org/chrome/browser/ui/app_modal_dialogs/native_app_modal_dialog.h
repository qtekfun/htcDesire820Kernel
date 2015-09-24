// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_NATIVE_APP_MODAL_DIALOG_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_NATIVE_APP_MODAL_DIALOG_H_

#include "ui/gfx/native_widget_types.h"

class JavaScriptAppModalDialog;

class NativeAppModalDialog {
 public:
  
  
  virtual int GetAppModalDialogButtons() const = 0;

  
  virtual void ShowAppModalDialog() = 0;

  
  virtual void ActivateAppModalDialog() = 0;

  
  virtual void CloseAppModalDialog() = 0;

  
  virtual void AcceptAppModalDialog() = 0;
  virtual void CancelAppModalDialog() = 0;

  
  static NativeAppModalDialog* CreateNativeJavaScriptPrompt(
      JavaScriptAppModalDialog* dialog,
      gfx::NativeWindow parent_window);
};

#endif  

