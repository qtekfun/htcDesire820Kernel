// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_CHROME_RENDER_WIDGET_HOST_VIEW_MAC_HISTORY_SWIPER_
#define CHROME_BROWSER_RENDERER_HOST_CHROME_RENDER_WIDGET_HOST_VIEW_MAC_HISTORY_SWIPER_

#import <Cocoa/Cocoa.h>

@class HistorySwiper;
@protocol HistorySwiperDelegate
- (BOOL)shouldAllowHistorySwiping;
- (NSView*)viewThatWantsHistoryOverlay;
@end

namespace history_swiper {
enum NavigationDirection {
  kBackwards = 0,
  kForwards,
};
} 

@class HistoryOverlayController;
@interface HistorySwiper : NSObject {
 @private
  
  
  BOOL isPinnedLeft_;
  BOOL isPinnedRight_;

  
  
  BOOL hasHorizontalScrollbar_;

  
  
  
  
  BOOL gotUnhandledWheelEvent_;

  
  HistoryOverlayController* historyOverlay_;
  
  
  int currentGestureId_;
  
  NSPoint gestureStartPoint_;
  
  NSPoint gestureCurrentPoint_;
  
  
  
  BOOL inGesture_;
  
  
  int gestureStartPointValid_;
  
  int lastProcessedGestureId_;
  
  
  BOOL historySwipeCancelled_;
  
  history_swiper::NavigationDirection historySwipeDirection_;
  
  BOOL historySwipeDirectionInverted_;

  id<HistorySwiperDelegate> delegate_;

  
  
  
  
  
  
  
  
  
  BOOL receivedTouch_;
  
  
  NSSize mouseScrollDelta_;
}

- (BOOL)handleEvent:(NSEvent*)event;
- (void)gotUnhandledWheelEvent;
- (void)scrollOffsetPinnedToLeft:(BOOL)left toRight:(BOOL)right;
- (void)setHasHorizontalScrollbar:(BOOL)hasHorizontalScrollbar;

- (void)touchesBeganWithEvent:(NSEvent*)event;
- (void)touchesMovedWithEvent:(NSEvent*)event;
- (void)touchesCancelledWithEvent:(NSEvent*)event;
- (void)touchesEndedWithEvent:(NSEvent*)event;
- (void)beginGestureWithEvent:(NSEvent*)event;
- (void)endGestureWithEvent:(NSEvent*)event;

- (id)initWithDelegate:(id<HistorySwiperDelegate>)delegate;

@property (nonatomic, assign) id<HistorySwiperDelegate> delegate;

@end

@interface HistorySwiper (PrivateExposedForTesting)
+ (void)resetMagicMouseState;
@end

#endif 
