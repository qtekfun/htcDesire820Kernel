// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_FIND_DIALOG_H_
#define CHROME_FRAME_FIND_DIALOG_H_

#include <atlbase.h>
#include <atlwin.h>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "resource.h"
#include "grit/chrome_frame_dialogs.h"

class ChromeFrameAutomationClient;

class CFFindDialog : public CDialogImpl<CFFindDialog> {
 public:
  enum { IDD = IDD_CHROME_FRAME_FIND_DIALOG };

  BEGIN_MSG_MAP(CFFindDialog)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnFind)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
  END_MSG_MAP()

  CFFindDialog();
  void Init(ChromeFrameAutomationClient* automation_client);

  LRESULT OnDestroy(UINT msg, WPARAM wparam,
                    LPARAM lparam, BOOL& handled);  
  LRESULT OnFind(WORD wNotifyCode, WORD wID,
                 HWND hWndCtl, BOOL& bHandled);  
  LRESULT OnCancel(WORD wNotifyCode, WORD wID,
                   HWND hWndCtl, BOOL& bHandled);  
  LRESULT OnInitDialog(UINT msg, WPARAM wparam,
                       LPARAM lparam, BOOL& handled);  

 private:

  
  
  
  bool InstallMessageHook();
  bool UninstallMessageHook();
  static LRESULT CALLBACK GetMsgProc(int code, WPARAM wparam, LPARAM lparam);
  static HHOOK msg_hook_;

  
  
  string16 last_find_text_;

  
  scoped_refptr<ChromeFrameAutomationClient> automation_client_;
};

#endif  
