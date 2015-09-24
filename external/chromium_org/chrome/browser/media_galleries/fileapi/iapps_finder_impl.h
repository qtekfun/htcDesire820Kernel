// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IAPPS_FINDER_IMPL_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IAPPS_FINDER_IMPL_H_

#include <string>

#include "base/callback.h"
#include "chrome/browser/media_galleries/fileapi/iapps_finder.h"
#include "chrome/browser/storage_monitor/storage_info.h"

#if defined(OS_MACOSX)

class MacPreferences;
#if defined(__OBJC__)
@class NSString;
#else  
class NSString;
#endif  

#endif  

namespace iapps {

#if defined(OS_MACOSX)

extern NSString* const kIPhotoRecentDatabasesKey;
extern NSString* const kITunesRecentDatabasePathsKey;

void SetMacPreferencesForTesting(MacPreferences* preferences);

#endif  

typedef base::Callback<void(const IAppsFinderCallback&)> IAppsFinderTask;

void FindIAppsOnFileThread(StorageInfo::Type type, const IAppsFinderTask& task,
                           const IAppsFinderCallback& callback);

}  

#endif  
