// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_WEB_APPLICATION_INFO_H_
#define CHROME_COMMON_WEB_APPLICATION_INFO_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

struct WebApplicationInfo {
  struct IconInfo {
    GURL url;
    int width;
    int height;
    SkBitmap data;
  };

  WebApplicationInfo();
  ~WebApplicationInfo();

  
  
  
  GURL manifest_url;

  
  
  
  
  
  bool is_bookmark_app;

  
  base::string16 title;

  
  base::string16 description;

  
  GURL app_url;

  
  std::vector<IconInfo> icons;

  
  std::vector<std::string> permissions;

  
  
  std::vector<GURL> urls;

  
  
  std::string launch_container;

  
  bool is_offline_enabled;
};

#endif  
