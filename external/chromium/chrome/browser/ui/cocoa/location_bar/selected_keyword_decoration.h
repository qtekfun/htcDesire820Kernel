// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_SELECTED_KEYWORD_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_SELECTED_KEYWORD_DECORATION_H_
#pragma once

#include <string>

#import <Cocoa/Cocoa.h>

#include "base/string16.h"
#include "chrome/browser/ui/cocoa/location_bar/bubble_decoration.h"

class SelectedKeywordDecoration : public BubbleDecoration {
 public:
  SelectedKeywordDecoration(NSFont* font);
  virtual ~SelectedKeywordDecoration();

  
  
  void SetKeyword(const string16& keyword, bool is_extension_keyword);

  
  
  
  virtual CGFloat GetWidthForSpace(CGFloat width);

  void SetImage(NSImage* image);

 private:
  friend class SelectedKeywordDecorationTest;
  FRIEND_TEST_ALL_PREFIXES(SelectedKeywordDecorationTest,
                           UsesPartialKeywordIfNarrow);

  scoped_nsobject<NSImage> search_image_;
  scoped_nsobject<NSString> full_string_;
  scoped_nsobject<NSString> partial_string_;

  DISALLOW_COPY_AND_ASSIGN(SelectedKeywordDecoration);
};

#endif  
