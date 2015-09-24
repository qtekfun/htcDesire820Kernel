// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TURNDOWN_PROMPT_TURNDOWN_PROMPT_WINDOW_H_
#define CHROME_FRAME_TURNDOWN_PROMPT_TURNDOWN_PROMPT_WINDOW_H_

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlframe.h>
#include <atltheme.h>
#include <atlwin.h>

#include "base/debug/debugger.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_handle.h"
#include "chrome_frame/infobars/infobar_content.h"
#include "chrome_frame/resource.h"
#include "grit/chrome_frame_dialogs.h"

class UrlLauncher;

namespace WTL {
class CHyperLink;
}  

class TurndownPromptWindow
    : public CDialogImpl<TurndownPromptWindow, CWindow>,
      public CDialogResize<TurndownPromptWindow>,
      public CThemeImpl<TurndownPromptWindow> {
 public:
  enum { IDD = IDD_CHROME_FRAME_TURNDOWN_PROMPT };

  
  
  
  
  
  
  
  
  
  
  static base::WeakPtr<TurndownPromptWindow> CreateInstance(
      InfobarContent::Frame* frame,
      UrlLauncher* url_launcher,
      const base::Closure& uninstall_closure);

  BEGIN_MSG_MAP(InfobarWindow)
    CHAIN_MSG_MAP(CThemeImpl<TurndownPromptWindow>)
    MSG_WM_DESTROY(OnDestroy)
    MSG_WM_INITDIALOG(OnInitDialog)
    NOTIFY_HANDLER(IDC_TD_PROMPT_LINK, NM_CLICK, OnLearnMore)
    COMMAND_HANDLER(IDUNINSTALL, BN_CLICKED, OnUninstall)
    CHAIN_MSG_MAP(CDialogResize<TurndownPromptWindow>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(InfobarWindow)
    DLGRESIZE_CONTROL(IDUNINSTALL, DLSZ_CENTER_Y | DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_TD_PROMPT_LINK, DLSZ_CENTER_Y | DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_TD_PROMPT_MESSAGE, DLSZ_SIZE_Y | DLSZ_SIZE_X)
  END_DLGRESIZE_MAP()

  virtual void OnFinalMessage(HWND);

 private:
  
  TurndownPromptWindow(InfobarContent::Frame* frame,
                       UrlLauncher* url_launcher,
                       const base::Closure& uninstall_closure);

  
  virtual ~TurndownPromptWindow();

  
  void OnDestroy();
  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  LRESULT OnLearnMore(WORD wParam, LPNMHDR lParam, BOOL& bHandled);  
  LRESULT OnUninstall(WORD wNotifyCode,
                      WORD wID,
                      HWND hWndCtl,
                      BOOL& bHandled);

  
  static string16 GetPromptText();

  InfobarContent::Frame* frame_;  
  scoped_ptr<WTL::CHyperLink> link_;
  scoped_ptr<UrlLauncher> url_launcher_;
  base::Closure uninstall_closure_;

  base::WeakPtrFactory<TurndownPromptWindow> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(TurndownPromptWindow);
};  

#endif  
