// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_FRAME_TREE_NODE_H_
#define CONTENT_BROWSER_FRAME_HOST_FRAME_TREE_NODE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "content/browser/frame_host/render_frame_host_impl.h"
#include "content/browser/frame_host/render_frame_host_manager.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

class Navigator;
class RenderFrameHostImpl;

class CONTENT_EXPORT FrameTreeNode {
 public:
  static const int64 kInvalidFrameId;

  FrameTreeNode(Navigator* navigator,
                RenderFrameHostDelegate* render_frame_delegate,
                RenderViewHostDelegate* render_view_delegate,
                RenderWidgetHostDelegate* render_widget_delegate,
                RenderFrameHostManager::Delegate* manager_delegate,
                int64 frame_id,
                const std::string& name);

  ~FrameTreeNode();

  void AddChild(scoped_ptr<FrameTreeNode> child);
  void RemoveChild(FrameTreeNode* child);

  
  
  void set_render_frame_host(
      RenderFrameHostImpl* render_frame_host,
      bool owns_render_frame_host) {
    render_frame_host_ = render_frame_host;
    owns_render_frame_host_ = owns_render_frame_host;
  }

  
  
  
  
  
  
  
  
  void ResetForMainFrame(RenderFrameHostImpl* new_render_frame_host);

  Navigator* navigator() {
    return navigator_.get();
  }

  RenderFrameHostManager* render_manager() {
    return &render_manager_;
  }

  int64 frame_tree_node_id() const {
    return frame_tree_node_id_;
  }

  
  
  void set_frame_id(int64 frame_id) {
    DCHECK_EQ(frame_id_, kInvalidFrameId);
    frame_id_ = frame_id;
  }
  int64 frame_id() const {
    return frame_id_;
  }

  const std::string& frame_name() const {
    return frame_name_;
  }

  size_t child_count() const {
    return children_.size();
  }

  FrameTreeNode* child_at(size_t index) const {
    return children_[index];
  }

  const GURL& current_url() const {
    return current_url_;
  }

  void set_current_url(const GURL& url) {
    current_url_ = url;
  }

  RenderFrameHostImpl* render_frame_host() const {
    return render_frame_host_;
  }

 private:
  
  static int64 next_frame_tree_node_id_;

  
  
  scoped_refptr<Navigator> navigator_;

  
  
  
  
  
  RenderFrameHostManager render_manager_;

  
  
  const int64 frame_tree_node_id_;

  
  
  
  
  int64 frame_id_;

  
  
  std::string frame_name_;

  
  ScopedVector<FrameTreeNode> children_;

  
  
  
  
  
  
  
  
  
  bool owns_render_frame_host_;

  
  
  
  RenderFrameHostImpl* render_frame_host_;

  
  
  
  
  GURL current_url_;

  DISALLOW_COPY_AND_ASSIGN(FrameTreeNode);
};

}  

#endif  
