// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_MODAL_NATIVE_WEB_CONTENTS_MODAL_DIALOG_MANAGER_H_
#define COMPONENTS_WEB_MODAL_NATIVE_WEB_CONTENTS_MODAL_DIALOG_MANAGER_H_

#include "components/web_modal/native_web_contents_modal_dialog.h"

namespace content {
class WebContents;
}  

namespace web_modal {

class WebContentsModalDialogHost;

class NativeWebContentsModalDialogManagerDelegate {
 public:
  NativeWebContentsModalDialogManagerDelegate() {}
  virtual ~NativeWebContentsModalDialogManagerDelegate() {}

  virtual content::WebContents* GetWebContents() const = 0;
  virtual void WillClose(NativeWebContentsModalDialog dialog) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeWebContentsModalDialogManagerDelegate);
};

class NativeWebContentsModalDialogManager {
 public:
  virtual ~NativeWebContentsModalDialogManager() {}

  
  
  
  virtual void ManageDialog(NativeWebContentsModalDialog dialog) = 0;

  
  
  virtual void ShowDialog(NativeWebContentsModalDialog dialog) = 0;

  
  virtual void HideDialog(NativeWebContentsModalDialog dialog) = 0;

  
  virtual void CloseDialog(NativeWebContentsModalDialog dialog) = 0;

  
  virtual void FocusDialog(NativeWebContentsModalDialog dialog) = 0;

  
  virtual void PulseDialog(NativeWebContentsModalDialog dialog) = 0;

  
  virtual void HostChanged(WebContentsModalDialogHost* new_host) = 0;

 protected:
  NativeWebContentsModalDialogManager() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeWebContentsModalDialogManager);
};

}  

#endif  
