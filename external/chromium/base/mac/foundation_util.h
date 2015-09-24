// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_FOUNDATION_UTIL_H_
#define BASE_MAC_FOUNDATION_UTIL_H_
#pragma once

#include <string>
#include <vector>

#include "base/logging.h"

#if defined(__OBJC__)
#import <Foundation/Foundation.h>
@class NSBundle;
#else  
class NSBundle;
#endif  

class FilePath;

#if __LP64__ || NS_BUILD_32_LIKE_64
typedef unsigned long NSSearchPathDirectory;
typedef unsigned long NSSearchPathDomainMask;
#else
typedef unsigned int NSSearchPathDirectory;
typedef unsigned int NSSearchPathDomainMask;
#endif

namespace base {
namespace mac {

bool AmIBundled();
void SetOverrideAmIBundled(bool value);

bool IsBackgroundOnlyProcess();

NSBundle* MainAppBundle();
FilePath MainAppBundlePath();

FilePath PathForMainAppBundleResource(CFStringRef resourceName);

void SetOverrideAppBundle(NSBundle* bundle);
void SetOverrideAppBundlePath(const FilePath& file_path);

OSType CreatorCodeForCFBundleRef(CFBundleRef bundle);

OSType CreatorCodeForApplication();

bool GetSearchPathDirectory(NSSearchPathDirectory directory,
                            NSSearchPathDomainMask domain_mask,
                            FilePath* result);

bool GetLocalDirectory(NSSearchPathDirectory directory, FilePath* result);

bool GetUserDirectory(NSSearchPathDirectory directory, FilePath* result);

FilePath GetUserLibraryPath();

FilePath GetAppBundlePath(const FilePath& exec_name);

CFTypeRef GetValueFromDictionary(CFDictionaryRef dict,
                                 CFStringRef key,
                                 CFTypeID expected_type);

void NSObjectRetain(void* obj);
void NSObjectRelease(void* obj);

}  
}  

#endif  
