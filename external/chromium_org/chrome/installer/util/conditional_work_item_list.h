// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_CONDITIONAL_WORK_ITEM_LIST_H_
#define CHROME_INSTALLER_UTIL_CONDITIONAL_WORK_ITEM_LIST_H_

#include "chrome/installer/util/work_item_list.h"

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"

class ConditionalWorkItemList : public WorkItemList {
 public:
  explicit ConditionalWorkItemList(Condition* condition);
  virtual ~ConditionalWorkItemList();

  
  
  
  virtual bool Do();

  
  virtual void Rollback();

 protected:
  
  
  scoped_ptr<Condition> condition_;
};


class ConditionRunIfFileExists : public WorkItem::Condition {
 public:
  explicit ConditionRunIfFileExists(const base::FilePath& key_path)
      : key_path_(key_path) {}
  bool ShouldRun() const;

 private:
  base::FilePath key_path_;
};

class Not : public WorkItem::Condition {
 public:
   explicit Not(WorkItem::Condition* original_condition)
     : original_condition_(original_condition) {}
  bool ShouldRun() const;

 private:
  scoped_ptr<WorkItem::Condition> original_condition_;
};


#endif  
