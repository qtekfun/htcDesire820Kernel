// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_COPY_TREE_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_COPY_TREE_WORK_ITEM_H_

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/gtest_prod_util.h"
#include "chrome/installer/util/work_item.h"

class CopyTreeWorkItem : public WorkItem {
 public:
  virtual ~CopyTreeWorkItem();

  virtual bool Do();

  virtual void Rollback();

 private:
  friend class WorkItem;

  
  
  
  
  
  CopyTreeWorkItem(const base::FilePath& source_path,
                   const base::FilePath& dest_path,
                   const base::FilePath& temp_dir,
                   CopyOverWriteOption overwrite_option,
                   const base::FilePath& alternative_path);

  
  bool IsFileInUse(const base::FilePath& path);

  
  base::FilePath source_path_;

  
  base::FilePath dest_path_;

  
  base::FilePath temp_dir_;

  
  CopyOverWriteOption overwrite_option_;

  
  
  
  base::FilePath alternative_path_;

  
  bool copied_to_dest_path_;

  
  
  bool moved_to_backup_;

  
  
  bool copied_to_alternate_path_;

  
  base::ScopedTempDir backup_path_;

  FRIEND_TEST_ALL_PREFIXES(CopyTreeWorkItemTest, CopyFileSameContent);
  FRIEND_TEST_ALL_PREFIXES(CopyTreeWorkItemTest, CopyFileInUse);
  FRIEND_TEST_ALL_PREFIXES(CopyTreeWorkItemTest, CopyFileAndCleanup);
  FRIEND_TEST_ALL_PREFIXES(CopyTreeWorkItemTest, NewNameAndCopyTest);
  FRIEND_TEST_ALL_PREFIXES(CopyTreeWorkItemTest, CopyFileInUseAndCleanup);
};

#endif  
