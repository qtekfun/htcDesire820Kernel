// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_KEYWORD_HINT_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_KEYWORD_HINT_DECORATION_H_

#include <string>

#import "chrome/browser/ui/cocoa/location_bar/location_bar_decoration.h"

#import "base/mac/scoped_nsobject.h"
#include "base/strings/string16.h"


class KeywordHintDecoration : public LocationBarDecoration {
 public:
  KeywordHintDecoration();
  virtual ~KeywordHintDecoration();

  
  
  void SetKeyword(const base::string16& keyword, bool is_extension_keyword);

  
  virtual void DrawInFrame(NSRect frame, NSView* control_view) OVERRIDE;
  virtual CGFloat GetWidthForSpace(CGFloat width) OVERRIDE;

 private:
  
  NSImage* GetHintImage();

  
  base::scoped_nsobject<NSDictionary> attributes_;

  
  base::scoped_nsobject<NSImage> hint_image_;

  
  base::scoped_nsobject<NSString> hint_prefix_;
  base::scoped_nsobject<NSString> hint_suffix_;

  DISALLOW_COPY_AND_ASSIGN(KeywordHintDecoration);
};

#endif  
