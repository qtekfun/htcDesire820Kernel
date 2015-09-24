// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_WORK_ITEM_LIST_H_
#define CHROME_INSTALLER_UTIL_WORK_ITEM_LIST_H_

#include <windows.h>

#include <list>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/installer/util/work_item.h"

namespace base {
class FilePath;
}

class WorkItemList : public WorkItem {
 public:
  virtual ~WorkItemList();

  
  
  virtual bool Do();

  
  virtual void Rollback();

  
  
  
  virtual void AddWorkItem(WorkItem* work_item);

  
  virtual WorkItem* AddCallbackWorkItem(
      base::Callback<bool(const CallbackWorkItem&)> callback);

  
  virtual WorkItem* AddCopyRegKeyWorkItem(HKEY predefined_root,
                                          const std::wstring& source_key_path,
                                          const std::wstring& dest_key_path,
                                          CopyOverWriteOption overwrite_option);

  
  
  
  virtual WorkItem* AddCopyTreeWorkItem(
      const std::wstring& source_path,
      const std::wstring& dest_path,
      const std::wstring& temp_dir,
      CopyOverWriteOption overwrite_option,
      const std::wstring& alternative_path = L"");

  
  virtual WorkItem* AddCreateDirWorkItem(const base::FilePath& path);

  
  
  virtual WorkItem* AddCreateRegKeyWorkItem(HKEY predefined_root,
                                            const std::wstring& path);

  
  
  virtual WorkItem* AddDeleteRegKeyWorkItem(HKEY predefined_root,
                                            const std::wstring& path);

  
  
  virtual WorkItem* AddDeleteRegValueWorkItem(HKEY predefined_root,
                                              const std::wstring& key_path,
                                              const std::wstring& value_name);

  
  
  
  virtual WorkItem* AddDeleteTreeWorkItem(
      const base::FilePath& root_path,
      const base::FilePath& temp_path,
      const std::vector<base::FilePath>& key_paths);

  
  virtual WorkItem* AddDeleteTreeWorkItem(const base::FilePath& root_path,
                                          const base::FilePath& temp_path);

  
  virtual WorkItem* AddMoveTreeWorkItem(const std::wstring& source_path,
                                        const std::wstring& dest_path,
                                        const std::wstring& temp_dir,
                                        MoveTreeOption duplicate_option);

  
  
  virtual WorkItem* AddSetRegValueWorkItem(HKEY predefined_root,
                                           const std::wstring& key_path,
                                           const std::wstring& value_name,
                                           const std::wstring& value_data,
                                           bool overwrite);

  
  
  virtual WorkItem* AddSetRegValueWorkItem(HKEY predefined_root,
                                           const std::wstring& key_path,
                                           const std::wstring& value_name,
                                           DWORD value_data,
                                           bool overwrite);

  
  
  virtual WorkItem* AddSetRegValueWorkItem(HKEY predefined_root,
                                           const std::wstring& key_path,
                                           const std::wstring& value_name,
                                           int64 value_data,
                                           bool overwrite);

  
  
  
  virtual WorkItem* AddSelfRegWorkItem(const std::wstring& dll_path,
                                       bool do_register,
                                       bool user_level_registration);

 protected:
  friend class WorkItem;

  typedef std::list<WorkItem*> WorkItems;
  typedef WorkItems::iterator WorkItemIterator;

  enum ListStatus {
    
    ADD_ITEM,
    
    LIST_EXECUTED,
    
    LIST_ROLLED_BACK
  };

  WorkItemList();

  ListStatus status_;

  
  WorkItems list_;

  
  
  WorkItems executed_list_;
};

class NoRollbackWorkItemList : public WorkItemList {
 public:
  virtual ~NoRollbackWorkItemList();

  
  
  
  virtual bool Do();

  
  virtual void Rollback();
};

#endif  
