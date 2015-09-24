// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_CURSOR_UTILS_H_
#define CONTENT_RENDERER_CURSOR_UTILS_H_

class WebCursor;

namespace blink {
struct WebCursorInfo;
}

namespace content {

bool GetWebKitCursorInfo(const WebCursor& cursor,
                         blink::WebCursorInfo* webkit_cursor_info);

void InitializeCursorFromWebKitCursorInfo(
    WebCursor* cursor,
    const blink::WebCursorInfo& webkit_cursor_info);

}  

#endif  
