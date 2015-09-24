// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_FRAME_TREE_H_
#define CONTENT_BROWSER_FRAME_HOST_FRAME_TREE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/frame_host/frame_tree_node.h"
#include "content/common/content_export.h"

namespace content {

class FrameTreeNode;
class Navigator;
class RenderFrameHostDelegate;
class RenderProcessHost;
class RenderViewHostDelegate;
class RenderViewHostImpl;
class RenderFrameHostManager;
class RenderWidgetHostDelegate;

class CONTENT_EXPORT FrameTree {
 public:
  
  
  
  
  
  
  FrameTree(Navigator* navigator,
            RenderFrameHostDelegate* render_frame_delegate,
            RenderViewHostDelegate* render_view_delegate,
            RenderWidgetHostDelegate* render_widget_delegate,
            RenderFrameHostManager::Delegate* manager_delegate);
  ~FrameTree();

  
  FrameTreeNode* FindByID(int64 frame_tree_node_id);

  
  
  
  void ForEach(const base::Callback<bool(FrameTreeNode*)>& on_node) const;

  
  
  
  
  
  
  
  
  bool IsFirstNavigationAfterSwap() const;
  void OnFirstNavigationAfterSwap(int main_frame_id);

  
  
  RenderFrameHostImpl* AddFrame(int render_frame_host_id,
                                int64 parent_frame_tree_node_id,
                                int64 frame_id,
                                const std::string& frame_name);
  void RemoveFrame(RenderFrameHostImpl* render_frame_host,
                   int64 parent_frame_id,
                   int64 frame_id);
  void SetFrameUrl(int64 frame_id, const GURL& url);

  
  
  
  
  
  
  
  
  
  
  
  void SwapMainFrame(RenderFrameHostImpl* main_frame);

  
  RenderFrameHostImpl* GetMainFrame() const;

  
  
  
  
  void SetFrameRemoveListener(
      const base::Callback<void(RenderViewHostImpl*, int64)>& on_frame_removed);

  FrameTreeNode* root() const { return root_.get(); }

 private:
  
  
  
  FrameTreeNode* FindByFrameID(int64 frame_id);

  scoped_ptr<FrameTreeNode> CreateNode(int64 frame_id,
                                       const std::string& frame_name,
                                       int render_frame_host_id,
                                       FrameTreeNode* parent_node);

  
  
  RenderFrameHostDelegate* render_frame_delegate_;
  RenderViewHostDelegate* render_view_delegate_;
  RenderWidgetHostDelegate* render_widget_delegate_;
  RenderFrameHostManager::Delegate* manager_delegate_;

  scoped_ptr<FrameTreeNode> root_;

  base::Callback<void(RenderViewHostImpl*, int64)> on_frame_removed_;

  DISALLOW_COPY_AND_ASSIGN(FrameTree);
};

}  

#endif  
