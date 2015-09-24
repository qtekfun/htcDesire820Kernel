// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_COMMON_DROP_DATA_H_
#define CONTENT_PUBLIC_COMMON_DROP_DATA_H_

#include <map>
#include <string>
#include <vector>

#include "base/strings/nullable_string16.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT DropData {
  
  struct CONTENT_EXPORT FileInfo {
    FileInfo();
    FileInfo(const base::string16& path, const base::string16& display_name);

    
    base::string16 path;
    
    base::string16 display_name;
  };

  DropData();
  ~DropData();

  
  GURL url;
  base::string16 url_title;  

  
  base::string16 download_metadata;

  
  
  blink::WebReferrerPolicy referrer_policy;

  
  std::vector<FileInfo> filenames;

  
  base::string16 filesystem_id;

  
  base::NullableString16 text;

  
  
  
  base::NullableString16 html;
  GURL html_base_url;

  
  base::string16 file_description_filename;
  std::string file_contents;

  std::map<base::string16, base::string16> custom_data;
};

}  

#endif  
