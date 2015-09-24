// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_TEST_RENDER_FRAME_HOST_FACTORY_H_
#define CONTENT_TEST_TEST_RENDER_FRAME_HOST_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/frame_host/render_frame_host_factory.h"

namespace content {

class TestRenderFrameHostFactory : public RenderFrameHostFactory {
 public:
  TestRenderFrameHostFactory();
  virtual ~TestRenderFrameHostFactory();

 protected:
  
  virtual scoped_ptr<RenderFrameHostImpl> CreateRenderFrameHost(
      RenderViewHostImpl* render_view_host,
      RenderFrameHostDelegate* delegate,
      FrameTree* frame_tree,
      FrameTreeNode* frame_tree_node,
      int routing_id,
      bool is_swapped_out) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(TestRenderFrameHostFactory);
};

}  

#endif  
