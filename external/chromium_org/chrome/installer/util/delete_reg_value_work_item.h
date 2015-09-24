// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_DELETE_REG_VALUE_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_DELETE_REG_VALUE_WORK_ITEM_H_

#include <windows.h>

#include <string>

#include "chrome/installer/util/work_item.h"

class DeleteRegValueWorkItem : public WorkItem {
 public:
  virtual ~DeleteRegValueWorkItem();

  virtual bool Do();

  virtual void Rollback();

 private:
  friend class WorkItem;

  enum DeletionStatus {
    
    DELETE_VALUE,
    
    VALUE_DELETED,
    
    VALUE_NOT_FOUND,
    
    VALUE_ROLLED_BACK,
    
    VALUE_UNCHANGED
  };

  DeleteRegValueWorkItem(HKEY predefined_root, const std::wstring& key_path,
                         const std::wstring& value_name);

  
  
  HKEY predefined_root_;

  
  std::wstring key_path_;

  
  std::wstring value_name_;

  DeletionStatus status_;

  
  DWORD previous_type_;
  std::string previous_value_;
};

#endif  
