// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_RENDER_PASS_TEST_UTILS_H_
#define CC_TEST_RENDER_PASS_TEST_UTILS_H_

#include "cc/base/scoped_ptr_vector.h"
#include "cc/output/filter_operations.h"
#include "cc/quads/render_pass.h"
#include "cc/resources/resource_provider.h"
#include "third_party/skia/include/core/SkColor.h"

namespace gfx {
class Rect;
class Transform;
}

namespace cc {

class SolidColorDrawQuad;
class TestRenderPass;

TestRenderPass* AddRenderPass(
    RenderPassList* pass_list,
    RenderPass::Id id,
    gfx::Rect output_rect,
    const gfx::Transform& root_transform);

SolidColorDrawQuad* AddQuad(TestRenderPass* pass,
                            gfx::Rect rect,
                            SkColor color);

SolidColorDrawQuad* AddClippedQuad(TestRenderPass* pass,
                                   gfx::Rect rect,
                                   SkColor color);

SolidColorDrawQuad* AddTransformedQuad(TestRenderPass* pass,
                                       gfx::Rect rect,
                                       SkColor color,
                                       const gfx::Transform& transform);

void AddRenderPassQuad(TestRenderPass* to_pass,
                       TestRenderPass* contributing_pass);

void AddRenderPassQuad(TestRenderPass* toPass,
                       TestRenderPass* contributing_pass,
                       ResourceProvider::ResourceId mask_resource_id,
                       const FilterOperations& filters,
                       gfx::Transform transform);

}  

#endif  
