// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_TAB_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_TAB_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/applescript/element_applescript.h"

namespace content {
class WebContents;
}

@interface TabAppleScript : ElementAppleScript {
 @private
  content::WebContents* webContents_;  
  
  
  
  NSString* tempURL_;
}

- (id)init;

- (id)initWithWebContents:(content::WebContents*)webContents;

- (void)setWebContents:(content::WebContents*)webContents;

- (NSString*)URL;

- (void)setURL:(NSString*)aURL;

- (NSString*)title;

- (NSNumber*)loading;

- (void)handlesUndoScriptCommand:(NSScriptCommand*)command;
- (void)handlesRedoScriptCommand:(NSScriptCommand*)command;

- (void)handlesCutScriptCommand:(NSScriptCommand*)command;
- (void)handlesCopyScriptCommand:(NSScriptCommand*)command;
- (void)handlesPasteScriptCommand:(NSScriptCommand*)command;

- (void)handlesSelectAllScriptCommand:(NSScriptCommand*)command;

- (void)handlesGoBackScriptCommand:(NSScriptCommand*)command;
- (void)handlesGoForwardScriptCommand:(NSScriptCommand*)command;
- (void)handlesReloadScriptCommand:(NSScriptCommand*)command;
- (void)handlesStopScriptCommand:(NSScriptCommand*)command;

- (void)handlesPrintScriptCommand:(NSScriptCommand*)command;

- (void)handlesSaveScriptCommand:(NSScriptCommand*)command;

- (void)handlesCloseScriptCommand:(NSScriptCommand*)command;

- (void)handlesViewSourceScriptCommand:(NSScriptCommand*)command;

- (id)handlesExecuteJavascriptScriptCommand:(NSScriptCommand*)command;

@end

#endif  
