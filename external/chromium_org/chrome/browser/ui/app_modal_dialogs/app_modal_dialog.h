// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "build/build_config.h"

class NativeAppModalDialog;

namespace content {
class WebContents;
}

class AppModalDialog {
 public:
  
  
  AppModalDialog(content::WebContents* web_contents,
                 const base::string16& title);
  virtual ~AppModalDialog();

  
  void ShowModalDialog();

  
  void ActivateModalDialog();

  
  void CloseModalDialog();

  
  
  void CompleteDialog();

  base::string16 title() const { return title_; }
  NativeAppModalDialog* native_dialog() const { return native_dialog_; }
  content::WebContents* web_contents() const { return web_contents_; }

  
  
  
  
  
  void CreateAndShowDialog();

  
  
  
  
  bool IsValid();

  

  
  
  virtual void Invalidate();

  
  
  virtual bool IsJavaScriptModalDialog();

 protected:
  
  virtual NativeAppModalDialog* CreateNativeDialog() = 0;

 private:
  
  base::string16 title_;

  
  bool completed_;

  
  
  bool valid_;

  
  NativeAppModalDialog* native_dialog_;

  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(AppModalDialog);
};

#endif  
