// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_IMPL_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/browser/download/download_net_log_parameters.h"
#include "content/browser/download/download_request_handle.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_destination_observer.h"
#include "content/public/browser/download_item.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "url/gurl.h"

namespace content {
class DownloadFile;
class DownloadItemImplDelegate;

class CONTENT_EXPORT DownloadItemImpl
    : public DownloadItem,
      public DownloadDestinationObserver {
 public:
  enum ResumeMode {
    RESUME_MODE_INVALID = 0,
    RESUME_MODE_IMMEDIATE_CONTINUE,
    RESUME_MODE_IMMEDIATE_RESTART,
    RESUME_MODE_USER_CONTINUE,
    RESUME_MODE_USER_RESTART
  };

  
  static const int kMaxAutoResumeAttempts;

  
  
  

  
  
  DownloadItemImpl(DownloadItemImplDelegate* delegate,
                   uint32 id,
                   const base::FilePath& current_path,
                   const base::FilePath& target_path,
                   const std::vector<GURL>& url_chain,
                   const GURL& referrer_url,
                   const base::Time& start_time,
                   const base::Time& end_time,
                   const std::string& etag,
                   const std::string& last_modified,
                   int64 received_bytes,
                   int64 total_bytes,
                   DownloadItem::DownloadState state,
                   DownloadDangerType danger_type,
                   DownloadInterruptReason interrupt_reason,
                   bool opened,
                   const net::BoundNetLog& bound_net_log);

  
  
  DownloadItemImpl(DownloadItemImplDelegate* delegate,
                   uint32 id,
                   const DownloadCreateInfo& info,
                   const net::BoundNetLog& bound_net_log);

  
  
  DownloadItemImpl(DownloadItemImplDelegate* delegate,
                   uint32 id,
                   const base::FilePath& path,
                   const GURL& url,
                   const std::string& mime_type,
                   scoped_ptr<DownloadRequestHandleInterface> request_handle,
                   const net::BoundNetLog& bound_net_log);

  virtual ~DownloadItemImpl();

  
  virtual void AddObserver(DownloadItem::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(DownloadItem::Observer* observer) OVERRIDE;
  virtual void UpdateObservers() OVERRIDE;
  virtual void ValidateDangerousDownload() OVERRIDE;
  virtual void StealDangerousDownload(const AcquireFileCallback& callback)
      OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual void Resume() OVERRIDE;
  virtual void Cancel(bool user_cancel) OVERRIDE;
  virtual void Remove() OVERRIDE;
  virtual void OpenDownload() OVERRIDE;
  virtual void ShowDownloadInShell() OVERRIDE;
  virtual uint32 GetId() const OVERRIDE;
  virtual DownloadState GetState() const OVERRIDE;
  virtual DownloadInterruptReason GetLastReason() const OVERRIDE;
  virtual bool IsPaused() const OVERRIDE;
  virtual bool IsTemporary() const OVERRIDE;
  virtual bool CanResume() const OVERRIDE;
  virtual bool IsDone() const OVERRIDE;
  virtual const GURL& GetURL() const OVERRIDE;
  virtual const std::vector<GURL>& GetUrlChain() const OVERRIDE;
  virtual const GURL& GetOriginalUrl() const OVERRIDE;
  virtual const GURL& GetReferrerUrl() const OVERRIDE;
  virtual std::string GetSuggestedFilename() const OVERRIDE;
  virtual std::string GetContentDisposition() const OVERRIDE;
  virtual std::string GetMimeType() const OVERRIDE;
  virtual std::string GetOriginalMimeType() const OVERRIDE;
  virtual std::string GetRemoteAddress() const OVERRIDE;
  virtual bool HasUserGesture() const OVERRIDE;
  virtual PageTransition GetTransitionType() const OVERRIDE;
  virtual const std::string& GetLastModifiedTime() const OVERRIDE;
  virtual const std::string& GetETag() const OVERRIDE;
  virtual bool IsSavePackageDownload() const OVERRIDE;
  virtual const base::FilePath& GetFullPath() const OVERRIDE;
  virtual const base::FilePath& GetTargetFilePath() const OVERRIDE;
  virtual const base::FilePath& GetForcedFilePath() const OVERRIDE;
  virtual base::FilePath GetFileNameToReportUser() const OVERRIDE;
  virtual TargetDisposition GetTargetDisposition() const OVERRIDE;
  virtual const std::string& GetHash() const OVERRIDE;
  virtual const std::string& GetHashState() const OVERRIDE;
  virtual bool GetFileExternallyRemoved() const OVERRIDE;
  virtual void DeleteFile() OVERRIDE;
  virtual bool IsDangerous() const OVERRIDE;
  virtual DownloadDangerType GetDangerType() const OVERRIDE;
  virtual bool TimeRemaining(base::TimeDelta* remaining) const OVERRIDE;
  virtual int64 CurrentSpeed() const OVERRIDE;
  virtual int PercentComplete() const OVERRIDE;
  virtual bool AllDataSaved() const OVERRIDE;
  virtual int64 GetTotalBytes() const OVERRIDE;
  virtual int64 GetReceivedBytes() const OVERRIDE;
  virtual base::Time GetStartTime() const OVERRIDE;
  virtual base::Time GetEndTime() const OVERRIDE;
  virtual bool CanShowInFolder() OVERRIDE;
  virtual bool CanOpenDownload() OVERRIDE;
  virtual bool ShouldOpenFileBasedOnExtension() OVERRIDE;
  virtual bool GetOpenWhenComplete() const OVERRIDE;
  virtual bool GetAutoOpened() OVERRIDE;
  virtual bool GetOpened() const OVERRIDE;
  virtual BrowserContext* GetBrowserContext() const OVERRIDE;
  virtual WebContents* GetWebContents() const OVERRIDE;
  virtual void OnContentCheckCompleted(DownloadDangerType danger_type) OVERRIDE;
  virtual void SetOpenWhenComplete(bool open) OVERRIDE;
  virtual void SetIsTemporary(bool temporary) OVERRIDE;
  virtual void SetOpened(bool opened) OVERRIDE;
  virtual void SetDisplayName(const base::FilePath& name) OVERRIDE;
  virtual std::string DebugString(bool verbose) const OVERRIDE;

  
  

  
  
  
  virtual ResumeMode GetResumeMode() const;

  
  
  virtual void MergeOriginInfoOnResume(
      const DownloadCreateInfo& new_create_info);

  

  
  
  
  virtual void Start(scoped_ptr<DownloadFile> download_file,
                     scoped_ptr<DownloadRequestHandleInterface> req_handle);

  

  
  

  
  virtual void NotifyRemoved();

  virtual void OnDownloadedFileRemoved();

  
  
  virtual base::WeakPtr<DownloadDestinationObserver>
      DestinationObserverAsWeakPtr();

  
  virtual const net::BoundNetLog& GetBoundNetLog() const;

  

  
  virtual void SetTotalBytes(int64 total_bytes);

  virtual void OnAllDataSaved(const std::string& final_hash);

  
  
  virtual void MarkAsComplete();

  
  virtual void DestinationUpdate(int64 bytes_so_far,
                                 int64 bytes_per_sec,
                                 const std::string& hash_state) OVERRIDE;
  virtual void DestinationError(DownloadInterruptReason reason) OVERRIDE;
  virtual void DestinationCompleted(const std::string& final_hash) OVERRIDE;

 private:
  
  
  
  

  enum DownloadInternalState {
    
    
    
    
    
    
    
    
    
    
    
    IN_PROGRESS_INTERNAL,

    
    
    
    
    
    
    COMPLETING_INTERNAL,

    
    
    
    
    
    
    
    
    COMPLETE_INTERNAL,

    
    
    
    
    
    
    
    
    CANCELLED_INTERNAL,

    
    
    
    
    
    
    
    INTERRUPTED_INTERNAL,

    
    
    
    
    
    
    
    
    RESUMING_INTERNAL,

    MAX_DOWNLOAD_INTERNAL_STATE,
  };

  
  
  enum ShouldUpdateObservers {
    UPDATE_OBSERVERS,
    DONT_UPDATE_OBSERVERS
  };

  

  
  
  

  
  
  
  
  void Init(bool active, DownloadType download_type);

  
  
  
  
  
  virtual void OnDownloadTargetDetermined(
      const base::FilePath& target_path,
      TargetDisposition disposition,
      DownloadDangerType danger_type,
      const base::FilePath& intermediate_path);

  
  void OnDownloadFileInitialized(DownloadInterruptReason result);

  void OnDownloadRenamedToIntermediateName(
      DownloadInterruptReason reason, const base::FilePath& full_path);

  
  
  
  void MaybeCompleteDownload();

  
  
  
  void OnDownloadCompleting();

  void OnDownloadRenamedToFinalName(DownloadInterruptReason reason,
                                    const base::FilePath& full_path);

  
  
  void DelayedDownloadOpened(bool auto_opened);

  
  
  void Completed();

  
  void OnResumeRequestStarted(DownloadItem* item, net::Error error);

  

  
  void Interrupt(DownloadInterruptReason reason);

  
  
  
  
  void ReleaseDownloadFile(bool destroy_file);

  
  
  
  bool IsDownloadReadyForCompletion(const base::Closure& state_change_notify);

  
  
  
  void TransitionTo(DownloadInternalState new_state,
                    ShouldUpdateObservers notify_action);

  
  void SetDangerType(DownloadDangerType danger_type);

  void SetFullPath(const base::FilePath& new_path);

  void AutoResumeIfValid();

  void ResumeInterruptedDownload();

  static DownloadState InternalToExternalState(
      DownloadInternalState internal_state);
  static DownloadInternalState ExternalToInternalState(
      DownloadState external_state);

  
  static const char* DebugDownloadStateString(DownloadInternalState state);
  static const char* DebugResumeModeString(ResumeMode mode);

  
  
  const bool is_save_package_download_;

  
  
  scoped_ptr<DownloadRequestHandleInterface> request_handle_;

  uint32 download_id_;

  
  
  base::FilePath display_name_;

  
  
  
  
  base::FilePath current_path_;

  
  
  
  base::FilePath target_path_;

  // Whether the target should be overwritten, uniquified or prompted for.
  TargetDisposition target_disposition_;

  
  std::vector<GURL> url_chain_;

  
  GURL referrer_url_;

  
  
  
  std::string suggested_filename_;

  
  
  base::FilePath forced_file_path_;

  
  PageTransition transition_type_;

  
  bool has_user_gesture_;

  
  
  std::string content_disposition_;

  
  std::string mime_type_;

  
  
  
  std::string original_mime_type_;

  
  
  std::string remote_address_;

  
  int64 total_bytes_;

  
  int64 received_bytes_;

  
  int64 bytes_per_sec_;

  
  
  
  std::string hash_;

  
  
  std::string hash_state_;

  
  std::string last_modified_time_;

  
  std::string etag_;

  
  DownloadInterruptReason last_reason_;

  
  base::TimeTicks start_tick_;

  
  DownloadInternalState state_;

  
  DownloadDangerType danger_type_;

  
  ObserverList<Observer> observers_;

  
  base::Time start_time_;

  
  base::Time end_time_;

  
  DownloadItemImplDelegate* delegate_;

  
  bool is_paused_;

  
  int auto_resume_count_;

  
  bool open_when_complete_;

  
  bool file_externally_removed_;

  
  
  
  bool auto_opened_;

  
  bool is_temporary_;

  
  bool all_data_saved_;

  
  
  
  DownloadInterruptReason destination_error_;

  
  
  
  
  bool opened_;

  
  bool delegate_delayed_complete_;

  
  
  
  
  scoped_ptr<DownloadFile> download_file_;

  
  const net::BoundNetLog bound_net_log_;

  base::WeakPtrFactory<DownloadItemImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemImpl);
};

}  

#endif  
