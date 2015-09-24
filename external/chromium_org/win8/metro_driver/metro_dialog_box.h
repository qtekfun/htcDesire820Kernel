// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_METRO_DRIVER_METRO_DIALOG_BOX_H_
#define CHROME_BROWSER_UI_METRO_DRIVER_METRO_DIALOG_BOX_H_

#include <windows.ui.popups.h>
#include <string>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/win/metro.h"

class MetroDialogBox : public winui::Popups::IUICommandInvokedHandler {
 public:
  struct DialogBoxInfo {
    string16 title;
    string16 content;
    string16 button1_label;
    string16 button2_label;
    base::win::MetroDialogButtonPressedHandler button1_handler;
    base::win::MetroDialogButtonPressedHandler button2_handler;
  };

  MetroDialogBox();
  ~MetroDialogBox();

  
  void Show(const DialogBoxInfo& dialog_box_info);

  
  void Dismiss();

  
  
  
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** object) {
    DVLOG(1) << __FUNCTION__;
    CHECK(false);
    return E_NOINTERFACE;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) {
    DVLOG(1) << __FUNCTION__;
    return 1;
  }

  virtual ULONG STDMETHODCALLTYPE Release(void) {
    DVLOG(1) << __FUNCTION__;
    return 1;
  }

  virtual HRESULT STDMETHODCALLTYPE Invoke(winui::Popups::IUICommand* command);

 private:
  
  mswr::ComPtr<winui::Popups::IMessageDialog> dialog_box_;
  DialogBoxInfo dialog_box_info_;
};

#endif  

