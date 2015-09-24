// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_PLUGIN_CONSTANTS_WIN_H_
#define CONTENT_COMMON_PLUGIN_CONSTANTS_WIN_H_

#include "base/strings/string16.h"

#if !defined(OS_WIN)
#error "Windows-only header"
#endif

namespace content {

extern const char16 kNativeWindowClassName[];

extern const char16 kWrapperNativeWindowClassName[];

extern const char16 kPluginNameAtomProperty[];
extern const char16 kPluginVersionAtomProperty[];
extern const char16 kDummyActivationWindowName[];

extern const char16 kPaintMessageName[];

extern const char16 kRegistryMozillaPlugins[];

extern const char16 kMozillaActiveXPlugin[];
extern const char16 kNewWMPPlugin[];
extern const char16 kOldWMPPlugin[];
extern const char16 kYahooApplicationStatePlugin[];
extern const char16 kWanWangProtocolHandlerPlugin[];
extern const char16 kFlashPlugin[];
extern const char16 kAcrobatReaderPlugin[];
extern const char16 kRealPlayerPlugin[];
extern const char16 kSilverlightPlugin[];
extern const char16 kJavaPlugin1[];
extern const char16 kJavaPlugin2[];

extern const char kGPUPluginMimeType[];

extern const char16 kPluginDummyParentProperty[];

}  

#endif  
