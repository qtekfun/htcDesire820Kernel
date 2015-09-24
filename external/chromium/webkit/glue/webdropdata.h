// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_WEBDROPDATA_H_
#define WEBKIT_GLUE_WEBDROPDATA_H_

#include <string>
#include <vector>

#include "base/string16.h"
#include "googleurl/src/gurl.h"

struct IDataObject;

namespace WebKit {
class WebDragData;
}

struct WebDropData {
  
  explicit WebDropData(const WebKit::WebDragData&);

  WebDropData();

  ~WebDropData();

  
  GURL url;
  string16 url_title;  

  
  string16 download_metadata;

  
  string16 file_extension;

  
  std::vector<string16> filenames;

  
  string16 plain_text;

  
  
  
  string16 text_html;
  GURL html_base_url;

  
  string16 file_description_filename;
  std::string file_contents;

  
  WebKit::WebDragData ToDragData() const;

  
  
  
  static void PopulateWebDropData(IDataObject* data_object,
                                  WebDropData* drop_data);
};

#endif  
