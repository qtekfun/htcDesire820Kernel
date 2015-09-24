// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_MAC_FONT_LOADER_H_
#define CONTENT_COMMON_MAC_FONT_LOADER_H_

#include <ApplicationServices/ApplicationServices.h>

#include "base/memory/shared_memory.h"
#include "content/common/content_export.h"

#ifdef __OBJC__
@class NSFont;
#else
class NSFont;
#endif

struct FontDescriptor;


class FontLoader {
 public:
  
  
  
  struct Result {
    uint32 font_data_size;
    base::SharedMemory font_data;
    uint32 font_id;
  };
  
  
  
  
  
  
  
  
  
  
  
  CONTENT_EXPORT
  static void LoadFont(const FontDescriptor& font, FontLoader::Result* result);

  
  
  
  
  
  
  
  
  
  
  
  CONTENT_EXPORT
  static bool CGFontRefFromBuffer(base::SharedMemoryHandle font_data,
                                  uint32 font_data_size,
                                  CGFontRef* out);
};

#endif 
