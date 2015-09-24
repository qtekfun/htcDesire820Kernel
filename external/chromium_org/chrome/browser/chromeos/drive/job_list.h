// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_JOB_LIST_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_JOB_LIST_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "chrome/browser/chromeos/drive/file_errors.h"

namespace drive {

enum JobType {
  TYPE_GET_ABOUT_RESOURCE,
  TYPE_GET_APP_LIST,
  TYPE_GET_ALL_RESOURCE_LIST,
  TYPE_GET_RESOURCE_LIST_IN_DIRECTORY,
  TYPE_SEARCH,
  TYPE_GET_CHANGE_LIST,
  TYPE_GET_REMAINING_CHANGE_LIST,
  TYPE_GET_REMAINING_FILE_LIST,
  TYPE_GET_RESOURCE_ENTRY,
  TYPE_GET_SHARE_URL,
  TYPE_TRASH_RESOURCE,
  TYPE_COPY_RESOURCE,
  TYPE_UPDATE_RESOURCE,
  TYPE_RENAME_RESOURCE,
  TYPE_ADD_RESOURCE_TO_DIRECTORY,
  TYPE_REMOVE_RESOURCE_FROM_DIRECTORY,
  TYPE_ADD_NEW_DIRECTORY,
  TYPE_DOWNLOAD_FILE,
  TYPE_UPLOAD_NEW_FILE,
  TYPE_UPLOAD_EXISTING_FILE,
  TYPE_CREATE_FILE,
  TYPE_GET_RESOURCE_LIST_IN_DIRECTORY_BY_WAPI,
  TYPE_GET_REMAINING_RESOURCE_LIST,
};

std::string JobTypeToString(JobType type);

enum JobState {
  
  STATE_NONE,

  
  STATE_RUNNING,

  
  STATE_RETRY,
};

std::string JobStateToString(JobState state);

typedef int32 JobID;

struct JobInfo {
  explicit JobInfo(JobType job_type);

  
  JobType job_type;

  
  JobID job_id;

  
  JobState state;

  
  

  
  int64 num_completed_bytes;

  
  int64 num_total_bytes;

  
  base::FilePath file_path;

  
  base::Time start_time;

  
  std::string ToString() const;
};

bool IsActiveFileTransferJobInfo(const JobInfo& job_info);

class JobListObserver {
 public:
  
  virtual void OnJobAdded(const JobInfo& job_info) {}

  
  
  
  virtual void OnJobDone(const JobInfo& job_info,
                         FileError error) {}

  
  virtual void OnJobUpdated(const JobInfo& job_info) {}

 protected:
  virtual ~JobListObserver() {}
};

class JobListInterface {
 public:
  virtual ~JobListInterface() {}

  
  virtual std::vector<JobInfo> GetJobInfoList() = 0;

  
  virtual void AddObserver(JobListObserver* observer) = 0;

  
  virtual void RemoveObserver(JobListObserver* observer) = 0;

  
  virtual void CancelJob(JobID job_id) = 0;

  
  virtual void CancelAllJobs() = 0;
};

}  

#endif  
