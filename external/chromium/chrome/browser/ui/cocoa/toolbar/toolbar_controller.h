// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/command_observer_bridge.h"
#import "chrome/browser/ui/cocoa/tracking_area.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"
#include "chrome/browser/prefs/pref_member.h"

@class AutocompleteTextField;
@class AutocompleteTextFieldEditor;
@class BrowserActionsContainerView;
@class BackForwardMenuController;
class Browser;
@class BrowserActionsController;
class CommandUpdater;
class LocationBar;
class LocationBarViewMac;
@class MenuButton;
@class ToolbarButton;
namespace ToolbarControllerInternal {
class NotificationBridge;
class WrenchAcceleratorDelegate;
}  
class Profile;
@class ReloadButton;
class TabContents;
class ToolbarModel;
@class WrenchMenuController;
class WrenchMenuModel;


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
  IBOutlet WrenchMenuController* wrenchMenuController_;

 @private
  ToolbarModel* toolbarModel_;  
  CommandUpdater* commands_;  
  Profile* profile_;  
  Browser* browser_;  
  scoped_ptr<CommandObserverBridge> commandObserver_;
  scoped_ptr<LocationBarViewMac> locationBarView_;
  scoped_nsobject<AutocompleteTextFieldEditor> autocompleteTextFieldEditor_;
  id<ViewResizer> resizeDelegate_;  
  scoped_nsobject<BackForwardMenuController> backMenuController_;
  scoped_nsobject<BackForwardMenuController> forwardMenuController_;
  scoped_nsobject<BrowserActionsController> browserActionsController_;

  
  
  
  scoped_ptr<ToolbarControllerInternal::WrenchAcceleratorDelegate>
      acceleratorDelegate_;
  scoped_ptr<WrenchMenuModel> wrenchMenuModel_;

  
  scoped_ptr<ToolbarControllerInternal::NotificationBridge> notificationBridge_;
  BooleanPrefMember showHomeButton_;
  BOOL hasToolbar_;  
  BOOL hasLocationBar_;  
  BOOL locationBarAtMinSize_; 

  
  
  scoped_nsobject<AutocompleteTextField> locationBarRetainer_;

  
  ScopedCrTrackingArea trackingArea_;

  
  
  
  
  
  NSButton* hoveredButton_;
}

- (id)initWithModel:(ToolbarModel*)model
           commands:(CommandUpdater*)commands
            profile:(Profile*)profile
            browser:(Browser*)browser
     resizeDelegate:(id<ViewResizer>)resizeDelegate;

- (LocationBarViewMac*)locationBarBridge;

- (id)customFieldEditorForObject:(id)obj;

- (void)focusLocationBar:(BOOL)selectAll;

- (void)updateToolbarWithContents:(TabContents*)tabForRestoring
               shouldRestoreState:(BOOL)shouldRestore;

- (void)setStarredState:(BOOL)isStarred;

- (void)setIsLoading:(BOOL)isLoading force:(BOOL)force;

- (void)setHasToolbar:(BOOL)toolbar hasLocationBar:(BOOL)locBar;

- (NSPoint)bookmarkBubblePoint;

- (CGFloat)desiredHeightForCompression:(CGFloat)compressByHeight;

- (void)setDividerOpacity:(CGFloat)opacity;

- (void)createBrowserActionButtons;

- (BrowserActionsController*)browserActionsController;

@end

@interface ToolbarController(ProtectedMethods)
- (id)initWithModel:(ToolbarModel*)model
           commands:(CommandUpdater*)commands
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
