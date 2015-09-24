// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_MEDIA_DEVICE_ID_H_
#define CONTENT_PUBLIC_BROWSER_MEDIA_DEVICE_ID_H_

#include <string>

#include "content/common/content_export.h"
#include "content/public/browser/resource_context.h"
#include "url/gurl.h"

namespace content {

CONTENT_EXPORT std::string GetHMACForMediaDeviceID(
    const ResourceContext::SaltCallback& sc,
    const GURL& security_origin,
    const std::string& raw_unique_id);

CONTENT_EXPORT bool DoesMediaDeviceIDMatchHMAC(
    const ResourceContext::SaltCallback& sc,
    const GURL& security_origin,
    const std::string& device_guid,
    const std::string& raw_unique_id);

}  

#endif  
