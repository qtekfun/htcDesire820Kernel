// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_WINDOW_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_WINDOW_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/applescript/element_applescript.h"

class Browser;
class Profile;
@class TabAppleScript;

@interface WindowAppleScript : ElementAppleScript {
 @private
  Browser* browser_;  
}

- (id)init;

- (id)initWithProfile:(Profile*)aProfile;

- (id)initWithBrowser:(Browser*)aBrowser;

- (NSNumber*)activeTabIndex;
- (void)setActiveTabIndex:(NSNumber*)anActiveTabIndex;

- (NSString*)mode;
- (void)setMode:(NSString*)theMode;

- (TabAppleScript*)activeTab;

- (NSArray*)tabs;

- (void)insertInTabs:(TabAppleScript*)aTab;

- (void)insertInTabs:(TabAppleScript*)aTab atIndex:(int)index;

- (void)removeFromTabsAtIndex:(int)index;

- (void)setOrderedIndex:(NSNumber*)anIndex;

- (NSComparisonResult)windowComparator:(WindowAppleScript*)otherWindow;

- (id)valueForUndefinedKey:(NSString*)key;
- (void)setValue:(id)value forUndefinedKey:(NSString*)key;

- (void)handlesCloseScriptCommand:(NSCloseCommand*)command;

- (NSNumber*)orderedIndex;

- (NSNumber*)presenting;

- (void)handlesEnterPresentationMode:(NSScriptCommand*)command;

- (void)handlesExitPresentationMode:(NSScriptCommand*)command;

@end

#endif  
