// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_COCOA_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_COCOA_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/memory/scoped_nsobject.h"
#import "chrome/browser/accessibility/browser_accessibility_delegate_mac.h"
#include "chrome/browser/accessibility/browser_accessibility.h"

@interface BrowserAccessibilityCocoa : NSView {
 @private
  BrowserAccessibility* browserAccessibility_;
  scoped_nsobject<NSMutableArray> children_;
  id<BrowserAccessibilityDelegateCocoa> delegate_;
}

- (id)initWithObject:(BrowserAccessibility*)accessibility
            delegate:(id<BrowserAccessibilityDelegateCocoa>)delegate;

- (void)childrenChanged;

@property(nonatomic, readonly) NSArray* children;
@property(nonatomic, readonly, getter=isIgnored) BOOL ignored;
@property(nonatomic, readonly) NSPoint origin;
@property(nonatomic, readonly) NSString* role;
@property(nonatomic, readonly) NSSize size;

@end

#endif 
