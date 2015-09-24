// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_DELETE_TREE_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_DELETE_TREE_WORK_ITEM_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/installer/util/work_item.h"

class DeleteTreeWorkItem : public WorkItem {
 public:
  virtual ~DeleteTreeWorkItem();

  virtual bool Do();

  virtual void Rollback();

 private:
  friend class WorkItem;

  DeleteTreeWorkItem(const base::FilePath& root_path,
                     const base::FilePath& temp_path,
                     const std::vector<base::FilePath>& key_paths);

  
  base::FilePath root_path_;

  
  base::FilePath temp_path_;

  
  ptrdiff_t num_key_files_;

  
  
  scoped_ptr<base::FilePath[]> key_paths_;

  
  
  
  scoped_ptr<base::ScopedTempDir[]> key_backup_paths_;

  
  base::ScopedTempDir backup_path_;

  
  bool copied_to_backup_;
};

#endif  
