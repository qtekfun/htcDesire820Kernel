// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_PAGE_ZOOM_H_
#define CHROME_BROWSER_CHROME_PAGE_ZOOM_H_

#include <vector>
#include "content/public/common/page_zoom.h"

namespace content {
class WebContents;
}

namespace chrome_page_zoom {

std::vector<double> PresetZoomFactors(double custom_factor);

std::vector<double> PresetZoomLevels(double custom_level);

void Zoom(content::WebContents* web_contents, content::PageZoom zoom);

}  

#endif  
