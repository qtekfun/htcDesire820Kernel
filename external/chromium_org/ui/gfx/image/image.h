// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GFX_IMAGE_IMAGE_H_
#define UI_GFX_IMAGE_IMAGE_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted_memory.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_MACOSX) && !defined(OS_IOS)
typedef struct CGColorSpace* CGColorSpaceRef;
#endif

class SkBitmap;

namespace {
class ImageTest;
class ImageMacTest;
}

namespace gfx {
struct ImagePNGRep;
class ImageSkia;
class Size;

#if defined(TOOLKIT_GTK)
class CairoCachedSurface;
#endif

namespace internal {
class ImageRep;
class ImageStorage;
}

class GFX_EXPORT Image {
 public:
  enum RepresentationType {
    kImageRepGdk,
    kImageRepCocoa,
    kImageRepCocoaTouch,
    kImageRepCairo,
    kImageRepSkia,
    kImageRepPNG,
  };

  typedef std::map<RepresentationType, internal::ImageRep*> RepresentationMap;

  
  Image();

  
  
  explicit Image(const std::vector<ImagePNGRep>& image_reps);

  
  
  explicit Image(const ImageSkia& image);

#if defined(TOOLKIT_GTK)
  
  explicit Image(GdkPixbuf* pixbuf);
#elif defined(OS_IOS)
  
  explicit Image(UIImage* image);
#elif defined(OS_MACOSX)
  
  
  
  explicit Image(NSImage* image);
#endif

  
  Image(const Image& other);

  
  Image& operator=(const Image& other);

  
  
  ~Image();

  
  
  
  static Image CreateFrom1xBitmap(const SkBitmap& bitmap);

  
  
  
  
  
  static Image CreateFrom1xPNGBytes(const unsigned char* input,
                                    size_t input_size);

  
  
  
  const SkBitmap* ToSkBitmap() const;
  const ImageSkia* ToImageSkia() const;
#if defined(TOOLKIT_GTK)
  GdkPixbuf* ToGdkPixbuf() const;
  CairoCachedSurface* const ToCairo() const;
#elif defined(OS_IOS)
  UIImage* ToUIImage() const;
#elif defined(OS_MACOSX)
  NSImage* ToNSImage() const;
#endif

  
  
  
  
  scoped_refptr<base::RefCountedMemory> As1xPNGBytes() const;

  
  SkBitmap AsBitmap() const;

  
  
  ImageSkia AsImageSkia() const;

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  NSImage* AsNSImage() const;
#endif

  
  
  
  
  
  
  scoped_refptr<base::RefCountedMemory> Copy1xPNGBytes() const;
  ImageSkia* CopyImageSkia() const;
  SkBitmap* CopySkBitmap() const;
#if defined(TOOLKIT_GTK)
  GdkPixbuf* CopyGdkPixbuf() const;
#elif defined(OS_IOS)
  UIImage* CopyUIImage() const;
#elif defined(OS_MACOSX)
  NSImage* CopyNSImage() const;
#endif

  
  bool HasRepresentation(RepresentationType type) const;

  
  size_t RepresentationCount() const;

  
  bool IsEmpty() const;

  
  int Width() const;
  int Height() const;
  gfx::Size Size() const;

  
  void SwapRepresentations(gfx::Image* other);

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  
  void SetSourceColorSpace(CGColorSpaceRef color_space);
#endif  

 private:
  
  RepresentationType DefaultRepresentationType() const;

  
  
  internal::ImageRep* GetRepresentation(
      RepresentationType rep_type, bool must_exist) const;

  
  void AddRepresentation(internal::ImageRep* rep) const;

  
  
  scoped_refptr<internal::ImageStorage> storage_;

  friend class ::ImageTest;
  friend class ::ImageMacTest;
};

}  

#endif  
