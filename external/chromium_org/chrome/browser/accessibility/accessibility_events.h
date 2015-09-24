// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_ACCESSIBILITY_EVENTS_H_
#define CHROME_BROWSER_ACCESSIBILITY_ACCESSIBILITY_EVENTS_H_

#include <string>
#include "base/compiler_specific.h"
#include "ui/base/accessibility/accessibility_types.h"

class AccessibilityControlInfo;
class AccessibilityMenuInfo;
class AccessibilityWindowInfo;
class Profile;

namespace base {
class DictionaryValue;
}

void SendControlAccessibilityNotification(
    ui::AccessibilityTypes::Event event,
    AccessibilityControlInfo* info);

void SendMenuAccessibilityNotification(
    ui::AccessibilityTypes::Event event,
    AccessibilityMenuInfo* info);

void SendWindowAccessibilityNotification(
    ui::AccessibilityTypes::Event event,
    AccessibilityWindowInfo* info);

class AccessibilityEventInfo {
 public:
  virtual ~AccessibilityEventInfo() {}

  
  
  virtual void SerializeToDict(base::DictionaryValue* dict) const = 0;

  Profile* profile() const { return profile_; }

 protected:
  explicit AccessibilityEventInfo(Profile* profile) : profile_(profile) {}

  
  Profile* profile_;
};

class AccessibilityControlInfo : public AccessibilityEventInfo {
 public:
  virtual ~AccessibilityControlInfo();

  
  
  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  
  
  virtual const char* type() const = 0;

  const std::string& name() const { return name_; }

  const std::string& context() const { return context_; }

 protected:
  AccessibilityControlInfo(Profile* profile,
                           const std::string& name);

  void set_context(const std::string& context) { context_ = context; }

  
  std::string name_;

  
  
  std::string context_;
};

class AccessibilityWindowInfo : public AccessibilityControlInfo {
 public:
  AccessibilityWindowInfo(Profile* profile, const std::string& window_name);

  virtual const char* type() const OVERRIDE;
};

class AccessibilityButtonInfo : public AccessibilityControlInfo {
 public:
  AccessibilityButtonInfo(Profile* profile,
                          const std::string& button_name,
                          const std::string& context);

  virtual const char* type() const OVERRIDE;
};

class AccessibilityLinkInfo : public AccessibilityControlInfo {
 public:
  AccessibilityLinkInfo(Profile* profile,
                        const std::string& link_name,
                        const std::string& context);

  virtual const char* type() const OVERRIDE;
};

class AccessibilityRadioButtonInfo : public AccessibilityControlInfo {
 public:
  AccessibilityRadioButtonInfo(Profile* profile,
                               const std::string& name,
                               const std::string& context,
                               bool checked,
                               int item_index,
                               int item_count);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  void SetChecked(bool checked) { checked_ = checked; }

  int item_index() const { return item_index_; }
  int item_count() const { return item_count_; }
  bool checked() const { return checked_; }

 private:
  bool checked_;
  
  int item_index_;
  int item_count_;
};

class AccessibilityCheckboxInfo : public AccessibilityControlInfo {
 public:
  AccessibilityCheckboxInfo(Profile* profile,
                            const std::string& name,
                            const std::string& context,
                            bool checked);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  void SetChecked(bool checked) { checked_ = checked; }

  bool checked() const { return checked_; }

 private:
  bool checked_;
};

class AccessibilityTabInfo : public AccessibilityControlInfo {
 public:
  AccessibilityTabInfo(Profile* profile,
                       const std::string& tab_name,
                       const std::string& context,
                       int tab_index,
                       int tab_count);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  void SetTab(int tab_index, std::string tab_name) {
    tab_index_ = tab_index;
    name_ = tab_name;
  }

  int tab_index() const { return tab_index_; }
  int tab_count() const { return tab_count_; }

 private:
  
  int tab_index_;
  int tab_count_;
};

