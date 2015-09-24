// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/styled_text_field_cell.h"

@class AutocompleteTextField;
class LocationBarDecoration;

@interface AutocompleteTextFieldCell : StyledTextFieldCell {
 @private
  
  
  std::vector<LocationBarDecoration*> leftDecorations_;
  std::vector<LocationBarDecoration*> rightDecorations_;

  
  
  BOOL hideFocusState_;

  
  BOOL isPopupMode_;
}

@property(assign, nonatomic) BOOL isPopupMode;

- (CGFloat)lineHeight;

- (void)clearDecorations;

- (void)addLeftDecoration:(LocationBarDecoration*)decoration;

- (void)addRightDecoration:(LocationBarDecoration*)decoration;

- (CGFloat)availableWidthInFrame:(const NSRect)frame;

- (NSRect)frameForDecoration:(const LocationBarDecoration*)aDecoration
                     inFrame:(NSRect)cellFrame;

- (LocationBarDecoration*)decorationForEvent:(NSEvent*)theEvent
                                      inRect:(NSRect)cellFrame
                                      ofView:(AutocompleteTextField*)field;

- (NSMenu*)decorationMenuForEvent:(NSEvent*)theEvent
                           inRect:(NSRect)cellFrame
                           ofView:(AutocompleteTextField*)controlView;

- (BOOL)mouseDown:(NSEvent*)theEvent
           inRect:(NSRect)cellFrame
           ofView:(AutocompleteTextField*)controlView;

- (void)mouseEntered:(NSEvent*)theEvent
              inView:(AutocompleteTextField*)controlView;
- (void)mouseExited:(NSEvent*)theEvent
             inView:(AutocompleteTextField*)controlView;

- (void)setUpTrackingAreasInRect:(NSRect)frame
                          ofView:(AutocompleteTextField*)view;

- (NSRect)textCursorFrameForFrame:(NSRect)cellFrame;

- (void)updateToolTipsInRect:(NSRect)cellFrame
                      ofView:(AutocompleteTextField*)controlView;

- (BOOL)hideFocusState;
- (void)setHideFocusState:(BOOL)hideFocusState
                   ofView:(AutocompleteTextField*)controlView;

@end
