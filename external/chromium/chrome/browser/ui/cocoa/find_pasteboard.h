// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FIND_PASTEBOARD_H_
#define CHROME_BROWSER_UI_COCOA_FIND_PASTEBOARD_H_
#pragma once

#include "base/string16.h"

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"

extern NSString* kFindPasteboardChangedNotification;

@interface FindPasteboard : NSObject {
 @private
  scoped_nsobject<NSString> findText_;
}

+ (FindPasteboard*)sharedInstance;

- (NSString*)findText;

- (void)setFindText:(NSString*)newText;
@end

@interface FindPasteboard (TestingAPI)
- (void)loadTextFromPasteboard:(NSNotification*)notification;

- (NSPasteboard*)findPboard;
@end

#endif  

string16 GetFindPboardText();

#endif  
