// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_BASE_RESOURCES_H_
#define REMOTING_HOST_BASE_RESOURCES_H_

#include <string>

namespace remoting {

bool LoadResources(const std::string& pref_locale);

void UnloadResources();

}  

#endif  
