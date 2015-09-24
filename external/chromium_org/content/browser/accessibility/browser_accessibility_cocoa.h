// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_COCOA_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_COCOA_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#include "content/browser/accessibility/browser_accessibility.h"
#import "content/browser/accessibility/browser_accessibility_delegate_mac.h"
#include "third_party/WebKit/public/web/WebAXEnums.h"

@interface BrowserAccessibilityCocoa : NSObject {
 @private
  content::BrowserAccessibility* browserAccessibility_;
  base::scoped_nsobject<NSMutableArray> children_;
  id<BrowserAccessibilityDelegateCocoa> delegate_;
}

- (id)initWithObject:(content::BrowserAccessibility*)accessibility
            delegate:(id<BrowserAccessibilityDelegateCocoa>)delegate;

- (void)detach;

- (void)childrenChanged;

- (blink::WebAXRole)internalRole;

- (NSString*)methodNameForAttribute:(NSString*)attribute;

@property(nonatomic, readonly) NSPoint origin;

@property(nonatomic, readonly) NSString* accessKey;
@property(nonatomic, readonly) NSNumber* ariaAtomic;
@property(nonatomic, readonly) NSNumber* ariaBusy;
@property(nonatomic, readonly) NSString* ariaLive;
@property(nonatomic, readonly) NSString* ariaRelevant;
@property(nonatomic, readonly) NSArray* children;
@property(nonatomic, readonly) NSArray* columns;
@property(nonatomic, readonly) NSArray* columnHeaders;
@property(nonatomic, readonly) NSValue* columnIndexRange;
@property(nonatomic, readonly) NSString* description;
@property(nonatomic, readonly) NSNumber* disclosing;
@property(nonatomic, readonly) id disclosedByRow;
@property(nonatomic, readonly) NSNumber* disclosureLevel;
@property(nonatomic, readonly) id disclosedRows;
@property(nonatomic, readonly) NSNumber* enabled;
@property(nonatomic, readonly) NSNumber* focused;
@property(nonatomic, readonly) NSString* help;
@property(nonatomic, readonly, getter=isIgnored) BOOL ignored;
@property(nonatomic, readonly) NSNumber* index;
@property(nonatomic, readonly) NSString* invalid;
@property(nonatomic, readonly) NSNumber* loaded;
@property(nonatomic, readonly) NSNumber* loadingProgress;
@property(nonatomic, readonly) NSNumber* maxValue;
@property(nonatomic, readonly) NSNumber* minValue;
@property(nonatomic, readonly) NSNumber* numberOfCharacters;
@property(nonatomic, readonly) NSString* orientation;
@property(nonatomic, readonly) id parent;
@property(nonatomic, readonly) NSValue* position;
@property(nonatomic, readonly) NSNumber* required;
@property(nonatomic, readonly) NSString* role;
@property(nonatomic, readonly) NSString* roleDescription;
@property(nonatomic, readonly) NSArray* rowHeaders;
@property(nonatomic, readonly) NSValue* rowIndexRange;
@property(nonatomic, readonly) NSArray* rows;
@property(nonatomic, readonly) NSValue* size;
@property(nonatomic, readonly) NSString* subrole;
@property(nonatomic, readonly) NSArray* tabs;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) id titleUIElement;
@property(nonatomic, readonly) NSString* url;
@property(nonatomic, readonly) NSString* value;
@property(nonatomic, readonly) NSString* valueDescription;
@property(nonatomic, readonly) NSValue* visibleCharacterRange;
@property(nonatomic, readonly) NSArray* visibleCells;
@property(nonatomic, readonly) NSArray* visibleColumns;
@property(nonatomic, readonly) NSArray* visibleRows;
@property(nonatomic, readonly) NSNumber* visited;
@property(nonatomic, readonly) id window;
@end

#endif 
