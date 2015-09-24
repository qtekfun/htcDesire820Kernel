// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_RESULT_VIEW_MODEL_H_
#define CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_RESULT_VIEW_MODEL_H_

namespace gfx {
class Image;
}

class OmniboxResultViewModel {
 public:
  
  virtual bool IsSelectedIndex(size_t index) const = 0;

  
  virtual bool IsHoveredIndex(size_t index) const = 0;

  
  
  virtual gfx::Image GetIconIfExtensionMatch(size_t index) const = 0;
};

#endif  
