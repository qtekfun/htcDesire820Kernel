// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_MAC_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_MAC_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "components/web_modal/native_web_contents_modal_dialog.h"

namespace content {
class WebContents;
}
class ConstrainedWindowMac;
@protocol ConstrainedWindowSheet;

class ConstrainedWindowMacDelegate {
 public:
  virtual void OnConstrainedWindowClosed(ConstrainedWindowMac* window) = 0;
};

class ConstrainedWindowMac {
 public:
  ConstrainedWindowMac(
      ConstrainedWindowMacDelegate* delegate,
      content::WebContents* web_contents,
      id<ConstrainedWindowSheet> sheet);
  virtual ~ConstrainedWindowMac();

  void ShowWebContentsModalDialog();
  
  void CloseWebContentsModalDialog();
  void FocusWebContentsModalDialog();
  void PulseWebContentsModalDialog();
  web_modal::NativeWebContentsModalDialog GetNativeDialog();

 private:
  
  NSWindow* GetParentWindow() const;

  ConstrainedWindowMacDelegate* delegate_;  

  
  content::WebContents* web_contents_;

  base::scoped_nsprotocol<id<ConstrainedWindowSheet>> sheet_;

  
  bool shown_;
};

#endif  
