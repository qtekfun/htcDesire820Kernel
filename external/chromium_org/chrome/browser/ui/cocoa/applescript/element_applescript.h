// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_ELEMENT_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_ELEMENT_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

@interface ElementAppleScript : NSObject {
 @protected
  
  NSNumber* uniqueID_;
  
  
  id container_;
  NSString* containerProperty_;
}

@property(nonatomic, copy) NSNumber* uniqueID;
@property(nonatomic, retain) id container;
@property(nonatomic, copy) NSString* containerProperty;

- (NSScriptObjectSpecifier*)objectSpecifier;

- (void)setContainer:(id)value property:(NSString*)property;

@end

#endif
