// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_TASK_MANAGER_TASK_MANAGER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_TASK_MANAGER_TASK_MANAGER_HANDLER_H_

#include <set>
#include <string>
#include <vector>

#include "content/public/browser/web_ui_message_handler.h"
#include "chrome/browser/task_manager/task_manager.h"

namespace base {
class ListValue;
class Value;
}

class TaskManagerHandler : public content::WebUIMessageHandler,
                           public TaskManagerModelObserver {
 public:
  explicit TaskManagerHandler(TaskManager* tm);
  virtual ~TaskManagerHandler();

  
  
  virtual void OnModelChanged() OVERRIDE;
  
  virtual void OnItemsChanged(int start, int length) OVERRIDE;
  
  virtual void OnItemsAdded(int start, int length) OVERRIDE;
  
  virtual void OnItemsRemoved(int start, int length) OVERRIDE;

  
  
  virtual void OnReadyPeriodicalUpdate() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleKillProcesses(const base::ListValue* indexes);

  
  void HandleActivatePage(const base::ListValue* resource_index);

  
  void HandleInspect(const base::ListValue* resource_index);

  void EnableTaskManager(const base::ListValue* indexes);
  void DisableTaskManager(const base::ListValue* indexes);
  void OpenAboutMemory(const base::ListValue* indexes);

  
  void HandleSetUpdateColumn(const base::ListValue* args);

 private:
  bool is_alive();

  
  TaskManager* task_manager_;
  TaskManagerModel* model_;

  bool is_enabled_;

  
  std::set<std::string> enabled_columns_;

  
  
  void OnGroupAdded(int start, int length);
  void OnGroupChanged(int start, int length);
  void OnGroupRemoved(int start, int length);

  
  
  
  base::DictionaryValue* CreateTaskGroupValue(int group_index);

  
  
  
  
  
  
  void CreateGroupColumnList(const std::string& column_name,
                             const int index,
                             const int length,
                             base::DictionaryValue* val);

  
  
  
  
  
  base::Value* CreateColumnValue(const std::string& column_name,
                                 const int i);

  DISALLOW_COPY_AND_ASSIGN(TaskManagerHandler);
};

#endif  
