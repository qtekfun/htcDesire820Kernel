// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_TRUETYPE_FONT_LIST_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_TRUETYPE_FONT_LIST_H_

#include <string>
#include <vector>

namespace ppapi {
namespace proxy {
struct SerializedTrueTypeFontDesc;
}
}

namespace content {

void GetFontFamilies_SlowBlocking(std::vector<std::string>* font_families);

void GetFontsInFamily_SlowBlocking(
    const std::string& family,
    std::vector<ppapi::proxy::SerializedTrueTypeFontDesc>* fonts_in_family);

}  

#endif  
