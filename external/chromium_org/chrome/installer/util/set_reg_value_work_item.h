// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_SET_REG_VALUE_WORK_ITEM_H__
#define CHROME_INSTALLER_UTIL_SET_REG_VALUE_WORK_ITEM_H__

#include <windows.h>

#include <string>
#include <vector>

#include "chrome/installer/util/work_item.h"

class SetRegValueWorkItem : public WorkItem {
 public:
  virtual ~SetRegValueWorkItem();

  virtual bool Do();

  virtual void Rollback();

 private:
  friend class WorkItem;

  enum SettingStatus {
    
    SET_VALUE,
    
    NEW_VALUE_CREATED,
    
    // been overwritten.
    VALUE_OVERWRITTEN,
    
    
    
    VALUE_UNCHANGED,
    
    VALUE_ROLL_BACK
  };

  SetRegValueWorkItem(HKEY predefined_root,
                      const std::wstring& key_path,
                      const std::wstring& value_name,
                      const std::wstring& value_data,
                      bool overwrite);

  SetRegValueWorkItem(HKEY predefined_root, const std::wstring& key_path,
                      const std::wstring& value_name, DWORD value_data,
                      bool overwrite);

  SetRegValueWorkItem(HKEY predefined_root, const std::wstring& key_path,
                      const std::wstring& value_name, int64 value_data,
                      bool overwrite);

  
  
  HKEY predefined_root_;

  
  std::wstring key_path_;

  
  std::wstring value_name_;

  
  bool overwrite_;

  
  DWORD type_;
  std::vector<uint8> value_;
  DWORD previous_type_;
  std::vector<uint8> previous_value_;

  SettingStatus status_;
};

#endif  
