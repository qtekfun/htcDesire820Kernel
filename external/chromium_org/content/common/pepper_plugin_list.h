// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_PEPPER_PLUGIN_LIST_H_
#define CONTENT_COMMON_PEPPER_PLUGIN_LIST_H_

#include <vector>

namespace content {
struct PepperPluginInfo;
struct WebPluginInfo;

bool MakePepperPluginInfo(const WebPluginInfo& webplugin_info,
                          PepperPluginInfo* pepper_info);

void ComputePepperPluginList(std::vector<PepperPluginInfo>* plugins);

}  

#endif  
