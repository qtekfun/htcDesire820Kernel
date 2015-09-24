// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_HTML_DIALOG_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_HTML_DIALOG_WINDOW_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/webui/html_dialog_ui.h"

class HtmlDialogWindowDelegateBridge;
class Profile;
class TabContents;

@interface HtmlDialogWindowController : NSWindowController {
 @private
  
  
  scoped_ptr<HtmlDialogWindowDelegateBridge> delegate_;
  scoped_ptr<TabContents> tabContents_;
}

+ (NSWindow*)showHtmlDialog:(HtmlDialogUIDelegate*)delegate
                    profile:(Profile*)profile;

@end

@interface HtmlDialogWindowController (TestingAPI)

- (id)initWithDelegate:(HtmlDialogUIDelegate*)delegate
               profile:(Profile*)profile;

- (void)loadDialogContents;

@end

#endif  

