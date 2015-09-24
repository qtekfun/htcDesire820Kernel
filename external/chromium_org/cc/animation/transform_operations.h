// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_ANIMATION_TRANSFORM_OPERATIONS_H_
#define CC_ANIMATION_TRANSFORM_OPERATIONS_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "cc/animation/transform_operation.h"
#include "cc/base/cc_export.h"
#include "ui/gfx/transform.h"

namespace gfx {
class BoxF;
struct DecomposedTransform;
}

namespace cc {

class CC_EXPORT TransformOperations {
 public:
  TransformOperations();
  TransformOperations(const TransformOperations& other);
  ~TransformOperations();

  
  gfx::Transform Apply() const;

  
  
  
  
  
  
  
  gfx::Transform Blend(const TransformOperations& from,
                       SkMScalar progress) const;

  
  
  
  
  bool BlendedBoundsForBox(const gfx::BoxF& box,
                           const TransformOperations& from,
                           SkMScalar min_progress,
                           SkMScalar max_progress,
                           gfx::BoxF* bounds) const;

  
  
  bool MatchesTypes(const TransformOperations& other) const;

  
  
  
  bool CanBlendWith(const TransformOperations& other) const;

  void AppendTranslate(SkMScalar x, SkMScalar y, SkMScalar z);
  void AppendRotate(SkMScalar x, SkMScalar y, SkMScalar z, SkMScalar degrees);
  void AppendScale(SkMScalar x, SkMScalar y, SkMScalar z);
  void AppendSkew(SkMScalar x, SkMScalar y);
  void AppendPerspective(SkMScalar depth);
  void AppendMatrix(const gfx::Transform& matrix);
  void AppendIdentity();
  bool IsIdentity() const;

 private:
  bool BlendInternal(const TransformOperations& from,
                     SkMScalar progress,
                     gfx::Transform* result) const;

  std::vector<TransformOperation> operations_;

  bool ComputeDecomposedTransform() const;

  
  mutable scoped_ptr<gfx::DecomposedTransform> decomposed_transform_;
  mutable bool decomposed_transform_dirty_;

  DISALLOW_ASSIGN(TransformOperations);
};

}  

#endif  
