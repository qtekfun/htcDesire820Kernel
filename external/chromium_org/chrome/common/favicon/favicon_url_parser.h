// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_FAVICON_FAVICON_URL_PARSER_H_
#define CHROME_COMMON_FAVICON_FAVICON_URL_PARSER_H_

#include <string>

#include "ui/base/layout.h"

namespace chrome {

struct ParsedFaviconPath {
  
  bool is_icon_url;

  
  std::string url;

  
  int size_in_dip;

  
  ui::ScaleFactor scale_factor;

  
  
  size_t path_index;
};

bool ParseFaviconPath(const std::string& path,
                      int icon_types,
                      ParsedFaviconPath* parsed);

}  

#endif  
