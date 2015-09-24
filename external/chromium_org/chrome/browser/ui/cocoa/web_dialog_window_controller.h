// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_WEB_DIALOG_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_WEB_DIALOG_WINDOW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/web_dialogs/web_dialog_ui.h"

class WebDialogWindowDelegateBridge;

namespace content {
class BrowserContext;
class WebContents;
}

@interface WebDialogWindowController : NSWindowController<NSWindowDelegate> {
 @private
  
  
  scoped_ptr<WebDialogWindowDelegateBridge> delegate_;
  scoped_ptr<content::WebContents> webContents_;
}

+ (NSWindow*)showWebDialog:(ui::WebDialogDelegate*)delegate
                   context:(content::BrowserContext*)context;

@end

@interface WebDialogWindowController (TestingAPI)

- (id)initWithDelegate:(ui::WebDialogDelegate*)delegate
               context:(content::BrowserContext*)context;

- (void)loadDialogContents;

@end

#endif  
