// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_DRIVE_COMMON_CALLBACKS_H_
#define GOOGLE_APIS_DRIVE_DRIVE_COMMON_CALLBACKS_H_

#include "google_apis/drive/base_requests.h"

namespace google_apis {

class AboutResource;
class AppList;
class ResourceEntry;
class ResourceList;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<ResourceList> resource_list)>
    GetResourceListCallback;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<ResourceEntry> entry)>
    GetResourceEntryCallback;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<AboutResource> about_resource)>
    AboutResourceCallback;

typedef base::Callback<void(GDataErrorCode error,
                            const GURL& share_url)> GetShareUrlCallback;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<AppList> app_list)> AppListCallback;

typedef base::Callback<void(
    const UploadRangeResponse& response,
    scoped_ptr<ResourceEntry> new_entry)> UploadRangeCallback;

typedef base::Callback<void(GDataErrorCode error,
                            const GURL& open_url)>
    AuthorizeAppCallback;

typedef base::Closure CancelCallback;

}  

#endif  
