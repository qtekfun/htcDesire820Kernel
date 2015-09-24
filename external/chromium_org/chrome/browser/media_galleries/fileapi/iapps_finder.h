// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IAPPS_FINDER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IAPPS_FINDER_H_

#include <string>

#include "base/callback.h"

namespace iapps {

typedef base::Callback<void(const std::string&)> IAppsFinderCallback;


void FindIPhotoLibrary(const IAppsFinderCallback& callback);

void FindITunesLibrary(const IAppsFinderCallback& callback);

}  

#endif  
