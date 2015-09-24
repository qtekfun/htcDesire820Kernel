// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ANIMATION_UTILS_H
#define CHROME_BROWSER_UI_COCOA_ANIMATION_UTILS_H
#pragma once

#import <Cocoa/Cocoa.h>



class WithNoAnimation {
 public:
  WithNoAnimation() {
    [NSAnimationContext beginGrouping];
    [[NSAnimationContext currentContext] setDuration:0.0];
  }

  ~WithNoAnimation() {
   [NSAnimationContext endGrouping];
  }
};


#endif 
