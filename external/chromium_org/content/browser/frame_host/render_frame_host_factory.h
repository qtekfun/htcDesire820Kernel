// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_RENDER_FRAME_HOST_FACTORY_H_
#define CONTENT_BROWSER_FRAME_HOST_RENDER_FRAME_HOST_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

namespace content {

class FrameTree;
class FrameTreeNode;
class RenderFrameHostDelegate;
class RenderFrameHostImpl;
class RenderViewHostImpl;

class CONTENT_EXPORT RenderFrameHostFactory {
 public:
  
  
  static scoped_ptr<RenderFrameHostImpl> Create(
      RenderViewHostImpl* render_view_host,
      RenderFrameHostDelegate* delegate,
      FrameTree* frame_tree,
      FrameTreeNode* frame_tree_node,
      int routing_id,
      bool is_swapped_out);

  
  static bool has_factory() { return !!factory_; }

 protected:
  RenderFrameHostFactory() {}
  virtual ~RenderFrameHostFactory() {}

  
  
  virtual scoped_ptr<RenderFrameHostImpl> CreateRenderFrameHost(
      RenderViewHostImpl* render_view_host,
      RenderFrameHostDelegate* delegate,
      FrameTree* frame_tree,
      FrameTreeNode* frame_tree_node,
      int routing_id,
      bool is_swapped_out) = 0;

  
  
  
  static void RegisterFactory(RenderFrameHostFactory* factory);

  
  
  static void UnregisterFactory();

 private:
  
  
  static RenderFrameHostFactory* factory_;

  DISALLOW_COPY_AND_ASSIGN(RenderFrameHostFactory);
};

}  

#endif  
