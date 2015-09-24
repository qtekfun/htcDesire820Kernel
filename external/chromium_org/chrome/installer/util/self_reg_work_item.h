// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_SELF_REG_WORK_ITEM_H__
#define CHROME_INSTALLER_UTIL_SELF_REG_WORK_ITEM_H__

#include <windows.h>
#include <string>

#include "chrome/installer/util/work_item.h"

class SelfRegWorkItem : public WorkItem {
 public:
  virtual ~SelfRegWorkItem();

  virtual bool Do();
  virtual void Rollback();

 private:
  friend class WorkItem;

  
  
  
  
  
  
  
  
  SelfRegWorkItem(const std::wstring& dll_path, bool do_register,
                  bool user_level_registration);

  
  
  
  
  bool RegisterDll(bool do_register);

  
  std::wstring dll_path_;

  
  
  bool do_register_;

  
  
  bool user_level_registration_;
};

#endif  
