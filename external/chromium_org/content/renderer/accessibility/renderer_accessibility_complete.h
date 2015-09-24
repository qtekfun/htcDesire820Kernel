// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ACCESSIBILITY_RENDERER_ACCESSIBILITY_COMPLETE_H_
#define CONTENT_RENDERER_ACCESSIBILITY_RENDERER_ACCESSIBILITY_COMPLETE_H_

#include <set>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/weak_ptr.h"
#include "content/common/accessibility_node_data.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/renderer/accessibility/renderer_accessibility.h"
#include "third_party/WebKit/public/web/WebAXEnums.h"
#include "third_party/WebKit/public/web/WebAXObject.h"

namespace blink {
class WebDocument;
class WebNode;
};

namespace content {
class RenderViewImpl;

class CONTENT_EXPORT RendererAccessibilityComplete
    : public RendererAccessibility {
 public:
  explicit RendererAccessibilityComplete(RenderViewImpl* render_view);
  virtual ~RendererAccessibilityComplete();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void FocusedNodeChanged(const blink::WebNode& node) OVERRIDE;
  virtual void DidFinishLoad(blink::WebFrame* frame) OVERRIDE;

  
  virtual void HandleWebAccessibilityEvent(
      const blink::WebAXObject& obj, blink::WebAXEvent event) OVERRIDE;

  
  
  
  struct CONTENT_EXPORT BrowserTreeNode {
    BrowserTreeNode();
    virtual ~BrowserTreeNode();
    int32 id;
    gfx::Rect location;
    BrowserTreeNode* parent;
    std::vector<BrowserTreeNode*> children;
  };

  virtual BrowserTreeNode* CreateBrowserTreeNode();

 protected:
  
  void SendPendingAccessibilityEvents();

  
  
  
  
  void AppendLocationChangeEvents(
      std::vector<AccessibilityHostMsg_EventParams>* events);

 private:
  
  
  
  
  
  void SerializeChangedNodes(const blink::WebAXObject& obj,
                             std::vector<AccessibilityNodeData>* dst,
                             std::set<int>* ids_serialized);

  
  
  void ClearBrowserTreeNode(BrowserTreeNode* browser_node);

  
  void OnDoDefaultAction(int acc_obj_id);
  void OnEventsAck();
  void OnChangeScrollPosition(int acc_obj_id, int scroll_x, int scroll_y);
  void OnScrollToMakeVisible(int acc_obj_id, gfx::Rect subfocus);
  void OnScrollToPoint(int acc_obj_id, gfx::Point point);
  void OnSetFocus(int acc_obj_id);
  void OnSetTextSelection(int acc_obj_id, int start_offset, int end_offset);
  void OnFatalError();

  
  bool IsEditableText(const blink::WebAXObject& node);

  
  
  
  void RecursiveAddEditableTextNodesToTree(
      const blink::WebAXObject& src,
      AccessibilityNodeData* dst);

  
  
  
  void BuildAccessibilityTree(const blink::WebAXObject& src,
                              bool include_children,
                              AccessibilityNodeData* dst);

  
  base::WeakPtrFactory<RendererAccessibilityComplete> weak_factory_;

  
  
  std::vector<AccessibilityHostMsg_EventParams> pending_events_;

  
  BrowserTreeNode* browser_root_;

  
  base::hash_map<int32, BrowserTreeNode*> browser_id_map_;

  
  
  
  gfx::Size last_scroll_offset_;

  
  AccessibilityMode mode_;

  
  bool ack_pending_;

  
  bool logging_;

  DISALLOW_COPY_AND_ASSIGN(RendererAccessibilityComplete);
};

#endif  

}  
