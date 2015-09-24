// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_FOUNDATION_UTIL_H_
#define BASE_MAC_FOUNDATION_UTIL_H_

#include <CoreFoundation/CoreFoundation.h>

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/logging.h"
#include "base/mac/scoped_cftyperef.h"

#if defined(__OBJC__)
#import <Foundation/Foundation.h>
@class NSFont;
@class UIFont;
#else  
#include <CoreFoundation/CoreFoundation.h>
class NSBundle;
class NSFont;
class NSString;
class UIFont;
#endif  

#if defined(OS_IOS)
#include <CoreText/CoreText.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#endif

#if __has_extension(cxx_strong_enums) && \
    (defined(OS_IOS) || (defined(MAC_OS_X_VERSION_10_8) && \
                         MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_8))
#define CR_FORWARD_ENUM(_type, _name) enum _name : _type _name
#else
#define CR_FORWARD_ENUM(_type, _name) _type _name
#endif

#if __LP64__ || NS_BUILD_32_LIKE_64
typedef CR_FORWARD_ENUM(unsigned long, NSSearchPathDirectory);
typedef unsigned long NSSearchPathDomainMask;
#else
typedef CR_FORWARD_ENUM(unsigned int, NSSearchPathDirectory);
typedef unsigned int NSSearchPathDomainMask;
#endif

typedef struct OpaqueSecTrustRef* SecACLRef;
typedef struct OpaqueSecTrustedApplicationRef* SecTrustedApplicationRef;

