// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ONE_CLICK_SIGNIN_VIEW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_ONE_CLICK_SIGNIN_VIEW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/callback.h"
#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/browser_window.h"

@class BrowserWindowController;
namespace content {
class WebContents;
}
@class HyperlinkTextView;

@interface OneClickSigninViewController : NSViewController<NSTextViewDelegate> {
 @private
  IBOutlet NSTextField* messageTextField_;
  IBOutlet NSTextField* informativePlaceholderTextField_;
  IBOutlet NSButton* advancedLink_;
  IBOutlet NSButton* closeButton_;

  
  BOOL isSyncDialog_;

  
  BOOL clickedLearnMore_;

  
  base::string16 email_;

  
  base::scoped_nsobject<NSString> errorMessage_;

  
  
  
  
  base::scoped_nsobject<HyperlinkTextView> informativeTextView_;
  BrowserWindow::StartSyncCallback startSyncCallback_;
  base::Closure closeCallback_;
  content::WebContents* webContents_;
}

- (id)initWithNibName:(NSString*)nibName
          webContents:(content::WebContents*)webContents
         syncCallback:(const BrowserWindow::StartSyncCallback&)syncCallback
        closeCallback:(const base::Closure&)callback
         isSyncDialog:(BOOL)isSyncDialog
                email:(const base::string16&)email
         errorMessage:(NSString*)errorMessage;

- (void)viewWillClose;

- (IBAction)ok:(id)sender;

- (IBAction)onClickClose:(id)sender;

- (IBAction)onClickUndo:(id)sender;

- (IBAction)onClickAdvancedLink:(id)sender;

@end

#endif  
