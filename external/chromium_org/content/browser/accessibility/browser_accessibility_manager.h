// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_H_

#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "content/common/accessibility_node_data.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebAXEnums.h"
#include "ui/gfx/native_widget_types.h"

struct AccessibilityHostMsg_EventParams;

namespace content {
class BrowserAccessibility;
#if defined(OS_ANDROID)
class BrowserAccessibilityManagerAndroid;
#endif
#if defined(OS_WIN)
class BrowserAccessibilityManagerWin;
#endif

class CONTENT_EXPORT BrowserAccessibilityDelegate {
 public:
  virtual ~BrowserAccessibilityDelegate() {}
  virtual void SetAccessibilityFocus(int acc_obj_id) = 0;
  virtual void AccessibilityDoDefaultAction(int acc_obj_id) = 0;
  virtual void AccessibilityScrollToMakeVisible(
      int acc_obj_id, gfx::Rect subfocus) = 0;
  virtual void AccessibilityScrollToPoint(
      int acc_obj_id, gfx::Point point) = 0;
  virtual void AccessibilitySetTextSelection(
      int acc_obj_id, int start_offset, int end_offset) = 0;
  virtual bool HasFocus() const = 0;
  virtual gfx::Rect GetViewBounds() const = 0;
  virtual gfx::Point GetLastTouchEventLocation() const = 0;
  virtual void FatalAccessibilityTreeError() = 0;
};

class CONTENT_EXPORT BrowserAccessibilityFactory {
 public:
  virtual ~BrowserAccessibilityFactory() {}

  
  
  virtual BrowserAccessibility* Create();
};

class CONTENT_EXPORT BrowserAccessibilityManager {
 public:
  
  
  static BrowserAccessibilityManager* Create(
      const AccessibilityNodeData& src,
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory = new BrowserAccessibilityFactory());

  virtual ~BrowserAccessibilityManager();

  void Initialize(const AccessibilityNodeData src);

  static AccessibilityNodeData GetEmptyDocument();

  virtual void NotifyAccessibilityEvent(
      blink::WebAXEvent event_type, BrowserAccessibility* node) { }

  
  BrowserAccessibility* GetRoot();

  
  virtual void RemoveNode(BrowserAccessibility* node);

  
  
  BrowserAccessibility* GetFromRendererID(int32 renderer_id);

  
  
  
  
  
  void GotFocus(bool touch_event_context);

  
  
  void WasHidden();

  
  
  void GotMouseDown();

  
  
  
  void SetFocus(BrowserAccessibility* node, bool notify);

  
  void DoDefaultAction(const BrowserAccessibility& node);

  
  
  
  
  void ScrollToMakeVisible(
      const BrowserAccessibility& node, gfx::Rect subfocus);

  
  
  void ScrollToPoint(
      const BrowserAccessibility& node, gfx::Point point);

  
  void SetTextSelection(
      const BrowserAccessibility& node, int start_offset, int end_offset);

  
  gfx::Rect GetViewBounds();

  
  
  void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params);

#if defined(OS_WIN)
  BrowserAccessibilityManagerWin* ToBrowserAccessibilityManagerWin();
#endif

#if defined(OS_ANDROID)
  BrowserAccessibilityManagerAndroid* ToBrowserAccessibilityManagerAndroid();
#endif

  
  
  BrowserAccessibility* GetFocus(BrowserAccessibility* root);

  
  
  bool IsOSKAllowed(const gfx::Rect& bounds);

  
  
  virtual bool UseRootScrollOffsetsWhenComputingBounds();

  
  
  
  
  void UpdateNodesForTesting(
      const AccessibilityNodeData& node,
      const AccessibilityNodeData& node2 = AccessibilityNodeData(),
      const AccessibilityNodeData& node3 = AccessibilityNodeData(),
      const AccessibilityNodeData& node4 = AccessibilityNodeData(),
      const AccessibilityNodeData& node5 = AccessibilityNodeData(),
      const AccessibilityNodeData& node6 = AccessibilityNodeData(),
      const AccessibilityNodeData& node7 = AccessibilityNodeData());

 protected:
  BrowserAccessibilityManager(
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory);

  BrowserAccessibilityManager(
      const AccessibilityNodeData& src,
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory);

  virtual void AddNodeToMap(BrowserAccessibility* node);

  virtual void NotifyRootChanged() {}

 private:
  
  
  enum OnScreenKeyboardState {
    
    OSK_DISALLOWED_BECAUSE_TAB_HIDDEN,

    
    
    OSK_DISALLOWED_BECAUSE_TAB_JUST_APPEARED,

    
    
    
    
    OSK_ALLOWED_WITHIN_FOCUSED_OBJECT,

    
    
    
    OSK_ALLOWED
  };

  
  
  bool UpdateNodes(const std::vector<AccessibilityNodeData>& nodes);

  
  
  bool UpdateNode(const AccessibilityNodeData& src);

  void SetRoot(BrowserAccessibility* root);

  BrowserAccessibility* CreateNode(
      BrowserAccessibility* parent,
      int32 renderer_id,
      int32 index_in_parent);

 protected:
  
  BrowserAccessibilityDelegate* delegate_;

  
  scoped_ptr<BrowserAccessibilityFactory> factory_;

  
  
  BrowserAccessibility* root_;
  BrowserAccessibility* focus_;

  
  OnScreenKeyboardState osk_state_;

  
  base::hash_map<int32, BrowserAccessibility*> renderer_id_map_;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityManager);
};

}  

#endif  
