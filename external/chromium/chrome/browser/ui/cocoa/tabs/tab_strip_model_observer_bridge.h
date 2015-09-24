// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_MODEL_OBSERVER_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_MODEL_OBSERVER_BRIDGE_H_
#pragma once

#import <Foundation/Foundation.h>

#include "chrome/browser/tabs/tab_strip_model_observer.h"

class TabContentsWrapper;
class TabStripModel;

class TabStripModelObserverBridge : public TabStripModelObserver {
 public:
  TabStripModelObserverBridge(TabStripModel* model, id controller);
  virtual ~TabStripModelObserverBridge();

  
  virtual void TabInsertedAt(TabContentsWrapper* contents,
                             int index,
                             bool foreground);
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            TabContentsWrapper* contents,
                            int index);
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);
  virtual void TabMoved(TabContentsWrapper* contents,
                        int from_index,
                        int to_index);
  virtual void TabChangedAt(TabContentsWrapper* contents, int index,
                            TabChangeType change_type);
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index);
  virtual void TabMiniStateChanged(TabContentsWrapper* contents, int index);
  virtual void TabStripEmpty();
  virtual void TabStripModelDeleted();

 private:
  id controller_;  
  TabStripModel* model_;  
};

@interface NSObject(TabStripModelBridge)
- (void)insertTabWithContents:(TabContentsWrapper*)contents
                      atIndex:(NSInteger)index
                 inForeground:(bool)inForeground;
- (void)tabClosingWithContents:(TabContentsWrapper*)contents
                       atIndex:(NSInteger)index;
- (void)tabDetachedWithContents:(TabContentsWrapper*)contents
                        atIndex:(NSInteger)index;
- (void)selectTabWithContents:(TabContentsWrapper*)newContents
             previousContents:(TabContentsWrapper*)oldContents
                      atIndex:(NSInteger)index
                  userGesture:(bool)wasUserGesture;
- (void)tabMovedWithContents:(TabContentsWrapper*)contents
                    fromIndex:(NSInteger)from
                      toIndex:(NSInteger)to;
- (void)tabChangedWithContents:(TabContentsWrapper*)contents
                       atIndex:(NSInteger)index
                    changeType:(TabStripModelObserver::TabChangeType)change;
- (void)tabReplacedWithContents:(TabContentsWrapper*)newContents
               previousContents:(TabContentsWrapper*)oldContents
                        atIndex:(NSInteger)index;
- (void)tabMiniStateChangedWithContents:(TabContentsWrapper*)contents
                                atIndex:(NSInteger)index;
- (void)tabStripEmpty;
- (void)tabStripModelDeleted;
@end

#endif  
