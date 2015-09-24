// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_QUAD_SINK_H_
#define CC_LAYERS_QUAD_SINK_H_

#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"

namespace cc {

class DrawQuad;
class SharedQuadState;

struct AppendQuadsData;

class CC_EXPORT QuadSink {
 public:
  virtual ~QuadSink() {}

  
  
  
  virtual SharedQuadState* UseSharedQuadState(
      scoped_ptr<SharedQuadState> shared_quad_state) = 0;

  
  
  virtual bool Append(scoped_ptr<DrawQuad> draw_quad,
                      AppendQuadsData* append_quads_data) = 0;
};

}  

#endif  
