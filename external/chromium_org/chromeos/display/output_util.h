// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DISPLAY_OUTPUT_UTIL_H_
#define CHROMEOS_DISPLAY_OUTPUT_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"

typedef unsigned long XID;
typedef XID RRMode;
struct _XRRModeInfo;
typedef _XRRModeInfo XRRModeInfo;
struct _XRRScreenResources;
typedef _XRRScreenResources XRRScreenResources;
struct _XRROutputInfo;
typedef _XRROutputInfo XRROutputInfo;

namespace chromeos {

CHROMEOS_EXPORT std::string GetDisplayName(XID output);

CHROMEOS_EXPORT bool GetOutputOverscanFlag(XID output, bool* flag);

CHROMEOS_EXPORT bool ParseOutputOverscanFlag(const unsigned char* prop,
                                             unsigned long nitems,
                                             bool* flag);

CHROMEOS_EXPORT bool IsInternalOutputName(const std::string& name);

CHROMEOS_EXPORT const XRRModeInfo* FindXRRModeInfo(
    const XRRScreenResources* screen_resources,
    XID mode);

CHROMEOS_EXPORT RRMode FindOutputModeMatchingSize(
    const XRRScreenResources* screen_resources,
    const XRROutputInfo* output_info,
    size_t width,
    size_t height);

namespace test {

CHROMEOS_EXPORT XRRModeInfo CreateModeInfo(int id,
                                           int width,
                                           int height,
                                           bool interlaced,
                                           float refresh_rate);

}  

}  

#endif  
