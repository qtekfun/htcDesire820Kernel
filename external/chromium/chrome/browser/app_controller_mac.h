// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APP_CONTROLLER_MAC_H_
#define CHROME_BROWSER_APP_CONTROLLER_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>
#include <vector>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

@class AboutWindowController;
class BookmarkMenuBridge;
class CommandUpdater;
class GURL;
class HistoryMenuBridge;
class Profile;

@interface AppController : NSObject<NSUserInterfaceValidations,
                                    NSApplicationDelegate> {
 @private
  scoped_ptr<CommandUpdater> menuState_;
  
  
  scoped_ptr<BookmarkMenuBridge> bookmarkMenuBridge_;
  scoped_ptr<HistoryMenuBridge> historyMenuBridge_;
  AboutWindowController* aboutController_;  

  
  
  
  std::vector<GURL> startupUrls_;
  BOOL startupComplete_;

  
  
  
  IBOutlet NSMenuItem* closeTabMenuItem_;
  IBOutlet NSMenuItem* closeWindowMenuItem_;
  BOOL fileMenuUpdatePending_;  

  
  
  IBOutlet NSMenu* helpMenu_;

  
  IBOutlet NSMenuItem* tabposeMenuItem_;
}

@property(readonly, nonatomic) BOOL startupComplete;

- (void)didEndMainMessageLoop;
- (Profile*)defaultProfile;

- (BOOL)tryToTerminateApplication:(NSApplication*)app;

- (void)stopTryingToTerminateApplication:(NSApplication*)app;

- (BOOL)keyWindowIsNotModal;

- (IBAction)showPreferences:(id)sender;

- (IBAction)orderFrontStandardAboutPanel:(id)sender;

- (IBAction)toggleConfirmToQuit:(id)sender;

- (NSMenu*)applicationDockMenu:(NSApplication*)sender;

- (const std::vector<GURL>&)startupUrls;

- (void)clearStartupUrls;

@end

#endif
