// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "build/build_config.h"

class NativeAppModalDialog;
class TabContents;

class AppModalDialog {
 public:
  
  
  
  AppModalDialog(TabContents* tab_contents, const std::wstring& title);
  virtual ~AppModalDialog();

  
  void ShowModalDialog();

  
  void ActivateModalDialog();

  
  void CloseModalDialog();

  
  
  void CompleteDialog();

  
  std::wstring title() const { return title_; }

  NativeAppModalDialog* native_dialog() const { return native_dialog_; }

  

  
  
  
  
  
  virtual void CreateAndShowDialog();

  
  
  
  
  virtual bool IsValid();

 protected:
  
  virtual NativeAppModalDialog* CreateNativeDialog() = 0;

  
  
  bool skip_this_dialog_;

  
  TabContents* tab_contents_;

  
  NativeAppModalDialog* native_dialog_;

 private:
  
  std::wstring title_;

  DISALLOW_COPY_AND_ASSIGN(AppModalDialog);
};

#endif  
