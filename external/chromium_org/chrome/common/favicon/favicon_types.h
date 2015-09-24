// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_FAVICON_FAVICON_TYPES_H_
#define CHROME_COMMON_FAVICON_FAVICON_TYPES_H_

#include "base/memory/ref_counted_memory.h"
#include "chrome/common/favicon/favicon_types.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

namespace chrome {

typedef int64 FaviconID;

enum IconType {
  INVALID_ICON = 0x0,
  FAVICON = 1 << 0,
  TOUCH_ICON = 1 << 1,
  TOUCH_PRECOMPOSED_ICON = 1 << 2
};

struct FaviconImageResult {
  FaviconImageResult();
  ~FaviconImageResult();

  
  gfx::Image image;

  
  
  
  
  
  GURL icon_url;
};

struct FaviconBitmapResult {
  FaviconBitmapResult();
  ~FaviconBitmapResult();

  
  bool is_valid() const { return bitmap_data.get() && bitmap_data->size(); }

  
  bool expired;

  
  scoped_refptr<base::RefCountedMemory> bitmap_data;

  
  gfx::Size pixel_size;

  
  GURL icon_url;

  
  IconType icon_type;
};

typedef FaviconBitmapResult FaviconBitmapData;

}  

#endif  
