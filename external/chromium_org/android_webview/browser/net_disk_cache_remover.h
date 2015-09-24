// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_NET_DISK_CACHE_REMOVER_H_
#define ANDROID_WEBVIEW_BROWSER_NET_DISK_CACHE_REMOVER_H_

namespace content {

class BrowserContext;

}  

namespace android_webview {

void RemoveHttpDiskCache(content::BrowserContext* browser_context,
                        int renderer_child_id);

}  

#endif  
