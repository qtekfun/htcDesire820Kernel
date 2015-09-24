// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_H_
#pragma once

#include <vector>

#include "base/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/glue/webaccessibility.h"

class BrowserAccessibility;
#if defined(OS_WIN)
class BrowserAccessibilityManagerWin;
#endif

struct ViewHostMsg_AccessibilityNotification_Params;

using webkit_glue::WebAccessibility;

class BrowserAccessibilityDelegate {
 public:
  virtual ~BrowserAccessibilityDelegate() {}
  virtual void SetAccessibilityFocus(int acc_obj_id) = 0;
  virtual void AccessibilityDoDefaultAction(int acc_obj_id) = 0;
  virtual bool HasFocus() = 0;
  virtual gfx::Rect GetViewBounds() const = 0;
};

class BrowserAccessibilityFactory {
 public:
  virtual ~BrowserAccessibilityFactory() {}

  
  
  virtual BrowserAccessibility* Create();
};

class BrowserAccessibilityManager {
 public:
  
  
  static BrowserAccessibilityManager* Create(
    gfx::NativeView parent_view,
    const WebAccessibility& src,
    BrowserAccessibilityDelegate* delegate,
    BrowserAccessibilityFactory* factory = new BrowserAccessibilityFactory());

  virtual ~BrowserAccessibilityManager();

  
  
  
  virtual void NotifyAccessibilityEvent(
      int type,
      BrowserAccessibility* node) { }

  
  static int32 GetNextChildID();

  
  BrowserAccessibility* GetRoot();

  
  void Remove(int32 child_id, int32 renderer_id);

  
  
  BrowserAccessibility* GetFromChildID(int32 child_id);

  
  
  void GotFocus();

  
  
  
  void SetFocus(BrowserAccessibility* node, bool notify);

  
  void DoDefaultAction(const BrowserAccessibility& node);

  
  gfx::Rect GetViewBounds();

  
  
  void OnAccessibilityNotifications(
      const std::vector<ViewHostMsg_AccessibilityNotification_Params>& params);

  gfx::NativeView GetParentView();

#if defined(OS_WIN)
  BrowserAccessibilityManagerWin* toBrowserAccessibilityManagerWin();
#endif

  
  
  BrowserAccessibility* GetFocus(BrowserAccessibility* root);

 protected:
  BrowserAccessibilityManager(
      gfx::NativeView parent_view,
      const WebAccessibility& src,
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory);

 private:
  void OnAccessibilityObjectStateChange(
      const WebAccessibility& acc_obj);
  void OnAccessibilityObjectChildrenChange(
      const WebAccessibility& acc_obj);
  void OnAccessibilityObjectFocusChange(
      const WebAccessibility& acc_obj);
  void OnAccessibilityObjectLoadComplete(
      const WebAccessibility& acc_obj);
  void OnAccessibilityObjectValueChange(
      const WebAccessibility& acc_obj);
  void OnAccessibilityObjectTextChange(
      const WebAccessibility& acc_obj);

  
  
  
  
  
  BrowserAccessibility* UpdateNode(
      const WebAccessibility& src,
      bool include_children);

  
  
  BrowserAccessibility* CreateAccessibilityTree(
      BrowserAccessibility* parent,
      const WebAccessibility& src,
      int index_in_parent);

 protected:
  
  static int32 next_child_id_;

  
  gfx::NativeView parent_view_;

  
  BrowserAccessibilityDelegate* delegate_;

  
  scoped_ptr<BrowserAccessibilityFactory> factory_;

  
  
  BrowserAccessibility* root_;
  BrowserAccessibility* focus_;

  
  
  base::hash_map<int32, int32> renderer_id_to_child_id_map_;

  
  base::hash_map<int32, BrowserAccessibility*> child_id_map_;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityManager);
};

#endif  
