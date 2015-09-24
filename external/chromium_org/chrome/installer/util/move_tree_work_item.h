// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_MOVE_TREE_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_MOVE_TREE_WORK_ITEM_H_

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/gtest_prod_util.h"
#include "chrome/installer/util/work_item.h"

class MoveTreeWorkItem : public WorkItem {
 public:
  virtual ~MoveTreeWorkItem();

  virtual bool Do();

  virtual void Rollback();

 private:
  friend class WorkItem;
  FRIEND_TEST_ALL_PREFIXES(MoveTreeWorkItemTest,
                           MoveDirectoryDestExistsCheckForDuplicatesFull);
  FRIEND_TEST_ALL_PREFIXES(MoveTreeWorkItemTest,
                           MoveDirectoryDestExistsCheckForDuplicatesPartial);

  
  
  
  
  
  
  
  
  
  MoveTreeWorkItem(const base::FilePath& source_path,
                   const base::FilePath& dest_path,
                   const base::FilePath& temp_dir,
                   MoveTreeOption duplicate_option);

  
  base::FilePath source_path_;

  
  base::FilePath dest_path_;

  
  base::FilePath temp_dir_;

  
  base::ScopedTempDir backup_path_;

  
  bool moved_to_dest_path_;

  
  
  bool moved_to_backup_;

  
  
  
  bool source_moved_to_backup_;

  
  MoveTreeOption duplicate_option_;
};

#endif  
