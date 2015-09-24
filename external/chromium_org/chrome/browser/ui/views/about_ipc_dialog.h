// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ABOUT_IPC_DIALOG_H_
#define CHROME_BROWSER_UI_VIEWS_ABOUT_IPC_DIALOG_H_

#include "ipc/ipc_message.h"  

#if defined(OS_WIN) && defined(IPC_MESSAGE_LOG_ENABLED)

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlwin.h>

#include "ipc/ipc_logging.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/dialog_delegate.h"

template <typename T> struct DefaultSingletonTraits;

namespace views {
class LabelButton;
class NativeViewHost;
}

class AboutIPCDialog : public views::DialogDelegateView,
                       public views::ButtonListener,
                       public IPC::Logging::Consumer {
 public:
  
  
  static void RunDialog();

  virtual ~AboutIPCDialog();

 private:
  friend struct DefaultSingletonTraits<AboutIPCDialog>;

  AboutIPCDialog();

  
  void SetupControls();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetDialogButtons() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual void Log(const IPC::LogData& data) OVERRIDE;

  
  virtual bool CanResize() const OVERRIDE;
  virtual bool UseNewStyleForThisDialog() const OVERRIDE;

  
  virtual void ButtonPressed(views::Button* button,
                             const ui::Event& event) OVERRIDE;

  WTL::CListViewCtrl message_list_;

  views::LabelButton* track_toggle_;
  views::LabelButton* clear_button_;
  views::LabelButton* filter_button_;
  views::NativeViewHost* table_;

  
  bool tracking_;

  DISALLOW_COPY_AND_ASSIGN(AboutIPCDialog);
};

#endif  

#endif  
