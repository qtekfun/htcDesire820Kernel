// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_WEB_APPS_H_
#define CHROME_RENDERER_WEB_APPS_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "ui/gfx/size.h"

namespace blink {
class WebFrame;
}

struct WebApplicationInfo;

namespace web_apps {

bool ParseIconSizes(const base::string16& text, std::vector<gfx::Size>* sizes,
                    bool* is_any);

bool ParseWebAppFromWebDocument(blink::WebFrame* frame,
                                WebApplicationInfo* web_app,
                                base::string16* error);

}  

#endif  
