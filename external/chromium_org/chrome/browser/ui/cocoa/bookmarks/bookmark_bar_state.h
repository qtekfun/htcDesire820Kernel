// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_STATE_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_STATE_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/bookmarks/bookmark_bar.h"

@protocol BookmarkBarState

- (BOOL)isVisible;

- (BOOL)isAnimationRunning;

- (BOOL)isInState:(BookmarkBar::State)state;

- (BOOL)isAnimatingToState:(BookmarkBar::State)state;

- (BOOL)isAnimatingFromState:(BookmarkBar::State)state;

- (BOOL)isAnimatingFromState:(BookmarkBar::State)fromState
                     toState:(BookmarkBar::State)toState;

- (BOOL)isAnimatingBetweenState:(BookmarkBar::State)fromState
                       andState:(BookmarkBar::State)toState;

- (CGFloat)detachedMorphProgress;

@end

#endif  
