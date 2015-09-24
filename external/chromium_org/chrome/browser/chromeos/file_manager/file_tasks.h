// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_FILE_TASKS_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_FILE_TASKS_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "chrome/browser/extensions/api/file_handlers/app_file_handler_util.h"
#include "url/gurl.h"

class PrefService;
class Profile;

namespace drive {
class DriveAppRegistry;
}

namespace fileapi {
class FileSystemURL;
}

namespace file_manager {
namespace file_tasks {

enum TaskType {
  TASK_TYPE_FILE_BROWSER_HANDLER,
  TASK_TYPE_FILE_HANDLER,
  TASK_TYPE_DRIVE_APP,
  TASK_TYPE_UNKNOWN,  
};

struct TaskDescriptor {
  TaskDescriptor(const std::string& in_app_id,
                 TaskType in_task_type,
                 const std::string& in_action_id)
      : app_id(in_app_id),
        task_type(in_task_type),
        action_id(in_action_id) {
  }
  TaskDescriptor() {
  }

  std::string app_id;
  TaskType task_type;
  std::string action_id;
};

class FullTaskDescriptor {
 public:
  FullTaskDescriptor(const TaskDescriptor& task_descriptor,
                     const std::string& task_title,
                     const GURL& icon_url,
                     bool is_default);
  const TaskDescriptor& task_descriptor() const { return task_descriptor_; }

  
  const std::string& task_title() const { return task_title_; }
  
  const GURL& icon_url() const { return icon_url_; }

  
  bool is_default() const { return is_default_; }
  void set_is_default(bool is_default) { is_default_ = is_default; }

 private:
  TaskDescriptor task_descriptor_;
  std::string task_title_;
  GURL icon_url_;
  bool is_default_;
};

void UpdateDefaultTask(PrefService* pref_service,
                       const std::string& task_id,
                       const std::set<std::string>& suffixes,
                       const std::set<std::string>& mime_types);

std::string GetDefaultTaskIdFromPrefs(const PrefService& pref_service,
                                      const std::string& mime_type,
                                      const std::string& suffix);

std::string MakeTaskID(const std::string& app_id,
                       TaskType task_type,
                       const std::string& action_id);

std::string MakeDriveAppTaskId(const std::string& app_id);

std::string TaskDescriptorToId(const TaskDescriptor& task_descriptor);

bool ParseTaskID(const std::string& task_id, TaskDescriptor* task);

typedef base::Callback<void(bool success)> FileTaskFinishedCallback;

bool ExecuteFileTask(Profile* profile,
                     const GURL& source_url,
                     const std::string& app_id,
                     const TaskDescriptor& task,
                     const std::vector<fileapi::FileSystemURL>& file_urls,
                     const FileTaskFinishedCallback& done);

typedef extensions::app_file_handler_util::PathAndMimeTypeSet
    PathAndMimeTypeSet;

void FindDriveAppTasks(const drive::DriveAppRegistry& drive_app_registry,
                       const PathAndMimeTypeSet& path_mime_set,
                       std::vector<FullTaskDescriptor>* result_list);

void FindFileHandlerTasks(Profile* profile,
                          const PathAndMimeTypeSet& path_mime_set,
                          std::vector<FullTaskDescriptor>* result_list);

void FindFileBrowserHandlerTasks(
    Profile* profile,
    const std::vector<GURL>& file_urls,
    std::vector<FullTaskDescriptor>* result_list);

void FindAllTypesOfTasks(
    Profile* profile,
    const drive::DriveAppRegistry* drive_app_registry,
    const PathAndMimeTypeSet& path_mime_set,
    const std::vector<GURL>& file_urls,
    std::vector<FullTaskDescriptor>* result_list);

void ChooseAndSetDefaultTask(const PrefService& pref_service,
                             const PathAndMimeTypeSet& path_mime_set,
                             std::vector<FullTaskDescriptor>* tasks);

}  
}  

#endif  
