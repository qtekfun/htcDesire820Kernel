// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APP_CONTROLLER_MAC_H_
#define CHROME_BROWSER_APP_CONTROLLER_MAC_H_

#if defined(__OBJC__)

#import <Cocoa/Cocoa.h>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/time/time.h"
#include "ui/base/work_area_watcher_observer.h"

class AppControllerProfileObserver;
@class AppShimMenuController;
class BookmarkMenuBridge;
class CommandUpdater;
class GURL;
class HistoryMenuBridge;
class Profile;
@class ProfileMenuController;
namespace ui {
class WorkAreaWatcherObserver;
}

@interface AppController : NSObject<NSUserInterfaceValidations,
                                    NSApplicationDelegate> {
 @private
  
  scoped_ptr<CommandUpdater> menuState_;

  
  
  Profile* lastProfile_;

  
  
  scoped_ptr<AppControllerProfileObserver> profileInfoCacheObserver_;

  
  
  scoped_ptr<BookmarkMenuBridge> bookmarkMenuBridge_;
  scoped_ptr<HistoryMenuBridge> historyMenuBridge_;

  
  base::scoped_nsobject<AppShimMenuController> appShimMenuController_;

  
  
  base::scoped_nsobject<ProfileMenuController> profileMenuController_;

  
  
  
  std::vector<GURL> startupUrls_;
  BOOL startupComplete_;

  
  
  
  IBOutlet NSMenuItem* closeTabMenuItem_;
  IBOutlet NSMenuItem* closeWindowMenuItem_;
  BOOL fileMenuUpdatePending_;  

  
  
  IBOutlet NSMenu* helpMenu_;

  
  IBOutlet NSMenuItem* tabposeMenuItem_;

  
  BOOL hasPopover_;

  
  
  base::TimeTicks reopenTime_;

  
  ObserverList<ui::WorkAreaWatcherObserver> workAreaChangeObservers_;

  scoped_ptr<PrefChangeRegistrar> profilePrefRegistrar_;
  PrefChangeRegistrar localPrefRegistrar_;
}

@property(readonly, nonatomic) BOOL startupComplete;
@property(readonly, nonatomic) Profile* lastProfile;

- (void)didEndMainMessageLoop;

- (BOOL)tryToTerminateApplication:(NSApplication*)app;

- (void)stopTryingToTerminateApplication:(NSApplication*)app;

- (BOOL)keyWindowIsModal;

- (IBAction)showPreferences:(id)sender;

- (IBAction)orderFrontStandardAboutPanel:(id)sender;

- (IBAction)toggleConfirmToQuit:(id)sender;

- (NSMenu*)applicationDockMenu:(NSApplication*)sender;

- (const std::vector<GURL>&)startupUrls;

- (void)clearStartupUrls;

- (BookmarkMenuBridge*)bookmarkMenuBridge;

- (void)addObserverForWorkAreaChange:(ui::WorkAreaWatcherObserver*)observer;
- (void)removeObserverForWorkAreaChange:(ui::WorkAreaWatcherObserver*)observer;

- (void)initAppShimMenuController;

@end

#endif  


namespace app_controller_mac {

bool IsOpeningNewWindow();

}  

#endif
