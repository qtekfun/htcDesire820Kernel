// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_IMPORTED_FAVICON_USAGE_H_
#define CHROME_COMMON_IMPORTER_IMPORTED_FAVICON_USAGE_H_

#include <set>
#include <vector>

#include "url/gurl.h"

struct ImportedFaviconUsage {
  ImportedFaviconUsage();
  ~ImportedFaviconUsage();

  
  GURL favicon_url;

  
  std::vector<unsigned char> png_data;

  
  std::set<GURL> urls;
};

#endif  
