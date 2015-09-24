// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#import "chrome/browser/ui/cocoa/command_observer_bridge.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"
#import "ui/base/cocoa/tracking_area.h"

@class AutocompleteTextField;
@class AutocompleteTextFieldEditor;
@class BackForwardMenuController;
class Browser;
@class BrowserActionsContainerView;
@class BrowserActionsController;
class CommandUpdater;
class LocationBarViewMac;
@class MenuButton;
class Profile;
@class ReloadButton;
@class ToolbarButton;
@class WrenchMenuController;

namespace content {
class WebContents;
}

namespace ToolbarControllerInternal {
class NotificationBridge;
}


@interface ToolbarController : NSViewController<CommandObserverProtocol,
                                                URLDropTargetController> {
 @protected
  
  
  
  IBOutlet MenuButton* backButton_;
  IBOutlet MenuButton* forwardButton_;
  IBOutlet ReloadButton* reloadButton_;
  IBOutlet ToolbarButton* homeButton_;
  IBOutlet MenuButton* wrenchButton_;
  IBOutlet AutocompleteTextField* locationBar_;
  IBOutlet BrowserActionsContainerView* browserActionsContainerView_;

 @private
  CommandUpdater* commands_;  
  Profile* profile_;  
  Browser* browser_;  
  scoped_ptr<CommandObserverBridge> commandObserver_;
  scoped_ptr<LocationBarViewMac> locationBarView_;
  base::scoped_nsobject<AutocompleteTextFieldEditor>
      autocompleteTextFieldEditor_;
  id<ViewResizer> resizeDelegate_;  
  base::scoped_nsobject<BackForwardMenuController> backMenuController_;
  base::scoped_nsobject<BackForwardMenuController> forwardMenuController_;
  base::scoped_nsobject<BrowserActionsController> browserActionsController_;

  
  base::scoped_nsobject<WrenchMenuController> wrenchMenuController_;

  
  scoped_ptr<ToolbarControllerInternal::NotificationBridge> notificationBridge_;
  BooleanPrefMember showHomeButton_;
  BOOL hasToolbar_;  
  BOOL hasLocationBar_;  
  BOOL locationBarAtMinSize_; 

  
  
  base::scoped_nsobject<AutocompleteTextField> locationBarRetainer_;

  
  ui::ScopedCrTrackingArea trackingArea_;

  
  
  
  
  
  NSButton* hoveredButton_;
}

- (id)initWithCommands:(CommandUpdater*)commands
               profile:(Profile*)profile
               browser:(Browser*)browser
        resizeDelegate:(id<ViewResizer>)resizeDelegate;

- (LocationBarViewMac*)locationBarBridge;

- (id)customFieldEditorForObject:(id)obj;

- (void)focusLocationBar:(BOOL)selectAll;

- (void)updateToolbarWithContents:(content::WebContents*)tab;

- (void)setStarredState:(BOOL)isStarred;

- (void)zoomChangedForActiveTab:(BOOL)canShowBubble;

- (void)setIsLoading:(BOOL)isLoading force:(BOOL)force;

- (void)setHasToolbar:(BOOL)toolbar hasLocationBar:(BOOL)locBar;

- (NSPoint)bookmarkBubblePoint;

- (CGFloat)desiredHeightForCompression:(CGFloat)compressByHeight;

- (void)setDividerOpacity:(CGFloat)opacity;

- (void)createBrowserActionButtons;

- (BrowserActionsController*)browserActionsController;

- (NSView*)wrenchButton;

@end

@interface ToolbarController(ProtectedMethods)
- (id)initWithCommands:(CommandUpdater*)commands
               profile:(Profile*)profile
               browser:(Browser*)browser
        resizeDelegate:(id<ViewResizer>)resizeDelegate
          nibFileNamed:(NSString*)nibName;
@end

@interface ToolbarController(PrivateTestMethods)
- (NSArray*)toolbarViews;
- (void)showOptionalHomeButton;
- (void)installWrenchMenu;
- (WrenchMenuController*)wrenchMenuController;
- (NSButton*)hoverButtonForEvent:(NSEvent*)theEvent;
@end

#endif  
