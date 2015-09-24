// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_ICON_VIEW_MODEL_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_ICON_VIEW_MODEL_H_

namespace gfx {
class ImageSkia;
}

namespace chrome {

class TabIconViewModel {
 public:
  
  virtual bool ShouldTabIconViewAnimate() const = 0;

  
  virtual gfx::ImageSkia GetFaviconForTabIconView() = 0;

 protected:
  virtual ~TabIconViewModel() {}
};

}  

#endif  
