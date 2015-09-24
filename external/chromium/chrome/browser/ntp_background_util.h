// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NTP_BACKGROUND_UTIL_H_
#define CHROME_BROWSER_NTP_BACKGROUND_UTIL_H_
#pragma once

namespace gfx {
class Canvas;
class Rect;
}

namespace ui {
class ThemeProvider;
}

class NtpBackgroundUtil {
 public:
  
  
  
  
  static void PaintBackgroundDetachedMode(
      ui::ThemeProvider* tp, gfx::Canvas* canvas,
      const gfx::Rect& area, int tab_contents_height);

 private:
  NtpBackgroundUtil() {}
};

#endif  
