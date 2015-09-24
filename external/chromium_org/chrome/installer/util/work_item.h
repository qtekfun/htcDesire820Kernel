// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_WORK_ITEM_H_

#include <windows.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"

class CallbackWorkItem;
class CopyRegKeyWorkItem;
class CopyTreeWorkItem;
class CreateDirWorkItem;
class CreateRegKeyWorkItem;
class DeleteTreeWorkItem;
class DeleteRegKeyWorkItem;
class DeleteRegValueWorkItem;
class MoveTreeWorkItem;
class SelfRegWorkItem;
class SetRegValueWorkItem;
class WorkItemList;

namespace base {
class FilePath;
}

class WorkItem {
 public:
  
  enum CopyOverWriteOption {
    ALWAYS,  
    NEVER,  
    IF_DIFFERENT,  
    IF_NOT_PRESENT,  
    NEW_NAME_IF_IN_USE  
  };

  
  enum MoveTreeOption {
    ALWAYS_MOVE,  
    CHECK_DUPLICATES  
  };

  
  
  class Condition {
   public:
    virtual ~Condition() {}
    virtual bool ShouldRun() const = 0;
  };

  virtual ~WorkItem();

  
  static CallbackWorkItem* CreateCallbackWorkItem(
      base::Callback<bool(const CallbackWorkItem&)> callback);

  
  static CopyRegKeyWorkItem* CreateCopyRegKeyWorkItem(
      HKEY predefined_root,
      const std::wstring& source_key_path,
      const std::wstring& dest_key_path,
      CopyOverWriteOption overwrite_option);

  
  
  
  
  
  
  static CopyTreeWorkItem* CreateCopyTreeWorkItem(
      const base::FilePath& source_path,
      const base::FilePath& dest_path,
      const base::FilePath& temp_dir,
      CopyOverWriteOption overwrite_option,
      const base::FilePath& alternative_path);

  
  static CreateDirWorkItem* CreateCreateDirWorkItem(const base::FilePath& path);

  
  
  static CreateRegKeyWorkItem* CreateCreateRegKeyWorkItem(
      HKEY predefined_root, const std::wstring& path);

  
  
  static DeleteRegKeyWorkItem* CreateDeleteRegKeyWorkItem(
      HKEY predefined_root, const std::wstring& path);

  
  static DeleteRegValueWorkItem* CreateDeleteRegValueWorkItem(
      HKEY predefined_root,
      const std::wstring& key_path,
      const std::wstring& value_name);

  
  
  
  static DeleteTreeWorkItem* CreateDeleteTreeWorkItem(
      const base::FilePath& root_path,
      const base::FilePath& temp_path,
      const std::vector<base::FilePath>& key_paths);

  
  
  static MoveTreeWorkItem* CreateMoveTreeWorkItem(
      const base::FilePath& source_path,
      const base::FilePath& dest_path,
      const base::FilePath& temp_dir,
      MoveTreeOption duplicate_option);

  
  
  static SetRegValueWorkItem* CreateSetRegValueWorkItem(
      HKEY predefined_root,
      const std::wstring& key_path,
      const std::wstring& value_name,
      const std::wstring& value_data,
      bool overwrite);

  
  
  static SetRegValueWorkItem* CreateSetRegValueWorkItem(
      HKEY predefined_root,
      const std::wstring& key_path,
      const std::wstring& value_name,
      DWORD value_data, bool overwrite);

  
  
  static SetRegValueWorkItem* CreateSetRegValueWorkItem(
      HKEY predefined_root,
      const std::wstring& key_path,
      const std::wstring& value_name,
      int64 value_data, bool overwrite);

  
  
  static SelfRegWorkItem* CreateSelfRegWorkItem(const std::wstring& dll_path,
                                                bool do_register,
                                                bool user_level_registration);

  
  
  static WorkItemList* CreateWorkItemList();

  
  
  
  static WorkItemList* CreateNoRollbackWorkItemList();

  
  
  
  static WorkItemList* CreateConditionalWorkItemList(Condition* condition);

  
  
  
  
  virtual bool Do() = 0;

  
  
  
  
  virtual void Rollback() = 0;

  
  
  void set_ignore_failure(bool ignore_failure) {
    ignore_failure_ = ignore_failure;
  }

  
  bool ignore_failure() const {
    return ignore_failure_;
  }

  
  
  void set_log_message(const std::string& log_message) {
    log_message_ = log_message;
  }

  
  const std::string& log_message() const { return log_message_; }

 protected:
  WorkItem();

  
  
  bool ignore_failure_;

  std::string log_message_;
};

#endif  
