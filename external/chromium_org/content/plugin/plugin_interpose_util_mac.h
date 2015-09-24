// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PLUGIN_PLUGIN_INTERPOSE_UTIL_MAC_H_
#define CONTENT_PLUGIN_PLUGIN_INTERPOSE_UTIL_MAC_H_

#include <Carbon/Carbon.h>

#import "base/basictypes.h"

typedef void* OpaquePluginRef;

namespace mac_plugin_interposing {

void SetUpCocoaInterposing();

void SwitchToPluginProcess();

OpaquePluginRef GetActiveDelegate();

void NotifyBrowserOfPluginSelectWindow(uint32 window_id, CGRect bounds,
                                       bool modal);

void NotifyBrowserOfPluginShowWindow(uint32 window_id, CGRect bounds,
                                     bool modal);

void NotifyBrowserOfPluginHideWindow(uint32 window_id, CGRect bounds);

}  

#endif  
