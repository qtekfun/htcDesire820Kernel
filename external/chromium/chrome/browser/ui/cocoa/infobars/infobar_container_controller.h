// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_CONTAINER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_CONTAINER_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"
#include "content/common/notification_registrar.h"

@class InfoBarController;
class InfoBarDelegate;
class InfoBarNotificationObserver;
class TabContents;
class TabStripModel;

@protocol InfoBarContainer
- (void)removeDelegate:(InfoBarDelegate*)delegate;
- (void)willRemoveController:(InfoBarController*)controller;
- (void)removeController:(InfoBarController*)controller;
@end


namespace infobars {

const CGFloat kBaseHeight = 36.0;

};  


@interface InfoBarContainerController : NSViewController <ViewResizer,
                                                          InfoBarContainer> {
 @private
  
  id<ViewResizer> resizeDelegate_;  

  
  TabContents* currentTabContents_;  

  
  scoped_nsobject<NSMutableArray> infobarControllers_;

  
  scoped_nsobject<NSMutableSet> closingInfoBars_;

  
  
  
  NotificationRegistrar registrar_;
  scoped_ptr<InfoBarNotificationObserver> infoBarObserver_;
}

- (id)initWithResizeDelegate:(id<ViewResizer>)resizeDelegate;

- (void)removeDelegate:(InfoBarDelegate*)delegate;

- (void)willRemoveController:(InfoBarController*)controller;

- (void)removeController:(InfoBarController*)controller;

- (void)changeTabContents:(TabContents*)contents;

- (void)tabDetachedWithContents:(TabContents*)contents;

- (NSUInteger)infobarCount;

- (CGFloat)antiSpoofHeight;

@end


@interface InfoBarContainerController (ForTheObserverAndTesting)

- (void)addInfoBar:(InfoBarDelegate*)delegate animate:(BOOL)animate;

- (void)closeInfoBarsForDelegate:(InfoBarDelegate*)delegate
                         animate:(BOOL)animate;

- (void)replaceInfoBarsForDelegate:(InfoBarDelegate*)old_delegate
                              with:(InfoBarDelegate*)new_delegate;

- (void)positionInfoBarsAndRedraw;

@end


@interface InfoBarContainerController (JustForTesting)

- (void)removeAllInfoBars;

@end

#endif  
