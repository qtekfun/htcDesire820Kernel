// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_PICASA_FINDER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_PICASA_FINDER_H_

#include <string>

#include "base/callback.h"
#include "base/files/file_path.h"

#if defined(OS_MACOSX)

class MacPreferences;
#if defined(__OBJC__)
@class NSString;
#else  
class NSString;
#endif  

#endif  

namespace picasa {

#if defined(OS_WIN)
extern const wchar_t kPicasaRegistryPath[];
extern const wchar_t kPicasaRegistryAppDataPathKey[];
#endif

#if defined(OS_MACOSX)
extern NSString* const kPicasaAppDataPathMacPreferencesKey;
#endif

typedef base::Callback<void(const std::string&)> DeviceIDCallback;

void FindPicasaDatabase(const DeviceIDCallback& callback);

base::FilePath MakePicasaDatabasePath(
    const base::FilePath& picasa_app_data_path);

#if defined(OS_MACOSX)
void SetMacPreferencesForTesting(MacPreferences* preferences);

base::FilePath GetCustomPicasaAppDataPathFromMacPreferences();
#endif  

}  

#endif  