namespace base {

class FilePath;

namespace mac {

BASE_EXPORT bool AmIBundled();
BASE_EXPORT void SetOverrideAmIBundled(bool value);

BASE_EXPORT bool IsBackgroundOnlyProcess();

BASE_EXPORT FilePath PathForFrameworkBundleResource(CFStringRef resourceName);

OSType CreatorCodeForCFBundleRef(CFBundleRef bundle);

BASE_EXPORT OSType CreatorCodeForApplication();

BASE_EXPORT bool GetSearchPathDirectory(NSSearchPathDirectory directory,
                                        NSSearchPathDomainMask domain_mask,
                                        FilePath* result);

BASE_EXPORT bool GetLocalDirectory(NSSearchPathDirectory directory,
                                   FilePath* result);

BASE_EXPORT bool GetUserDirectory(NSSearchPathDirectory directory,
                                  FilePath* result);

BASE_EXPORT FilePath GetUserLibraryPath();

BASE_EXPORT FilePath GetAppBundlePath(const FilePath& exec_name);

#define TYPE_NAME_FOR_CF_TYPE_DECL(TypeCF) \
BASE_EXPORT std::string TypeNameForCFType(TypeCF##Ref);

TYPE_NAME_FOR_CF_TYPE_DECL(CFArray);
TYPE_NAME_FOR_CF_TYPE_DECL(CFBag);
TYPE_NAME_FOR_CF_TYPE_DECL(CFBoolean);
TYPE_NAME_FOR_CF_TYPE_DECL(CFData);
TYPE_NAME_FOR_CF_TYPE_DECL(CFDate);
TYPE_NAME_FOR_CF_TYPE_DECL(CFDictionary);
TYPE_NAME_FOR_CF_TYPE_DECL(CFNull);
TYPE_NAME_FOR_CF_TYPE_DECL(CFNumber);
TYPE_NAME_FOR_CF_TYPE_DECL(CFSet);
TYPE_NAME_FOR_CF_TYPE_DECL(CFString);
TYPE_NAME_FOR_CF_TYPE_DECL(CFURL);
TYPE_NAME_FOR_CF_TYPE_DECL(CFUUID);

TYPE_NAME_FOR_CF_TYPE_DECL(CGColor);

TYPE_NAME_FOR_CF_TYPE_DECL(CTFont);
TYPE_NAME_FOR_CF_TYPE_DECL(CTRun);

#undef TYPE_NAME_FOR_CF_TYPE_DECL

BASE_EXPORT void NSObjectRetain(void* obj);
BASE_EXPORT void NSObjectRelease(void* obj);

BASE_EXPORT void* CFTypeRefToNSObjectAutorelease(CFTypeRef cf_object);

BASE_EXPORT const char* BaseBundleID();

BASE_EXPORT void SetBaseBundleID(const char* new_base_bundle_id);

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
BASE_EXPORT TypeNS* CFToNSCast(TypeCF##Ref cf_val); \
BASE_EXPORT TypeCF##Ref NSToCFCast(TypeNS* ns_val); \
} \
}

#define CF_TO_NS_MUTABLE_CAST_DECL(name) \
CF_TO_NS_CAST_DECL(CF##name, NS##name) \
OBJC_CPP_CLASS_DECL(NSMutable##name) \
\
namespace base { \
namespace mac { \
BASE_EXPORT NSMutable##name* CFToNSCast(CFMutable##name##Ref cf_val); \
BASE_EXPORT CFMutable##name##Ref NSToCFCast(NSMutable##name* ns_val); \
} \
}


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

#if defined(OS_IOS)
CF_TO_NS_CAST_DECL(CTFont, UIFont);
#else
CF_TO_NS_CAST_DECL(CTFont, NSFont);
#endif

#undef CF_TO_NS_CAST_DECL
#undef CF_TO_NS_MUTABLE_CAST_DECL
#undef OBJC_CPP_CLASS_DECL

namespace base {
namespace mac {


template<typename T>
T CFCast(const CFTypeRef& cf_val);

template<typename T>
T CFCastStrict(const CFTypeRef& cf_val);

#define CF_CAST_DECL(TypeCF) \
template<> BASE_EXPORT TypeCF##Ref \
CFCast<TypeCF##Ref>(const CFTypeRef& cf_val);\
\
template<> BASE_EXPORT TypeCF##Ref \
CFCastStrict<TypeCF##Ref>(const CFTypeRef& cf_val);

CF_CAST_DECL(CFArray);
CF_CAST_DECL(CFBag);
CF_CAST_DECL(CFBoolean);
CF_CAST_DECL(CFData);
CF_CAST_DECL(CFDate);
CF_CAST_DECL(CFDictionary);
CF_CAST_DECL(CFNull);
CF_CAST_DECL(CFNumber);
CF_CAST_DECL(CFSet);
CF_CAST_DECL(CFString);
CF_CAST_DECL(CFURL);
CF_CAST_DECL(CFUUID);

CF_CAST_DECL(CGColor);

CF_CAST_DECL(CTFont);
CF_CAST_DECL(CTRun);

CF_CAST_DECL(SecACL);
CF_CAST_DECL(SecTrustedApplication);

#undef CF_CAST_DECL

#if defined(__OBJC__)

template<typename T>
T* ObjCCast(id objc_val) {
  if ([objc_val isKindOfClass:[T class]]) {
    return reinterpret_cast<T*>(objc_val);
  }
  return nil;
}

template<typename T>
T* ObjCCastStrict(id objc_val) {
  T* rv = ObjCCast<T>(objc_val);
  DCHECK(objc_val == nil || rv);
  return rv;
}

#endif  

BASE_EXPORT std::string GetValueFromDictionaryErrorMessage(
    CFStringRef key, const std::string& expected_type, CFTypeRef value);

template<typename T>
T GetValueFromDictionary(CFDictionaryRef dict, CFStringRef key) {
  CFTypeRef value = CFDictionaryGetValue(dict, key);
  T value_specific = CFCast<T>(value);

  if (value && !value_specific) {
    std::string expected_type = TypeNameForCFType(value_specific);
    DLOG(WARNING) << GetValueFromDictionaryErrorMessage(key,
                                                        expected_type,
                                                        value);
  }

  return value_specific;
}

BASE_EXPORT NSString* FilePathToNSString(const FilePath& path);

BASE_EXPORT FilePath NSStringToFilePath(NSString* str);

}  
}  

BASE_EXPORT extern std::ostream& operator<<(std::ostream& o,
                                            const CFErrorRef err);
BASE_EXPORT extern std::ostream& operator<<(std::ostream& o,
                                            const CFStringRef str);

#endif  
