// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_H_

#include <map>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "content/common/accessibility_node_data.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebAXEnums.h"

#if defined(OS_MACOSX) && __OBJC__
@class BrowserAccessibilityCocoa;
#endif

namespace content {
class BrowserAccessibilityManager;
#if defined(OS_WIN)
class BrowserAccessibilityWin;
#elif defined(TOOLKIT_GTK)
class BrowserAccessibilityGtk;
#endif

class CONTENT_EXPORT BrowserAccessibility {
 public:
  
  
  static BrowserAccessibility* Create();

  virtual ~BrowserAccessibility();

  
  
  virtual void DetachTree(std::vector<BrowserAccessibility*>* nodes);

  
  
  
  
  virtual void PostInitialize() {}

  
  
  virtual bool IsNative() const;

  
  void InitializeTreeStructure(
      BrowserAccessibilityManager* manager,
      BrowserAccessibility* parent,
      int32 renderer_id,
      int32 index_in_parent);

  
  void InitializeData(const AccessibilityNodeData& src);

  virtual void SwapChildren(std::vector<BrowserAccessibility*>& children);

  
  void UpdateParent(BrowserAccessibility* parent, int index_in_parent);

  
  virtual void SetLocation(const gfx::Rect& new_location);

  
  bool IsDescendantOf(BrowserAccessibility* ancestor);

  
  BrowserAccessibility* parent() const { return parent_; }

  
  uint32 child_count() const { return children_.size(); }

  
  
  
  
  
  
  virtual bool PlatformIsLeaf() const;

  
  
  uint32 PlatformChildCount() const;

  
  
  BrowserAccessibility* PlatformGetChild(uint32 child_index) const;

  
  
  BrowserAccessibility* GetPreviousSibling();

  
  
  BrowserAccessibility* GetNextSibling();

  
  
  gfx::Rect GetLocalBoundsRect() const;

  
  gfx::Rect GetGlobalBoundsRect() const;

  
  
  
  gfx::Rect GetLocalBoundsForRange(int start, int len) const;

  
  
  gfx::Rect GetGlobalBoundsForRange(int start, int len) const;

  
  
  BrowserAccessibility* BrowserAccessibilityForPoint(const gfx::Point& point);

  
  
  
  
  
  
  
  
  
  virtual void Destroy();

  
  virtual void NativeAddReference() { }

  
  virtual void NativeReleaseReference();

  
  
  

  const std::vector<BrowserAccessibility*>& children() const {
    return children_;
  }
  const std::vector<std::pair<std::string, std::string> >&
  html_attributes() const {
    return html_attributes_;
  }
  int32 index_in_parent() const { return index_in_parent_; }
  gfx::Rect location() const { return location_; }
  BrowserAccessibilityManager* manager() const { return manager_; }
  const std::string& name() const { return name_; }
  const std::string& value() const { return value_; }
  int32 renderer_id() const { return renderer_id_; }
  int32 role() const { return role_; }
  int32 state() const { return state_; }
  bool instance_active() const { return instance_active_; }

  void set_name(const std::string& name) { name_ = name; }
  void set_value(const std::string& value) { value_ = value; }

#if defined(OS_MACOSX) && __OBJC__
  BrowserAccessibilityCocoa* ToBrowserAccessibilityCocoa();
#elif defined(OS_WIN)
  BrowserAccessibilityWin* ToBrowserAccessibilityWin();
#elif defined(TOOLKIT_GTK)
  BrowserAccessibilityGtk* ToBrowserAccessibilityGtk();
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  
  

  bool HasBoolAttribute(AccessibilityNodeData::BoolAttribute attr) const;
  bool GetBoolAttribute(AccessibilityNodeData::BoolAttribute attr) const;
  bool GetBoolAttribute(AccessibilityNodeData::BoolAttribute attr,
                        bool* value) const;

  bool HasFloatAttribute(AccessibilityNodeData::FloatAttribute attr) const;
  float GetFloatAttribute(AccessibilityNodeData::FloatAttribute attr) const;
  bool GetFloatAttribute(AccessibilityNodeData::FloatAttribute attr,
                         float* value) const;

  bool HasIntAttribute(AccessibilityNodeData::IntAttribute attribute) const;
  int GetIntAttribute(AccessibilityNodeData::IntAttribute attribute) const;
  bool GetIntAttribute(AccessibilityNodeData::IntAttribute attribute,
                       int* value) const;

  bool HasStringAttribute(
      AccessibilityNodeData::StringAttribute attribute) const;
  const std::string& GetStringAttribute(
      AccessibilityNodeData::StringAttribute attribute) const;
  bool GetStringAttribute(AccessibilityNodeData::StringAttribute attribute,
                          std::string* value) const;

  bool GetString16Attribute(AccessibilityNodeData::StringAttribute attribute,
                            base::string16* value) const;
  base::string16 GetString16Attribute(
      AccessibilityNodeData::StringAttribute attribute) const;

  bool HasIntListAttribute(
      AccessibilityNodeData::IntListAttribute attribute) const;
  const std::vector<int32>& GetIntListAttribute(
      AccessibilityNodeData::IntListAttribute attribute) const;
  bool GetIntListAttribute(AccessibilityNodeData::IntListAttribute attribute,
                           std::vector<int32>* value) const;

  void SetStringAttribute(
      AccessibilityNodeData::StringAttribute attribute,
      const std::string& value);

  
  
  bool GetHtmlAttribute(const char* attr, base::string16* value) const;
  bool GetHtmlAttribute(const char* attr, std::string* value) const;

  
  
  
  
  
  
  
  
  
  
  
  
  bool GetAriaTristate(const char* attr_name,
                       bool* is_defined,
                       bool* is_mixed) const;

  
  bool HasState(blink::WebAXState state_enum) const;

  
  bool IsEditableText() const;

  
  std::string GetTextRecursive() const;

 protected:
  
  
  
  
  virtual void PreInitialize() {}

  BrowserAccessibility();

  
  
  BrowserAccessibilityManager* manager_;

  
  BrowserAccessibility* parent_;

 private:
  
  int32 index_in_parent_;

  
  int32 renderer_id_;

  
  std::vector<BrowserAccessibility*> children_;

  
  std::string name_;
  std::string value_;
  std::vector<std::pair<
      AccessibilityNodeData::BoolAttribute, bool> > bool_attributes_;
  std::vector<std::pair<
      AccessibilityNodeData::FloatAttribute, float> > float_attributes_;
  std::vector<std::pair<
      AccessibilityNodeData::IntAttribute, int> > int_attributes_;
  std::vector<std::pair<
      AccessibilityNodeData::StringAttribute, std::string> > string_attributes_;
  std::vector<std::pair<
      AccessibilityNodeData::IntListAttribute, std::vector<int32> > >
          intlist_attributes_;
  std::vector<std::pair<std::string, std::string> > html_attributes_;
  int32 role_;
  int32 state_;
  gfx::Rect location_;

  
  
  
  
  
  bool instance_active_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibility);
};

}  

#endif  
