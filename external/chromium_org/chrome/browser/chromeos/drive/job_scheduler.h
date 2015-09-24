// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_JOB_SCHEDULER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_JOB_SCHEDULER_H_

#include <vector>

#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/drive/file_system_interface.h"
#include "chrome/browser/chromeos/drive/job_list.h"
#include "chrome/browser/chromeos/drive/job_queue.h"
#include "chrome/browser/drive/drive_service_interface.h"
#include "chrome/browser/drive/drive_uploader.h"
#include "net/base/network_change_notifier.h"

class PrefService;

namespace base {
class SeqencedTaskRunner;
}

namespace drive {

class JobScheduler
    : public net::NetworkChangeNotifier::ConnectionTypeObserver,
      public JobListInterface {
 public:
  JobScheduler(PrefService* pref_service,
               DriveServiceInterface* drive_service,
               base::SequencedTaskRunner* blocking_task_runner);
  virtual ~JobScheduler();

  
  virtual std::vector<JobInfo> GetJobInfoList() OVERRIDE;
  virtual void AddObserver(JobListObserver* observer) OVERRIDE;
  virtual void RemoveObserver(JobListObserver* observer) OVERRIDE;
  virtual void CancelJob(JobID job_id) OVERRIDE;
  virtual void CancelAllJobs() OVERRIDE;

  
  
  void GetAppList(const google_apis::AppListCallback& callback);

  
  
  void GetAboutResource(const google_apis::AboutResourceCallback& callback);

  
  
  void GetAllResourceList(const google_apis::GetResourceListCallback& callback);

  
  
  void GetResourceListInDirectory(
      const std::string& directory_resource_id,
      const google_apis::GetResourceListCallback& callback);

  
  
  void Search(const std::string& search_query,
              const google_apis::GetResourceListCallback& callback);

  
  
  void GetChangeList(int64 start_changestamp,
                     const google_apis::GetResourceListCallback& callback);

  
  
  void GetRemainingChangeList(
      const GURL& next_link,
      const google_apis::GetResourceListCallback& callback);

  
  
  void GetRemainingFileList(
      const GURL& next_link,
      const google_apis::GetResourceListCallback& callback);

  
  void GetResourceEntry(const std::string& resource_id,
                        const ClientContext& context,
                        const google_apis::GetResourceEntryCallback& callback);

  
  void GetShareUrl(const std::string& resource_id,
                   const GURL& embed_origin,
                   const ClientContext& context,
                   const google_apis::GetShareUrlCallback& callback);

  
  void TrashResource(const std::string& resource_id,
                     const ClientContext& context,
                     const google_apis::EntryActionCallback& callback);

  
  void CopyResource(
      const std::string& resource_id,
      const std::string& parent_resource_id,
      const std::string& new_title,
      const base::Time& last_modified,
      const google_apis::GetResourceEntryCallback& callback);

  
  void UpdateResource(
      const std::string& resource_id,
      const std::string& parent_resource_id,
      const std::string& new_title,
      const base::Time& last_modified,
      const base::Time& last_viewed_by_me,
      const ClientContext& context,
      const google_apis::GetResourceEntryCallback& callback);

  
  void RenameResource(const std::string& resource_id,
                      const std::string& new_title,
                      const google_apis::EntryActionCallback& callback);

  
  void AddResourceToDirectory(const std::string& parent_resource_id,
                              const std::string& resource_id,
                              const google_apis::EntryActionCallback& callback);

  
  void RemoveResourceFromDirectory(
      const std::string& parent_resource_id,
      const std::string& resource_id,
      const ClientContext& context,
      const google_apis::EntryActionCallback& callback);

  
  void AddNewDirectory(const std::string& parent_resource_id,
                       const std::string& directory_title,
                       const google_apis::GetResourceEntryCallback& callback);

  
  
  
  
  JobID DownloadFile(
      const base::FilePath& virtual_path,
      int64 expected_file_size,
      const base::FilePath& local_cache_path,
      const std::string& resource_id,
      const ClientContext& context,
      const google_apis::DownloadActionCallback& download_action_callback,
      const google_apis::GetContentCallback& get_content_callback);

  
  void UploadNewFile(const std::string& parent_resource_id,
                     const base::FilePath& drive_file_path,
                     const base::FilePath& local_file_path,
                     const std::string& title,
                     const std::string& content_type,
                     const ClientContext& context,
                     const google_apis::GetResourceEntryCallback& callback);

  
  void UploadExistingFile(
      const std::string& resource_id,
      const base::FilePath& drive_file_path,
      const base::FilePath& local_file_path,
      const std::string& content_type,
      const std::string& etag,
      const ClientContext& context,
      const google_apis::GetResourceEntryCallback& callback);

  
  void CreateFile(const std::string& parent_resource_id,
                  const base::FilePath& drive_file_path,
                  const std::string& title,
                  const std::string& content_type,
                  const ClientContext& context,
                  const google_apis::GetResourceEntryCallback& callback);

  
  
  void GetResourceListInDirectoryByWapi(
      const std::string& directory_resource_id,
      const google_apis::GetResourceListCallback& callback);

  
  
  void GetRemainingResourceList(
      const GURL& next_link,
      const google_apis::GetResourceListCallback& callback);

 private:
  friend class JobSchedulerTest;

  enum QueueType {
    METADATA_QUEUE,
    FILE_QUEUE,
    NUM_QUEUES
  };

  static const int kMaxJobCount[NUM_QUEUES];

  
  struct JobEntry {
    explicit JobEntry(JobType type);
    ~JobEntry();

    
    JobInfo job_info;

    
    ClientContext context;

    
    int retry_count;

    
    base::Callback<google_apis::CancelCallback()> task;

    
    google_apis::CancelCallback cancel_callback;

    
    
    base::Callback<void(google_apis::GDataErrorCode)> abort_callback;
  };

  
  struct ResumeUploadParams;

  
  JobEntry* CreateNewJob(JobType type);

  
  
  void StartJob(JobEntry* job);

  
  void QueueJob(JobID job_id);

  
  void DoJobLoop(QueueType queue_type);

  
  
  int GetCurrentAcceptedPriority(QueueType queue_type);

  
  void UpdateWait();

  
  bool OnJobDone(JobID job_id, google_apis::GDataErrorCode error);

  
  void OnGetResourceListJobDone(
      JobID job_id,
      const google_apis::GetResourceListCallback& callback,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::ResourceList> resource_list);

  
  void OnGetResourceEntryJobDone(
      JobID job_id,
      const google_apis::GetResourceEntryCallback& callback,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::ResourceEntry> entry);

  
  void OnGetAboutResourceJobDone(
      JobID job_id,
      const google_apis::AboutResourceCallback& callback,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::AboutResource> about_resource);

  
  void OnGetShareUrlJobDone(
      JobID job_id,
      const google_apis::GetShareUrlCallback& callback,
      google_apis::GDataErrorCode error,
      const GURL& share_url);

  
  void OnGetAppListJobDone(
      JobID job_id,
      const google_apis::AppListCallback& callback,
      google_apis::GDataErrorCode error,
      scoped_ptr<google_apis::AppList> app_list);

  
  void OnEntryActionJobDone(JobID job_id,
                            const google_apis::EntryActionCallback& callback,
                            google_apis::GDataErrorCode error);

  
  void OnDownloadActionJobDone(
      JobID job_id,
      const google_apis::DownloadActionCallback& callback,
      google_apis::GDataErrorCode error,
      const base::FilePath& temp_file);

  
  void OnUploadCompletionJobDone(
      JobID job_id,
      const ResumeUploadParams& resume_params,
      const google_apis::GetResourceEntryCallback& callback,
      google_apis::GDataErrorCode error,
      const GURL& upload_location,
      scoped_ptr<google_apis::ResourceEntry> resource_entry);

  
  void OnResumeUploadFileDone(
      JobID job_id,
      const base::Callback<google_apis::CancelCallback()>& original_task,
      const google_apis::GetResourceEntryCallback& callback,
      google_apis::GDataErrorCode error,
      const GURL& upload_location,
      scoped_ptr<google_apis::ResourceEntry> resource_entry);

  
  void UpdateProgress(JobID job_id, int64 progress, int64 total);

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  
  QueueType GetJobQueueType(JobType type);

  
  void SetDisableThrottling(bool disable) { disable_throttling_ = disable; }

  
  void AbortNotRunningJob(JobEntry* job, google_apis::GDataErrorCode error);

  
  void NotifyJobAdded(const JobInfo& job_info);
  void NotifyJobDone(const JobInfo& job_info,
                     google_apis::GDataErrorCode error);
  void NotifyJobUpdated(const JobInfo& job_info);

  
  std::string GetQueueInfo(QueueType type) const;

  
  static std::string QueueTypeToString(QueueType type);

  
  
  
  int throttle_count_;

  
  base::Time wait_until_;

  
  bool disable_throttling_;

  
  scoped_ptr<JobQueue> queue_[NUM_QUEUES];

  
  typedef IDMap<JobEntry, IDMapOwnPointer> JobIDMap;
  JobIDMap job_map_;

  
  ObserverList<JobListObserver> observer_list_;

  DriveServiceInterface* drive_service_;
  scoped_ptr<DriveUploaderInterface> uploader_;

  PrefService* pref_service_;

  
  
  base::WeakPtrFactory<JobScheduler> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(JobScheduler);
};

}  

#endif  
