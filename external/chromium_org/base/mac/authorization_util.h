// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_AUTHORIZATION_UTIL_H_
#define BASE_MAC_AUTHORIZATION_UTIL_H_


#include <CoreFoundation/CoreFoundation.h>
#include <Security/Authorization.h>
#include <stdio.h>
#include <sys/types.h>

#include "base/base_export.h"

namespace base {
namespace mac {

BASE_EXPORT
AuthorizationRef GetAuthorizationRightsWithPrompt(
    AuthorizationRights* rights,
    CFStringRef prompt,
    AuthorizationFlags extraFlags);

BASE_EXPORT
AuthorizationRef AuthorizationCreateToRunAsRoot(CFStringRef prompt);

BASE_EXPORT
OSStatus ExecuteWithPrivilegesAndGetPID(AuthorizationRef authorization,
                                        const char* tool_path,
                                        AuthorizationFlags options,
                                        const char** arguments,
                                        FILE** pipe,
                                        pid_t* pid);

BASE_EXPORT
OSStatus ExecuteWithPrivilegesAndWait(AuthorizationRef authorization,
                                      const char* tool_path,
                                      AuthorizationFlags options,
                                      const char** arguments,
                                      FILE** pipe,
                                      int* exit_status);

}  
}  

#endif  
