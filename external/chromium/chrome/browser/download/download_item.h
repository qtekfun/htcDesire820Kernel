// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/observer_list.h"
#include "base/time.h"
#include "base/timer.h"
#include "googleurl/src/gurl.h"

class DownloadFileManager;
class DownloadManager;
struct DownloadCreateInfo;

class DownloadItem {
 public:
  enum DownloadState {
    
    IN_PROGRESS,

    
    COMPLETE,

    
    CANCELLED,

    
    
    REMOVING,

    
    INTERRUPTED
  };

  enum SafetyState {
    SAFE = 0,
    DANGEROUS,
    DANGEROUS_BUT_VALIDATED  
  };

  
  
  enum DangerType {
    NOT_DANGEROUS = 0,

    
    
    DANGEROUS_FILE,

    
    DANGEROUS_URL,

    
    
    DANGEROUS_TYPE_MAX
  };

  
  enum DeleteReason {
    DELETE_DUE_TO_BROWSER_SHUTDOWN = 0,
    DELETE_DUE_TO_USER_DISCARD
  };

  
  
  class Observer {
   public:
    virtual void OnDownloadUpdated(DownloadItem* download) = 0;

    
    virtual void OnDownloadOpened(DownloadItem* download) = 0;

   protected:
    virtual ~Observer() {}
  };

  
  DownloadItem(DownloadManager* download_manager,
               const DownloadCreateInfo& info);

  
  DownloadItem(DownloadManager* download_manager,
               const DownloadCreateInfo& info,
               bool is_otr);

  
  DownloadItem(DownloadManager* download_manager,
               const FilePath& path,
               const GURL& url,
               bool is_otr);

  ~DownloadItem();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  void UpdateObservers();

  
  bool CanOpenDownload();

  
  bool ShouldOpenFileBasedOnExtension();

  
  
  
  void OpenFilesBasedOnExtension(bool open);

  
  
  void OpenDownload();

  
  void ShowDownloadInShell();

  
  void DangerousDownloadValidated();

  
  void Update(int64 bytes_so_far);

  
  
  
  
  
  
  
  
  
  void Cancel(bool update_history);

  
  void OnAllDataSaved(int64 size);

  
  
  void MarkAsComplete();

  
  
  
  void Interrupted(int64 size, int os_error);

  
  
  
  
  void Delete(DeleteReason reason);

  
  void Remove();

  
  
  
  
  bool TimeRemaining(base::TimeDelta* remaining) const;

  
  int64 CurrentSpeed() const;

  
  
  int PercentComplete() const;

  
  bool all_data_saved() const { return all_data_saved_; }

  
  
  
  void SetFileCheckResults(const FilePath& path,
                           bool is_dangerous_file,
                           bool is_dangerous_url,
                           int path_uniquifier,
                           bool prompt,
                           bool is_extension_install,
                           const FilePath& original_name);

  
  
  void Rename(const FilePath& full_path);

  
  void TogglePause();

  
  void OnNameFinalized();

  
  
  
  void OnDownloadCompleting(DownloadFileManager* file_manager);

  
  void OnDownloadRenamedToFinalName(const FilePath& full_path);

  
  bool MatchesQuery(const string16& query) const;

  
  bool IsPartialDownload() const;

  
  bool IsInProgress() const;

  
  bool IsCancelled() const;

  
  bool IsInterrupted() const;

  
  bool IsComplete() const;

  
  DownloadState state() const { return state_; }
  FilePath full_path() const { return full_path_; }
  void set_path_uniquifier(int uniquifier) { path_uniquifier_ = uniquifier; }
  const GURL& url() const { return url_chain_.back(); }
  const std::vector<GURL>& url_chain() const { return url_chain_; }
  const GURL& original_url() const { return url_chain_.front(); }
  const GURL& referrer_url() const { return referrer_url_; }
  std::string mime_type() const { return mime_type_; }
  std::string original_mime_type() const { return original_mime_type_; }
  int64 total_bytes() const { return total_bytes_; }
  void set_total_bytes(int64 total_bytes) { total_bytes_ = total_bytes; }
  int64 received_bytes() const { return received_bytes_; }
  int32 id() const { return id_; }
  base::Time start_time() const { return start_time_; }
  void set_db_handle(int64 handle) { db_handle_ = handle; }
  int64 db_handle() const { return db_handle_; }
  bool is_paused() const { return is_paused_; }
  bool open_when_complete() const { return open_when_complete_; }
  void set_open_when_complete(bool open) { open_when_complete_ = open; }
  int render_process_id() const { return render_process_id_; }
  int request_id() const { return request_id_; }
  SafetyState safety_state() const { return safety_state_; }
  void set_safety_state(SafetyState safety_state) {
    safety_state_ = safety_state;
  }
  DangerType danger_type() { return danger_type_;}
  bool auto_opened() { return auto_opened_; }
  FilePath target_name() const { return target_name_; }
  bool save_as() const { return save_as_; }
  bool is_otr() const { return is_otr_; }
  bool is_extension_install() const { return is_extension_install_; }
  bool name_finalized() const { return name_finalized_; }
  bool is_temporary() const { return is_temporary_; }
  void set_opened(bool opened) { opened_ = opened; }
  bool opened() const { return opened_; }

  
  FilePath GetTargetFilePath() const;

  
  
  FilePath GetFileNameToReportUser() const;

  
  
  
  FilePath GetUserVerifiedFilePath() const;

  
  bool NeedsRename() const {
    return target_name_ != full_path_.BaseName();
  }

  std::string DebugString(bool verbose) const;

 private:
  void Init(bool start_timer);

  
  void UpdateSize(int64 size);

  
  
  void Completed();

  
  void StartProgressTimer();
  void StopProgressTimer();

  
  int32 id_;

  
  FilePath full_path_;

  
  
  int path_uniquifier_;

  
  std::vector<GURL> url_chain_;

  
  GURL referrer_url_;

  
  std::string mime_type_;

  
  
  std::string original_mime_type_;

  
  int64 total_bytes_;

  
  int64 received_bytes_;

  
  int last_os_error_;

  
  base::TimeTicks start_tick_;

  
  DownloadState state_;

  
  ObserverList<Observer> observers_;

  
  base::Time start_time_;

  
  int64 db_handle_;

  
  base::RepeatingTimer<DownloadItem> update_timer_;

  
  DownloadManager* download_manager_;

  
  bool is_paused_;

  
  bool open_when_complete_;

  
  
  SafetyState safety_state_;

  
  DangerType danger_type_;

  
  
  
  bool auto_opened_;

  
  
  
  FilePath target_name_;

  
  int render_process_id_;
  int request_id_;

  
  bool save_as_;

  
  bool is_otr_;

  
  bool is_extension_install_;

  
  bool name_finalized_;

  
  bool is_temporary_;

  
  bool all_data_saved_;

  
  
  
  
  bool opened_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItem);
};

#endif  
