// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_RESOURCES_H_
#define CHROME_BROWSER_UI_TABS_TAB_RESOURCES_H_

namespace gfx {
class Path;
}

class TabResources {
 public:
  
  
  
  
  static void GetHitTestMask(int width,
                             int height,
                             bool include_top_shadow,
                             gfx::Path* path);
};

#endif  
