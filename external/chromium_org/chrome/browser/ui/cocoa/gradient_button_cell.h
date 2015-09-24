// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_GRADIENT_BUTTON_CELL_H_
#define CHROME_BROWSER_UI_COCOA_GRADIENT_BUTTON_CELL_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

namespace ui {
class ThemeProvider;
}


enum {
  kLeftButtonType = -1,
  kLeftButtonWithShadowType = -2,
  kStandardButtonType = 0,
  kRightButtonType = 1,
  kMiddleButtonType = 2,
  
  
  
  kStandardButtonTypeWithLimitedClickFeedback = 3,
};
typedef NSInteger ButtonType;

namespace gradient_button_cell {

typedef enum {
  
  kPulsedOn,
  kPulsedOff,
  
  kPulsingOn,
  kPulsingOff,
  
  kPulsingContinuous,
} PulseState;

};


@interface GradientButtonCell : NSButtonCell {
 @private
  
  
  BOOL isMouseInside_;
  base::scoped_nsobject<NSTrackingArea> trackingArea_;
  BOOL shouldTheme_;
  CGFloat hoverAlpha_;  
  NSTimeInterval lastHoverUpdate_;
  base::scoped_nsobject<NSGradient> gradient_;
  gradient_button_cell::PulseState pulseState_;
  CGFloat pulseMultiplier_;  
  CGFloat outerStrokeAlphaMult_;  
  base::scoped_nsobject<NSImage> overlayImage_;
}

- (void)setShouldTheme:(BOOL)shouldTheme;

- (void)drawBorderAndFillForTheme:(ui::ThemeProvider*)themeProvider
                      controlView:(NSView*)controlView
                        innerPath:(NSBezierPath*)innerPath
              showClickedGradient:(BOOL)showClickedGradient
            showHighlightGradient:(BOOL)showHighlightGradient
                       hoverAlpha:(CGFloat)hoverAlpha
                           active:(BOOL)active
                        cellFrame:(NSRect)cellFrame
                  defaultGradient:(NSGradient*)defaultGradient;

- (void)setMouseInside:(BOOL)flag animate:(BOOL)animate;

- (NSBezierPath*)clipPathForFrame:(NSRect)cellFrame
                           inView:(NSView*)controlView;

- (void)setIsContinuousPulsing:(BOOL)continuous;

- (BOOL)isContinuousPulsing;

- (void)safelyStopPulsing;

- (BOOL)isMouseReallyInside;

- (int)verticalTextOffset;

@property(assign, nonatomic) CGFloat hoverAlpha;

@property(retain, nonatomic) NSImage* overlayImage;

@end

@interface GradientButtonCell(TestingAPI)
- (BOOL)isMouseInside;
- (BOOL)pulsing;
- (gradient_button_cell::PulseState)pulseState;
- (void)setPulseState:(gradient_button_cell::PulseState)pstate;
@end

#endif  
