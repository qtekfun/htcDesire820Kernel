// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_STATE_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_STATE_H_
#pragma once

#import <Cocoa/Cocoa.h>

namespace bookmarks {

enum VisualState {
  kInvalidState  = 0,
  kHiddenState   = 1,
  kShowingState  = 2,
  kDetachedState = 3,
};

}  

@protocol BookmarkBarState

- (BOOL)isVisible;

- (BOOL)isAnimationRunning;

- (BOOL)isInState:(bookmarks::VisualState)state;

- (BOOL)isAnimatingToState:(bookmarks::VisualState)state;

- (BOOL)isAnimatingFromState:(bookmarks::VisualState)state;

- (BOOL)isAnimatingFromState:(bookmarks::VisualState)fromState
                     toState:(bookmarks::VisualState)toState;

- (BOOL)isAnimatingBetweenState:(bookmarks::VisualState)fromState
                       andState:(bookmarks::VisualState)toState;

- (CGFloat)detachedMorphProgress;

@end

#endif  
