// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_MAC_UTIL_H_
#define BASE_MAC_MAC_UTIL_H_
#pragma once

#include <Carbon/Carbon.h>
#include <string>

#include "base/logging.h"

#include "base/mac/foundation_util.h"

#if defined(__OBJC__)
#import <Foundation/Foundation.h>
#else  
class NSImage;
#endif  

class FilePath;

namespace base {
namespace mac {

enum FullScreenMode {
  kFullScreenModeHideAll = 0,
  kFullScreenModeHideDock = 1,
  kFullScreenModeAutoHideAll = 2,
  kNumFullScreenModes = 3,

  
  
  kFullScreenModeNormal = 10,
};

std::string PathFromFSRef(const FSRef& ref);
bool FSRefFromPath(const std::string& path, FSRef* ref);

CGColorSpaceRef GetSRGBColorSpace();

CGColorSpaceRef GetSystemColorSpace();

void RequestFullScreen(FullScreenMode mode);

void ReleaseFullScreen(FullScreenMode mode);

void SwitchFullScreenModes(FullScreenMode from_mode, FullScreenMode to_mode);

void SetCursorVisibility(bool visible);

bool ShouldWindowsMiniaturizeOnDoubleClick();

void ActivateProcess(pid_t pid);

bool SetFileBackupExclusion(const FilePath& file_path, bool exclude);

void SetProcessName(CFStringRef process_name);

CGImageRef CopyNSImageToCGImage(NSImage* image);

bool CheckLoginItemStatus(bool* is_hidden);

void AddToLoginItems(bool hide_on_startup);

void RemoveFromLoginItems();

bool WasLaunchedAsHiddenLoginItem();

}  
}  

#if !defined(__OBJC__)
#define OBJC_CPP_CLASS_DECL(x) class x;
#else  
#define OBJC_CPP_CLASS_DECL(x)
#endif  


#define CF_TO_NS_CAST_DECL(TypeCF, TypeNS) \
OBJC_CPP_CLASS_DECL(TypeNS) \
\
namespace base { \
namespace mac { \
TypeNS* CFToNSCast(TypeCF##Ref cf_val); \
TypeCF##Ref NSToCFCast(TypeNS* ns_val); \
} \
} \

#define CF_TO_NS_MUTABLE_CAST_DECL(name) \
CF_TO_NS_CAST_DECL(CF##name, NS##name) \
OBJC_CPP_CLASS_DECL(NSMutable##name) \
\
namespace base { \
namespace mac { \
NSMutable##name* CFToNSCast(CFMutable##name##Ref cf_val); \
CFMutable##name##Ref NSToCFCast(NSMutable##name* ns_val); \
} \
} \


CF_TO_NS_MUTABLE_CAST_DECL(Array);
CF_TO_NS_MUTABLE_CAST_DECL(AttributedString);
CF_TO_NS_CAST_DECL(CFCalendar, NSCalendar);
CF_TO_NS_MUTABLE_CAST_DECL(CharacterSet);
CF_TO_NS_MUTABLE_CAST_DECL(Data);
CF_TO_NS_CAST_DECL(CFDate, NSDate);
CF_TO_NS_MUTABLE_CAST_DECL(Dictionary);
CF_TO_NS_CAST_DECL(CFError, NSError);
CF_TO_NS_CAST_DECL(CFLocale, NSLocale);
CF_TO_NS_CAST_DECL(CFNumber, NSNumber);
CF_TO_NS_CAST_DECL(CFRunLoopTimer, NSTimer);
CF_TO_NS_CAST_DECL(CFTimeZone, NSTimeZone);
CF_TO_NS_MUTABLE_CAST_DECL(Set);
CF_TO_NS_CAST_DECL(CFReadStream, NSInputStream);
CF_TO_NS_CAST_DECL(CFWriteStream, NSOutputStream);
CF_TO_NS_MUTABLE_CAST_DECL(String);
CF_TO_NS_CAST_DECL(CFURL, NSURL);

extern std::ostream& operator<<(std::ostream& o, const CFErrorRef err);
extern std::ostream& operator<<(std::ostream& o, const CFStringRef str);

#endif  
