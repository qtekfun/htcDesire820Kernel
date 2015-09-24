// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_CREATE_DIR_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_CREATE_DIR_WORK_ITEM_H_

#include <windows.h>

#include "base/files/file_path.h"
#include "chrome/installer/util/work_item.h"

class CreateDirWorkItem : public WorkItem {
 public:
  virtual ~CreateDirWorkItem();

  virtual bool Do();

  
  
  
  
  virtual void Rollback();

 private:
  friend class WorkItem;

  explicit CreateDirWorkItem(const base::FilePath& path);

  
  
  
  void GetTopDirToCreate();

  
  base::FilePath path_;

  
  base::FilePath top_path_;

  bool rollback_needed_;
};

#endif  