class AccessibilityComboBoxInfo : public AccessibilityControlInfo {
 public:
  AccessibilityComboBoxInfo(Profile* profile,
                            const std::string& name,
                            const std::string& context,
                            const std::string& value,
                            int item_index,
                            int item_count);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  void SetValue(int item_index, const std::string& value) {
    item_index_ = item_index;
    value_ = value;
  }

  int item_index() const { return item_index_; }
  int item_count() const { return item_count_; }
  const std::string& value() const { return value_; }

 private:
  std::string value_;
  
  
  
  int item_index_;
  int item_count_;
};


class AccessibilityTextBoxInfo : public AccessibilityControlInfo {
 public:
  AccessibilityTextBoxInfo(Profile* profile,
                           const std::string& name,
                           const std::string& context,
                           bool password);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  void SetValue(
      const std::string& value, int selection_start, int selection_end) {
    value_ = value;
    selection_start_ = selection_start;
    selection_end_ = selection_end;
  }

  const std::string& value() const { return value_; }
  bool password() const { return password_; }
  int selection_start() const { return selection_start_; }
  int selection_end() const { return selection_end_; }

 private:
  std::string value_;
  bool password_;
  int selection_start_;
  int selection_end_;
};

class AccessibilityListBoxInfo : public AccessibilityControlInfo {
 public:
  AccessibilityListBoxInfo(Profile* profile,
                           const std::string& name,
                           const std::string& context,
                           const std::string& value,
                           int item_index,
                           int item_count);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  void SetValue(int item_index, std::string value) {
    item_index_ = item_index;
    value_ = value;
  }

  int item_index() const { return item_index_; }
  int item_count() const { return item_count_; }
  const std::string& value() const { return value_; }

 private:
  std::string value_;
  
  
  
  int item_index_;
  int item_count_;
};

class AccessibilityMenuInfo : public AccessibilityControlInfo {
 public:
  AccessibilityMenuInfo(Profile* profile, const std::string& menu_name);

  virtual const char* type() const OVERRIDE;
};

class AccessibilityMenuItemInfo : public AccessibilityControlInfo {
 public:
  AccessibilityMenuItemInfo(Profile* profile,
                            const std::string& name,
                            const std::string& context,
                            bool has_submenu,
                            int item_index,
                            int item_count);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  int item_index() const { return item_index_; }
  int item_count() const { return item_count_; }
  bool has_submenu() const { return has_submenu_; }

 private:
  bool has_submenu_;
  
  int item_index_;
  int item_count_;
};

class AccessibilityTreeInfo : public AccessibilityControlInfo {
 public:
  AccessibilityTreeInfo(Profile* profile, const std::string& menu_name);

  virtual const char* type() const OVERRIDE;
};

class AccessibilityTreeItemInfo : public AccessibilityControlInfo {
 public:
  AccessibilityTreeItemInfo(Profile* profile,
                            const std::string& name,
                            const std::string& context,
                            int item_depth,
                            int item_index,
                            int item_count,
                            int children_count,
                            bool is_expanded);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  int item_depth() const { return item_depth_; }
  int item_index() const { return item_index_; }
  int item_count() const { return item_count_; }
  int children_count() const { return children_count_; }
  bool is_expanded() const { return is_expanded_; }

 private:
  
  int item_depth_;
  
  
  int item_index_;
  
  int item_count_;
  
  int children_count_;
  
  bool is_expanded_;
};

class AccessibilitySliderInfo : public AccessibilityControlInfo {
 public:
  AccessibilitySliderInfo(Profile* profile,
                          const std::string& name,
                          const std::string& context,
                          const std::string& value);

  virtual const char* type() const OVERRIDE;

  virtual void SerializeToDict(base::DictionaryValue* dict) const OVERRIDE;

  const std::string& value() const { return value_; }

 private:
  std::string value_;
};

class AccessibilityAlertInfo : public AccessibilityControlInfo {
 public:
  AccessibilityAlertInfo(Profile* profile, const std::string& name);

  virtual const char* type() const OVERRIDE;
};

#endif  
