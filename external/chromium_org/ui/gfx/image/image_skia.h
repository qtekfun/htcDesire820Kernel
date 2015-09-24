// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_SKIA_H_
#define UI_GFX_IMAGE_IMAGE_SKIA_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/image/image_skia_rep.h"

namespace gfx {
class ImageSkiaSource;
class Size;

namespace internal {
class ImageSkiaStorage;
}  

namespace test {
class TestOnThread;
}

class GFX_EXPORT ImageSkia {
 public:
  typedef std::vector<ImageSkiaRep> ImageSkiaReps;

  
  ImageSkia();

  
  
  
  ImageSkia(ImageSkiaSource* source, const gfx::Size& size);

  
  
  
  ImageSkia(ImageSkiaSource* source, float scale);

  explicit ImageSkia(const gfx::ImageSkiaRep& image_rep);

  
  ImageSkia(const ImageSkia& other);

  
  ImageSkia& operator=(const ImageSkia& other);

  ~ImageSkia();

  
  static void SetSupportedScales(const std::vector<float>& scales);

  
  
  static const std::vector<float>& GetSupportedScales();

  
  static float GetMaxSupportedScale();

  
  
  
  
  
  static ImageSkia CreateFrom1xBitmap(const SkBitmap& bitmap);

  
  
  
  
  
  
  
  scoped_ptr<ImageSkia> DeepCopy() const;

  
  
  bool BackedBySameObjectAs(const gfx::ImageSkia& other) const;

  
  void AddRepresentation(const gfx::ImageSkiaRep& image_rep);

  
  void RemoveRepresentation(float scale);

  
  
  bool HasRepresentation(float scale) const;

  
  
  const gfx::ImageSkiaRep& GetRepresentation(float scale) const;

  
  
  
  
  
  
  
  void SetReadOnly();

  
  
  
  
  void MakeThreadSafe();
  bool IsThreadSafe() const;

  
  bool isNull() const { return storage_.get() == NULL; }

  
  int width() const;
  int height() const;
  gfx::Size size() const;

  
  
  
  
  
  const SkBitmap* bitmap() const { return &GetBitmap(); }

  
  
  
  std::vector<gfx::ImageSkiaRep> image_reps() const;

  
  
  
  void EnsureRepsForSupportedScales() const;

 private:
  friend class test::TestOnThread;
  FRIEND_TEST_ALL_PREFIXES(ImageSkiaTest, EmptyOnThreadTest);
  FRIEND_TEST_ALL_PREFIXES(ImageSkiaTest, StaticOnThreadTest);
  FRIEND_TEST_ALL_PREFIXES(ImageSkiaTest, SourceOnThreadTest);

  
  
  void Init(const gfx::ImageSkiaRep& image_rep);

  SkBitmap& GetBitmap() const;

  
  bool CanRead() const;
  bool CanModify() const;

  
  
  void DetachStorageFromThread();

  
  scoped_refptr<internal::ImageSkiaStorage> storage_;
};

}  

#endif  
