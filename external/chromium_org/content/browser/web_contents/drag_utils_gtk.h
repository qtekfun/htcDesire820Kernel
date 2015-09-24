// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_DRAG_UTILS_GTK_H_
#define CONTENT_BROWSER_WEB_CONTENTS_DRAG_UTILS_GTK_H_

#include <gtk/gtk.h>

#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"

namespace content {

CONTENT_EXPORT GdkDragAction WebDragOpToGdkDragAction(
    blink::WebDragOperationsMask op);
CONTENT_EXPORT blink::WebDragOperationsMask GdkDragActionToWebDragOp(
    GdkDragAction action);

}  

#endif  
