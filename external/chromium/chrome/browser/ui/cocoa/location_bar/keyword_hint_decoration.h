// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_KEYWORD_HINT_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_KEYWORD_HINT_DECORATION_H_
#pragma once

#include <string>

#import "chrome/browser/ui/cocoa/location_bar/location_bar_decoration.h"

#import "base/memory/scoped_nsobject.h"
#include "base/string16.h"


class KeywordHintDecoration : public LocationBarDecoration {
 public:
  KeywordHintDecoration(NSFont* font);
  virtual ~KeywordHintDecoration();

  
  
  void SetKeyword(const string16& keyword, bool is_extension_keyword);

  
  virtual void DrawInFrame(NSRect frame, NSView* control_view);
  virtual CGFloat GetWidthForSpace(CGFloat width);

 private:
  
  NSImage* GetHintImage();

  
  scoped_nsobject<NSDictionary> attributes_;

  
  scoped_nsobject<NSImage> hint_image_;

  
  scoped_nsobject<NSString> hint_prefix_;
  scoped_nsobject<NSString> hint_suffix_;

  DISALLOW_COPY_AND_ASSIGN(KeywordHintDecoration);
};

#endif  
