// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_VIEW_TYPE_UTILS_H_
#define EXTENSIONS_BROWSER_VIEW_TYPE_UTILS_H_

#include "extensions/common/view_type.h"

namespace content {
class WebContents;
}

namespace extensions {

ViewType GetViewType(content::WebContents* tab);
void SetViewType(content::WebContents* tab, ViewType type);

}  

#endif  