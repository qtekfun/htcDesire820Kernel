// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_IMAGE_UTIL_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_IMAGE_UTIL_H_

namespace gfx {
class ImageSkia;
}

namespace views {

gfx::ImageSkia GetMenuCheckImage(bool dark_background);

gfx::ImageSkia GetRadioButtonImage(bool selected);

gfx::ImageSkia GetSubmenuArrowImage(bool dark_background);

}  

#endif  
