// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ICON_UTIL_H_
#define UI_GFX_ICON_UTIL_H_

#include <windows.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

namespace base {
class FilePath;
}

namespace gfx {
class ImageFamily;
class Size;
}
class SkBitmap;

class GFX_EXPORT IconUtil {
 public:
  
  static const int kLargeIconSize = 256;
  
  
  
  static const int kMediumIconSize = 48;

  
  
  
  static const int kIconDimensions[];

  
  static const size_t kNumIconDimensions;
  
  static const size_t kNumIconDimensionsUpToMediumSize;

  
  
  
  
  
  
  static HICON CreateHICONFromSkBitmap(const SkBitmap& bitmap);

  
  
  
  
  
  
  
  
  
  static SkBitmap* CreateSkBitmapFromHICON(HICON icon, const gfx::Size& s);

  
  
  
  
  static scoped_ptr<SkBitmap> CreateSkBitmapFromIconResource(HMODULE module,
                                                             int resource_id,
                                                             int size);

  
  
  
  
  
  
  
  static SkBitmap* CreateSkBitmapFromHICON(HICON icon);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool CreateIconFileFromImageFamily(
      const gfx::ImageFamily& image_family,
      const base::FilePath& icon_path);

  
  
  static HICON CreateCursorFromDIB(const gfx::Size& icon_size,
                                   const gfx::Point& hotspot,
                                   const void* dib_bits,
                                   size_t dib_size);

 private:
  
  
  
  
  
#pragma pack(push)
#pragma pack(2)

  
  
  struct ICONDIRENTRY {
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    DWORD dwImageOffset;
  };

  
  
  struct ICONDIR {
    WORD idReserved;
    WORD idType;
    WORD idCount;
    ICONDIRENTRY idEntries[1];
  };

  
  
  struct GRPICONDIRENTRY {
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    WORD nID;
  };

  
  
  struct GRPICONDIR {
    WORD idReserved;
    WORD idType;
    WORD idCount;
    GRPICONDIRENTRY idEntries[1];
  };

  
  struct ICONIMAGE {
    BITMAPINFOHEADER icHeader;
    RGBQUAD icColors[1];
    BYTE icXOR[1];
    BYTE icAND[1];
  };
#pragma pack(pop)

  friend class IconUtilTest;

  
  
  static const int kResourceTypeIcon = 1;

  
  static bool PixelsHaveAlpha(const uint32* pixels, size_t num_pixels);

  
  
  static void InitializeBitmapHeader(BITMAPV5HEADER* header, int width,
                                     int height);

  
  
  
  
  
  
  
  
  
  // number of image bytes written to the buffer. Note that the number of bytes
  // includes only the image data written into the memory pointed to by
  
  static void SetSingleIconImageInformation(const SkBitmap& bitmap,
                                            size_t index,
                                            ICONDIR* icon_dir,
                                            ICONIMAGE* icon_image,
                                            size_t image_offset,
                                            size_t* image_byte_count);

  
  
  static void CopySkBitmapBitsIntoIconBuffer(const SkBitmap& bitmap,
                                             unsigned char* buffer,
                                             size_t buffer_size);

  
  
  
  static size_t ComputeIconFileBufferSize(const std::vector<SkBitmap>& set);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void ComputeBitmapSizeComponents(const SkBitmap& bitmap,
                                          size_t* xor_mask_size,
                                          size_t* bytes_in_resource);

  
  static SkBitmap CreateSkBitmapFromHICONHelper(HICON icon,
                                                const gfx::Size& s);

  
  
  DISALLOW_IMPLICIT_CONSTRUCTORS(IconUtil);
};

#endif  
