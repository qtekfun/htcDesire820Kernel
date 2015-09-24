// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ACCESSIBILITY_INVERT_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_ACCESSIBILITY_INVERT_BUBBLE_VIEW_H_

class Browser;

namespace views {
class View;
}

namespace chrome {

void MaybeShowInvertBubbleView(Browser* browser, views::View* anchor_view);

}  

#endif  
