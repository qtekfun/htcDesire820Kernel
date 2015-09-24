// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RESOURCE_CONTEXT_IMPL_H_
#define CONTENT_BROWSER_RESOURCE_CONTEXT_IMPL_H_

#include "content/public/browser/resource_context.h"

namespace content {

class ChromeBlobStorageContext;
class StreamContext;
class BrowserContext;
class HostZoomMap;
class URLDataManagerBackend;


ChromeBlobStorageContext* GetChromeBlobStorageContextForResourceContext(
    ResourceContext* resource_context);

StreamContext* GetStreamContextForResourceContext(
    ResourceContext* resource_context);

HostZoomMap* GetHostZoomMapForResourceContext(ResourceContext* context);

URLDataManagerBackend* GetURLDataManagerForResourceContext(
    ResourceContext* context);

void InitializeResourceContext(BrowserContext* browser_context);

}  

#endif  
