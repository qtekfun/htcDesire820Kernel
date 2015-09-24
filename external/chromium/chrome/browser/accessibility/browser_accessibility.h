// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_H_
#pragma once

#include <map>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "build/build_config.h"
#include "webkit/glue/webaccessibility.h"

class BrowserAccessibilityManager;
#if defined(OS_MACOSX) && __OBJC__
@class BrowserAccessibilityCocoa;
#elif defined(OS_WIN)
class BrowserAccessibilityWin;
#endif

using webkit_glue::WebAccessibility;

class BrowserAccessibility {
 public:
  
  
  static BrowserAccessibility* Create();

  virtual ~BrowserAccessibility();

  
  
  
  virtual void Initialize();

  
  virtual void ReplaceChild(
      BrowserAccessibility* old_acc,
      BrowserAccessibility* new_acc);

  
  
  void Initialize(BrowserAccessibilityManager* manager,
                  BrowserAccessibility* parent,
                  int32 child_id,
                  int32 index_in_parent,
                  const WebAccessibility& src);

  
  void AddChild(BrowserAccessibility* child);

  
  
  void DetachTree(std::vector<BrowserAccessibility*>* nodes);

  
  void UpdateParent(BrowserAccessibility* parent, int index_in_parent);

  
  bool IsDescendantOf(BrowserAccessibility* ancestor);

  
  BrowserAccessibility* parent() { return parent_; }

  
  uint32 child_count() const { return children_.size(); }

  
  BrowserAccessibility* GetChild(uint32 child_index);

  
  
  BrowserAccessibility* GetPreviousSibling();

  
  
  BrowserAccessibility* GetNextSibling();

  
  gfx::Rect GetBoundsRect();

  
  BrowserAccessibility* BrowserAccessibilityForPoint(const gfx::Point& point);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  virtual void InternalAddReference();

  
  
  virtual void InternalReleaseReference(bool recursive);

  
  virtual void NativeAddReference() { }

  
  virtual void NativeReleaseReference();

  
  
  

  const std::map<int32, string16>& attributes() const { return attributes_; }
  int32 child_id() const { return child_id_; }
  const std::vector<BrowserAccessibility*>& children() const {
    return children_;
  }
  const std::vector<std::pair<string16, string16> >& html_attributes() const {
    return html_attributes_;
  }
  int32 index_in_parent() const { return index_in_parent_; }
  gfx::Rect location() const { return location_; }
  BrowserAccessibilityManager* manager() const { return manager_; }
  const string16& name() const { return name_; }
  int32 renderer_id() const { return renderer_id_; }
  int32 role() const { return role_; }
  const string16& role_name() const { return role_name_; }
  int32 state() const { return state_; }
  const string16& value() const { return value_; }
  bool instance_active() const { return instance_active_; }
  int32 ref_count() const { return ref_count_; }

#if defined(OS_MACOSX) && __OBJC__
  BrowserAccessibilityCocoa* toBrowserAccessibilityCocoa();
#elif defined(OS_WIN)
  BrowserAccessibilityWin* toBrowserAccessibilityWin();
#endif

  
  
  bool HasAttribute(WebAccessibility::Attribute attribute);

  
  
  bool GetAttribute(WebAccessibility::Attribute attribute, string16* value);

  
  
  
  
  bool GetAttributeAsInt(
      WebAccessibility::Attribute attribute, int* value_int);

 protected:
  BrowserAccessibility();

  
  
  BrowserAccessibilityManager* manager_;

  
  BrowserAccessibility* parent_;

  
  int32 child_id_;

  
  int32 index_in_parent_;

  
  int32 renderer_id_;

  
  std::vector<BrowserAccessibility*> children_;

  
  int32 ref_count_;

  
  string16 name_;
  string16 value_;
  std::map<int32, string16> attributes_;
  std::vector<std::pair<string16, string16> > html_attributes_;
  int32 role_;
  int32 state_;
  string16 role_name_;
  gfx::Rect location_;
  std::vector<int32> indirect_child_ids_;

  
  
  
  
  
  bool instance_active_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibility);
};

#endif  
