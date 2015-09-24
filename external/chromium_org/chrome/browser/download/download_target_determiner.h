// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TARGET_DETERMINER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TARGET_DETERMINER_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/download/download_path_reservation_tracker.h"
#include "chrome/browser/download/download_target_determiner_delegate.h"
#include "chrome/browser/download/download_target_info.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager_delegate.h"

class ChromeDownloadManagerDelegate;
class Profile;
class DownloadPrefs;

namespace content {
enum DownloadDangerType;
}

//   Intermediate Path: Where the data should be written to during the course of
class DownloadTargetDeterminer
    : public content::DownloadItem::Observer {
 public:
  typedef base::Callback<void(scoped_ptr<DownloadTargetInfo>)>
      CompletionCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void Start(content::DownloadItem* download,
                    const base::FilePath& initial_virtual_path,
                    DownloadPrefs* download_prefs,
                    DownloadTargetDeterminerDelegate* delegate,
                    const CompletionCallback& callback);

  
  static base::FilePath GetCrDownloadPath(const base::FilePath& suggested_path);

 private:
  
  
  
  
  
  enum State {
    STATE_GENERATE_TARGET_PATH,
    STATE_NOTIFY_EXTENSIONS,
    STATE_RESERVE_VIRTUAL_PATH,
    STATE_PROMPT_USER_FOR_DOWNLOAD_PATH,
    STATE_DETERMINE_LOCAL_PATH,
    STATE_DETERMINE_MIME_TYPE,
    STATE_DETERMINE_IF_HANDLED_BY_BROWSER,
    STATE_CHECK_DOWNLOAD_URL,
    STATE_CHECK_VISITED_REFERRER_BEFORE,
    STATE_DETERMINE_INTERMEDIATE_PATH,
    STATE_NONE,
  };

  
  
  enum Result {
    
    CONTINUE,

    
    
    
    
    
    QUIT_DOLOOP,

    
    COMPLETE
  };

  
  
  enum PriorVisitsToReferrer {
    NO_VISITS_TO_REFERRER,
    VISITED_REFERRER,
  };

  
  
  DownloadTargetDeterminer(content::DownloadItem* download,
                           const base::FilePath& initial_virtual_path,
                           DownloadPrefs* download_prefs,
                           DownloadTargetDeterminerDelegate* delegate,
                           const CompletionCallback& callback);

  virtual ~DownloadTargetDeterminer();

  
  
  
  void DoLoop();

  

  
  
  
  
  
  Result DoGenerateTargetPath();

  
  
  
  
  
  Result DoNotifyExtensions();

  
  
  void NotifyExtensionsDone(
      const base::FilePath& new_path,
      DownloadPathReservationTracker::FilenameConflictAction conflict_action);

  
  
  
  
  Result DoReserveVirtualPath();

  
  void ReserveVirtualPathDone(const base::FilePath& path, bool verified);

  
  
  
  Result DoPromptUserForDownloadPath();

  
  
  void PromptUserForDownloadPathDone(const base::FilePath& virtual_path);

  
  
  
  
  
  
  Result DoDetermineLocalPath();

  
  void DetermineLocalPathDone(const base::FilePath& local_path);

  
  
  
  
  
  
  
  Result DoDetermineMimeType();

  
  
  void DetermineMimeTypeDone(const std::string& mime_type);

  
  
  
  
  Result DoDetermineIfHandledByBrowser();

  
  
  
  
  
  void DetermineIfHandledByBrowserDone(
      const base::Callback<bool(Profile*)>& per_profile_handler_checker);

  
  
  
  
  Result DoCheckDownloadUrl();

  
  
  void CheckDownloadUrlDone(content::DownloadDangerType danger_type);

  
  
  
  
  
  Result DoCheckVisitedReferrerBefore();

  
  
  void CheckVisitedReferrerBeforeDone(bool visited_referrer_before);

  
  
  // intermediate file will never be overwritten (always uniquified if needed).
  
  
  Result DoDetermineIntermediatePath();

  

  

  void ScheduleCallbackAndDeleteSelf();

  void CancelOnFailureAndDeleteSelf();

  Profile* GetProfile();

  
  
  
  
  
  
  bool ShouldPromptForDownload(const base::FilePath& filename) const;

  
  
  
  
  
  
  bool HasPromptedForPath() const;

  
  
  
  
  bool IsDangerousFile(PriorVisitsToReferrer visits);

  
  virtual void OnDownloadDestroyed(content::DownloadItem* download) OVERRIDE;

  
  State next_state_;
  bool should_prompt_;
  bool should_notify_extensions_;
  bool create_target_directory_;
  DownloadPathReservationTracker::FilenameConflictAction conflict_action_;
  content::DownloadDangerType danger_type_;
  base::FilePath virtual_path_;
  base::FilePath local_path_;
  base::FilePath intermediate_path_;
  std::string mime_type_;
  bool is_filetype_handled_securely_;

  content::DownloadItem* download_;
  const bool is_resumption_;
  DownloadPrefs* download_prefs_;
  DownloadTargetDeterminerDelegate* delegate_;
  CompletionCallback completion_callback_;
  CancelableRequestConsumer history_consumer_;

  base::WeakPtrFactory<DownloadTargetDeterminer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadTargetDeterminer);
};

#endif  
