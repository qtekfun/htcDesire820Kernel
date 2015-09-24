// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_ALERT_H_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_ALERT_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@interface ConstrainedWindowAlert : NSObject {
 @private
  base::scoped_nsobject<NSTextField> informativeTextField_;
  base::scoped_nsobject<NSTextField> messageTextField_;
  base::scoped_nsobject<NSButton> linkView_;
  base::scoped_nsobject<NSView> accessoryView_;
  base::scoped_nsobject<NSMutableArray> buttons_;
  base::scoped_nsobject<NSButton> closeButton_;
  base::scoped_nsobject<NSWindow> window_;
}

@property(nonatomic, copy) NSString* informativeText;
@property(nonatomic, copy) NSString* messageText;
@property(nonatomic, retain) NSView* accessoryView;
@property(nonatomic, readonly) NSArray* buttons;
@property(nonatomic, readonly) NSButton* closeButton;
@property(nonatomic, readonly) NSWindow* window;

- (id)init;

- (void)addButtonWithTitle:(NSString*)title
             keyEquivalent:(NSString*)keyEquivalent
                    target:(id)target
                    action:(SEL)action;

- (void)setLinkText:(NSString*)text
             target:(id)target
             action:(SEL)action;

- (void)layout;

@end

@interface ConstrainedWindowAlert (ExposedForTesting)
@property(nonatomic, readonly) NSButton* linkView;
@end

#endif  
