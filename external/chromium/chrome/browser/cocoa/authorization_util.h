// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COCOA_AUTHORIZATION_UTIL_H_
#define CHROME_BROWSER_COCOA_AUTHORIZATION_UTIL_H_
#pragma once


#include <CoreFoundation/CoreFoundation.h>
#include <Security/Authorization.h>
#include <stdio.h>
#include <sys/types.h>

namespace authorization_util {

AuthorizationRef AuthorizationCreateToRunAsRoot(CFStringRef prompt);

OSStatus ExecuteWithPrivilegesAndGetPID(AuthorizationRef authorization,
                                        const char* tool_path,
                                        AuthorizationFlags options,
                                        const char** arguments,
                                        FILE** pipe,
                                        pid_t* pid);

OSStatus ExecuteWithPrivilegesAndWait(AuthorizationRef authorization,
                                      const char* tool_path,
                                      AuthorizationFlags options,
                                      const char** arguments,
                                      FILE** pipe,
                                      int* exit_status);

}  

#endif  
