// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_WIN_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_WIN_H_

#include <oleacc.h>

#include "base/memory/scoped_ptr.h"
#include "base/win/scoped_comptr.h"
#include "content/browser/accessibility/browser_accessibility_manager.h"

namespace content {
class BrowserAccessibilityWin;

class AccessibleHWND;

class CONTENT_EXPORT BrowserAccessibilityManagerWin
    : public BrowserAccessibilityManager {
 public:
  BrowserAccessibilityManagerWin(
      HWND parent_hwnd,
      IAccessible* parent_iaccessible,
      const AccessibilityNodeData& src,
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory = new BrowserAccessibilityFactory());

  virtual ~BrowserAccessibilityManagerWin();

  static AccessibilityNodeData GetEmptyDocument();

  
  HWND parent_hwnd() { return parent_hwnd_; }

  
  IAccessible* parent_iaccessible() { return parent_iaccessible_; }
  void set_parent_iaccessible(IAccessible* parent_iaccessible) {
    parent_iaccessible_ = parent_iaccessible;
  }

  
  void MaybeCallNotifyWinEvent(DWORD event, LONG child_id);

  
  virtual void AddNodeToMap(BrowserAccessibility* node);
  virtual void RemoveNode(BrowserAccessibility* node) OVERRIDE;
  virtual void NotifyAccessibilityEvent(
      blink::WebAXEvent event_type, BrowserAccessibility* node) OVERRIDE;

  
  
  
  void TrackScrollingObject(BrowserAccessibilityWin* node);

  
  
  BrowserAccessibilityWin* GetFromUniqueIdWin(LONG unique_id_win);

  
  void OnAccessibleHwndDeleted();

 private:
  
  HWND parent_hwnd_;

  
  IAccessible* parent_iaccessible_;

  
  friend class BrowserAccessibilityManager;

  
  
  
  BrowserAccessibilityWin* tracked_scroll_object_;

  
  
  base::hash_map<long, int32> unique_id_to_renderer_id_map_;

  bool is_chrome_frame_;

  
  AccessibleHWND* accessible_hwnd_;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityManagerWin);
};

}  

#endif  
