// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_WEB_APPS_H_
#define CHROME_COMMON_WEB_APPS_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/size.h"

namespace WebKit {
class WebDocument;
class WebFrame;
}

class Value;

struct WebApplicationInfo {
  struct IconInfo {
    GURL url;
    int width;
    int height;
    SkBitmap data;
  };

  static const char kInvalidDefinitionURL[];
  static const char kInvalidLaunchURL[];
  static const char kInvalidURL[];
  static const char kInvalidIconSize[];
  static const char kInvalidIconURL[];

  WebApplicationInfo();
  ~WebApplicationInfo();

  
  
  
  GURL manifest_url;

  
  string16 title;

  
  string16 description;

  
  GURL app_url;

  
  std::vector<IconInfo> icons;

  
  std::vector<std::string> permissions;

  
  
  std::vector<GURL> urls;

  
  
  std::string launch_container;
};


namespace web_apps {

gfx::Size ParseIconSize(const string16& text);

bool ParseIconSizes(const string16& text, std::vector<gfx::Size>* sizes,
                    bool* is_any);

bool ParseWebAppFromWebDocument(WebKit::WebFrame* frame,
                                WebApplicationInfo* web_app,
                                string16* error);

bool ParseWebAppFromDefinitionFile(Value* definition,
                                   WebApplicationInfo* web_app,
                                   string16* error);

}  

#endif  
