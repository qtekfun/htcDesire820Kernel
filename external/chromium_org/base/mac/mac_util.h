// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_MAC_UTIL_H_
#define BASE_MAC_MAC_UTIL_H_

#include <AvailabilityMacros.h>
#include <Carbon/Carbon.h>
#include <string>

#include "base/base_export.h"
#include "base/logging.h"

#include "base/mac/foundation_util.h"

#if defined(__OBJC__)
#import <Foundation/Foundation.h>
#else  
class NSImage;
#endif  

namespace base {

class FilePath;

namespace mac {

enum FullScreenMode {
  kFullScreenModeHideAll = 0,
  kFullScreenModeHideDock = 1,
  kFullScreenModeAutoHideAll = 2,
  kNumFullScreenModes = 3,

  
  
  kFullScreenModeNormal = 10,
};

BASE_EXPORT std::string PathFromFSRef(const FSRef& ref);
BASE_EXPORT bool FSRefFromPath(const std::string& path, FSRef* ref);

BASE_EXPORT CGColorSpaceRef GetSRGBColorSpace();

BASE_EXPORT CGColorSpaceRef GetGenericRGBColorSpace();

BASE_EXPORT CGColorSpaceRef GetSystemColorSpace();

BASE_EXPORT void RequestFullScreen(FullScreenMode mode);

BASE_EXPORT void ReleaseFullScreen(FullScreenMode mode);

BASE_EXPORT void SwitchFullScreenModes(FullScreenMode from_mode,
                                       FullScreenMode to_mode);

BASE_EXPORT void SetCursorVisibility(bool visible);

BASE_EXPORT bool ShouldWindowsMiniaturizeOnDoubleClick();

BASE_EXPORT void ActivateProcess(pid_t pid);

BASE_EXPORT bool AmIForeground();

BASE_EXPORT bool SetFileBackupExclusion(const FilePath& file_path);

BASE_EXPORT bool CheckLoginItemStatus(bool* is_hidden);

BASE_EXPORT void AddToLoginItems(bool hide_on_startup);

BASE_EXPORT void RemoveFromLoginItems();

BASE_EXPORT bool WasLaunchedAsLoginOrResumeItem();

BASE_EXPORT bool WasLaunchedAsHiddenLoginItem();

BASE_EXPORT bool RemoveQuarantineAttribute(const FilePath& file_path);


BASE_EXPORT bool IsOSSnowLeopard();

BASE_EXPORT bool IsOSLion();
BASE_EXPORT bool IsOSLionOrEarlier();
BASE_EXPORT bool IsOSLionOrLater();

BASE_EXPORT bool IsOSMountainLion();
BASE_EXPORT bool IsOSMountainLionOrEarlier();
BASE_EXPORT bool IsOSMountainLionOrLater();

BASE_EXPORT bool IsOSMavericks();
BASE_EXPORT bool IsOSMavericksOrLater();

BASE_EXPORT bool IsOSLaterThanMavericks_DontCallThis();

inline bool IsOSLionOrEarlier() { return !IsOSMountainLionOrLater(); }
inline bool IsOSMountainLionOrEarlier() { return !IsOSMavericksOrLater(); }


#if defined(MAC_OS_X_VERSION_10_7) && \
    MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7
#define BASE_MAC_MAC_UTIL_H_INLINED_GE_10_7
inline bool IsOSSnowLeopard() { return false; }
inline bool IsOSLionOrLater() { return true; }
#endif

#if defined(MAC_OS_X_VERSION_10_7) && \
    MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_7
#define BASE_MAC_MAC_UTIL_H_INLINED_GT_10_7
inline bool IsOSLion() { return false; }
#endif

#if defined(MAC_OS_X_VERSION_10_8) && \
    MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_8
#define BASE_MAC_MAC_UTIL_H_INLINED_GE_10_8
inline bool IsOSMountainLionOrLater() { return true; }
#endif

#if defined(MAC_OS_X_VERSION_10_8) && \
    MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_8
#define BASE_MAC_MAC_UTIL_H_INLINED_GT_10_8
inline bool IsOSMountainLion() { return false; }
#endif

#if defined(MAC_OS_X_VERSION_10_9) && \
    MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_9
#define BASE_MAC_MAC_UTIL_H_INLINED_GE_10_9
inline bool IsOSMavericksOrLater() { return true; }
#endif

#if defined(MAC_OS_X_VERSION_10_9) && \
    MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_9
#define BASE_MAC_MAC_UTIL_H_INLINED_GT_10_9
inline bool IsOSMavericks() { return false; }
inline bool IsOSLaterThanMavericks_DontCallThis() { return true; }
#endif

BASE_EXPORT std::string GetModelIdentifier();

BASE_EXPORT bool ParseModelIdentifier(const std::string& ident,
                                      std::string* type,
                                      int32* major,
                                      int32* minor);

}  
}  

#endif  
