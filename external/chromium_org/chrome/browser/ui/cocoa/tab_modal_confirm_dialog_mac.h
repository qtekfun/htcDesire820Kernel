// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_MODAL_CONFIRM_DIALOG_MAC_H_
#define CHROME_BROWSER_UI_COCOA_TAB_MODAL_CONFIRM_DIALOG_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/cocoa/constrained_window/constrained_window_mac.h"
#include "chrome/browser/ui/tab_modal_confirm_dialog.h"

@class ConstrainedWindowAlert;

namespace content {
class WebContents;
}

class TabModalConfirmDialogDelegate;
@class TabModalConfirmDialogMacBridge;

class TabModalConfirmDialogMac : public TabModalConfirmDialog,
                                 public ConstrainedWindowMacDelegate {
 public:
  TabModalConfirmDialogMac(TabModalConfirmDialogDelegate* delegate,
                           content::WebContents* web_contents);

 private:
  virtual ~TabModalConfirmDialogMac();

  
  virtual void AcceptTabModalDialog() OVERRIDE;
  virtual void CancelTabModalDialog() OVERRIDE;

  
  virtual void CloseDialog() OVERRIDE;

  
  virtual void OnConstrainedWindowClosed(
      ConstrainedWindowMac* window) OVERRIDE;

  bool closing_;

  scoped_ptr<ConstrainedWindowMac> window_;
  scoped_ptr<TabModalConfirmDialogDelegate> delegate_;
  base::scoped_nsobject<ConstrainedWindowAlert> alert_;
  base::scoped_nsobject<TabModalConfirmDialogMacBridge> bridge_;

  DISALLOW_COPY_AND_ASSIGN(TabModalConfirmDialogMac);
};

#endif  
