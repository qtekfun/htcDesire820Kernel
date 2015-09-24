// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_TRUETYPE_FONT_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_TRUETYPE_FONT_H_

#include <string>
#include <vector>

#include "ppapi/proxy/serialized_structs.h"

namespace content {

class PepperTrueTypeFont {
 public:
  
  
  
  static PepperTrueTypeFont* Create(
      const ppapi::proxy::SerializedTrueTypeFontDesc& desc);
  virtual ~PepperTrueTypeFont() {}

  
  virtual bool IsValid() = 0;

  
  
  
  // failure. 'desc' is written only on success.
  virtual int32_t Describe(ppapi::proxy::SerializedTrueTypeFontDesc* desc) = 0;

  
  
  // written only on success.
  virtual int32_t GetTableTags(std::vector<uint32_t>* tags) = 0;

  
  
  
  // 'data' is written only on success.
  virtual int32_t GetTable(uint32_t table_tag,
                           int32_t offset,
                           int32_t max_data_length,
                           std::string* data) = 0;
};

}  

#endif  
