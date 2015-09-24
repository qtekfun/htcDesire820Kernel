// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_EVENTS_H_
#define CHROME_BROWSER_ACCESSIBILITY_EVENTS_H_
#pragma once

#include <string>

class AccessibilityControlInfo;
class DictionaryValue;
class NotificationType;
class Profile;

void SendAccessibilityNotification(
    NotificationType type, AccessibilityControlInfo* info);

class AccessibilityControlInfo {
 public:
  virtual ~AccessibilityControlInfo();

  
  
  virtual void SerializeToDict(DictionaryValue* dict) const;

  
  
  virtual const char* type() const = 0;

  Profile* profile() const { return profile_; }

  const std::string& name() const { return name_; }

 protected:
  AccessibilityControlInfo(Profile* profile, const std::string& control_name);

  
  Profile* profile_;

  
  std::string name_;
};

class AccessibilityWindowInfo : public AccessibilityControlInfo {
 public:
  AccessibilityWindowInfo(Profile* profile, const std::string& window_name);

  virtual const char* type() const;
};

class AccessibilityButtonInfo : public AccessibilityControlInfo {
 public:
  AccessibilityButtonInfo(Profile* profile, const std::string& button_name);

  virtual const char* type() const;
};

class AccessibilityLinkInfo : public AccessibilityControlInfo {
 public:
  AccessibilityLinkInfo(Profile* profile, const std::string& link_name);

  virtual const char* type() const;
};

class AccessibilityRadioButtonInfo : public AccessibilityControlInfo {
 public:
  AccessibilityRadioButtonInfo(Profile* profile,
                               const std::string& name,
                               bool checked,
                               int item_index,
                               int item_count);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

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
                            bool checked);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

  void SetChecked(bool checked) { checked_ = checked; }

  bool checked() const { return checked_; }

 private:
  bool checked_;
};

class AccessibilityTabInfo : public AccessibilityControlInfo {
 public:
  AccessibilityTabInfo(Profile* profile,
                       const std::string& tab_name,
                       int tab_index,
                       int tab_count);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

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
                            const std::string& value,
                            int item_index,
                            int item_count);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

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
                           bool password);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

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
                           const std::string& value,
                           int item_index,
                           int item_count);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

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

  virtual const char* type() const;
};

class AccessibilityMenuItemInfo : public AccessibilityControlInfo {
 public:
  AccessibilityMenuItemInfo(Profile* profile,
                            const std::string& name,
                            bool has_submenu,
                            int item_index,
                            int item_count);

  virtual const char* type() const;

  virtual void SerializeToDict(DictionaryValue* dict) const;

  int item_index() const { return item_index_; }
  int item_count() const { return item_count_; }
  bool has_submenu() const { return has_submenu_; }

 private:
  bool has_submenu_;
  
  int item_index_;
  int item_count_;
};

#endif  
