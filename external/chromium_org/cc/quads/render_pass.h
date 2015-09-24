// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_QUADS_RENDER_PASS_H_
#define CC_QUADS_RENDER_PASS_H_

#include <utility>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "cc/base/cc_export.h"
#include "cc/base/scoped_ptr_vector.h"
#include "skia/ext/refptr.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/transform.h"

namespace base {
class Value;
};

namespace cc {

class DrawQuad;
class CopyOutputRequest;
class SharedQuadState;

class QuadList : public ScopedPtrVector<DrawQuad> {
 public:
  typedef reverse_iterator BackToFrontIterator;
  typedef const_reverse_iterator ConstBackToFrontIterator;

  inline BackToFrontIterator BackToFrontBegin() { return rbegin(); }
  inline BackToFrontIterator BackToFrontEnd() { return rend(); }
  inline ConstBackToFrontIterator BackToFrontBegin() const { return rbegin(); }
  inline ConstBackToFrontIterator BackToFrontEnd() const { return rend(); }
};

typedef ScopedPtrVector<SharedQuadState> SharedQuadStateList;

class CC_EXPORT RenderPass {
 public:
  struct Id {
    int layer_id;
    int index;

    Id(int layer_id, int index) : layer_id(layer_id), index(index) {}
    void* AsTracingId() const;

    bool operator==(const Id& other) const {
      return layer_id == other.layer_id && index == other.index;
    }
    bool operator!=(const Id& other) const {
      return !(*this == other);
    }
    bool operator<(const Id& other) const {
      return layer_id < other.layer_id ||
          (layer_id == other.layer_id && index < other.index);
    }
  };

  ~RenderPass();

  static scoped_ptr<RenderPass> Create();
  static scoped_ptr<RenderPass> Create(size_t num_layers);

  
  
  scoped_ptr<RenderPass> Copy(Id new_id) const;

  
  static void CopyAll(const ScopedPtrVector<RenderPass>& in,
                      ScopedPtrVector<RenderPass>* out);

  void SetNew(Id id,
              gfx::Rect output_rect,
              gfx::RectF damage_rect,
              const gfx::Transform& transform_to_root_target);

  void SetAll(Id id,
              gfx::Rect output_rect,
              gfx::RectF damage_rect,
              const gfx::Transform& transform_to_root_target,
              bool has_transparent_background);

  scoped_ptr<base::Value> AsValue() const;

  
  Id id;

  
  gfx::Rect output_rect;
  gfx::RectF damage_rect;

  
  
  gfx::Transform transform_to_root_target;

  
  bool has_transparent_background;

  
  
  
  
  ScopedPtrVector<CopyOutputRequest> copy_requests;

  QuadList quad_list;
  SharedQuadStateList shared_quad_state_list;

 protected:
  explicit RenderPass(size_t num_layers);
  RenderPass();

 private:
  DISALLOW_COPY_AND_ASSIGN(RenderPass);
};

}  

namespace BASE_HASH_NAMESPACE {
#if defined(COMPILER_MSVC)
inline size_t hash_value(const cc::RenderPass::Id& key) {
  return base::HashPair(key.layer_id, key.index);
}
#elif defined(COMPILER_GCC)
template<>
struct hash<cc::RenderPass::Id> {
  size_t operator()(cc::RenderPass::Id key) const {
    return base::HashPair(key.layer_id, key.index);
  }
};
#else
#error define a hash function for your compiler
#endif  
}  

namespace cc {
typedef ScopedPtrVector<RenderPass> RenderPassList;
typedef base::hash_map<RenderPass::Id, RenderPass*> RenderPassIdHashMap;
}  

#endif  
