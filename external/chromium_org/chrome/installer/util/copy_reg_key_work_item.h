// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_COPY_REG_KEY_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_COPY_REG_KEY_WORK_ITEM_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/installer/util/registry_key_backup.h"
#include "chrome/installer/util/work_item.h"

class CopyRegKeyWorkItem : public WorkItem {
 public:
  virtual ~CopyRegKeyWorkItem();
  virtual bool Do() OVERRIDE;
  virtual void Rollback() OVERRIDE;

 private:
  
  
  friend class WorkItem;

  
  
  CopyRegKeyWorkItem(HKEY predefined_key,
                     const std::wstring& source_key_path,
                     const std::wstring& dest_key_path,
                     CopyOverWriteOption overwrite_option);

  
  
  HKEY predefined_root_;

  
  std::wstring source_key_path_;

  
  std::wstring dest_key_path_;

  
  CopyOverWriteOption overwrite_option_;

  
  RegistryKeyBackup backup_;

  
  bool cleared_destination_;

  DISALLOW_COPY_AND_ASSIGN(CopyRegKeyWorkItem);
};

#endif  
