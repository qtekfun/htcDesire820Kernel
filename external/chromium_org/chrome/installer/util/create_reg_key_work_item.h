// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_CREATE_REG_KEY_WORK_ITEM_H__
#define CHROME_INSTALLER_UTIL_CREATE_REG_KEY_WORK_ITEM_H__

#include <windows.h>

#include <string>
#include <vector>

#include "chrome/installer/util/work_item.h"

class CreateRegKeyWorkItem : public WorkItem {
 public:
  virtual ~CreateRegKeyWorkItem();

  virtual bool Do();

  virtual void Rollback();

 private:
  friend class WorkItem;

  CreateRegKeyWorkItem(HKEY predefined_root, const std::wstring& path);

  
  
  bool InitKeyList();

  
  
  HKEY predefined_root_;

  
  std::wstring path_;

  
  
  std::vector<std::wstring> key_list_;

  
  bool key_created_;
};

#endif  
