// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_WINDOW_SIZE_AUTOSAVER_H_
#define CHROME_BROWSER_UI_COCOA_WINDOW_SIZE_AUTOSAVER_H_

class PrefService;
@class NSWindow;

@interface WindowSizeAutosaver : NSObject {
  NSWindow* window_;  
  PrefService* prefService_;  
  const char* path_;
}

- (id)initWithWindow:(NSWindow*)window
         prefService:(PrefService*)prefs
                path:(const char*)path;
@end

#endif  

