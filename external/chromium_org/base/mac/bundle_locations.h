// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_BUNDLE_LOCATIONS_H_
#define BASE_MAC_BUNDLE_LOCATIONS_H_

#include "base/base_export.h"
#include "base/files/file_path.h"

#if defined(__OBJC__)
#import <Foundation/Foundation.h>
#else  
class NSBundle;
class NSString;
#endif  

namespace base {

class FilePath;

namespace mac {


BASE_EXPORT NSBundle* MainBundle();
BASE_EXPORT FilePath MainBundlePath();
BASE_EXPORT NSBundle* OuterBundle();
BASE_EXPORT FilePath OuterBundlePath();
BASE_EXPORT NSBundle* FrameworkBundle();
BASE_EXPORT FilePath FrameworkBundlePath();

BASE_EXPORT void SetOverrideOuterBundle(NSBundle* bundle);
BASE_EXPORT void SetOverrideFrameworkBundle(NSBundle* bundle);

BASE_EXPORT void SetOverrideOuterBundlePath(const FilePath& file_path);
BASE_EXPORT void SetOverrideFrameworkBundlePath(const FilePath& file_path);

}  
}  

#endif  
