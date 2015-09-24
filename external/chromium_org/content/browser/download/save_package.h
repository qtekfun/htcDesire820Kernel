// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_SAVE_PACKAGE_H_
#define CONTENT_BROWSER_DOWNLOAD_SAVE_PACKAGE_H_

#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager_delegate.h"
#include "content/public/browser/save_page_type.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/referrer.h"
#include "net/base/net_errors.h"
#include "url/gurl.h"

class GURL;

namespace content {
class DownloadItemImpl;
class DownloadManagerImpl;
class WebContents;
class SaveFileManager;
class SaveItem;
class SavePackage;
struct SaveFileCreateInfo;

class CONTENT_EXPORT SavePackage
    : public base::RefCountedThreadSafe<SavePackage>,
      public WebContentsObserver,
      public DownloadItem::Observer,
      public base::SupportsWeakPtr<SavePackage> {
 public:
  enum WaitState {
    
    INITIALIZE = 0,
    
    START_PROCESS,
    
    RESOURCES_LIST,
    
    NET_FILES,
    
    HTML_DATA,
    
    SUCCESSFUL,
    
    FAILED
  };

  static const base::FilePath::CharType kDefaultHtmlExtension[];

  
  
  
  explicit SavePackage(WebContents* web_contents);

  
  
  
  SavePackage(WebContents* web_contents,
              SavePageType save_type,
              const base::FilePath& file_full_path,
              const base::FilePath& directory_full_path);

  
  
  
  
  // written to disk.
  bool Init(const SavePackageDownloadCreatedCallback& cb);

  
  void Cancel(bool user_action);

  void Finish();

  
  void StartSave(const SaveFileCreateInfo* info);
  bool UpdateSaveProgress(int32 save_id, int64 size, bool write_success);
  void SaveFinished(int32 save_id, int64 size, bool is_success);
  void SaveFailed(const GURL& save_url);
  void SaveCanceled(SaveItem* save_item);

  
  
  int PercentComplete();

  bool canceled() const { return user_canceled_ || disk_error_occurred_; }
  bool finished() const { return finished_; }
  SavePageType save_type() const { return save_type_; }
  int contents_id() const { return contents_id_; }
  int id() const { return unique_id_; }
  WebContents* web_contents() const;

  void GetSaveInfo();

 private:
  friend class base::RefCountedThreadSafe<SavePackage>;

  void InitWithDownloadItem(
      const SavePackageDownloadCreatedCallback& download_created_callback,
      DownloadItemImpl* item);

  
  void OnMHTMLGenerated(int64 size);

  
  SavePackage(WebContents* web_contents,
              const base::FilePath& file_full_path,
              const base::FilePath& directory_full_path);

  virtual ~SavePackage();

  
  void InternalInit();

  void Stop();
  void CheckFinish();
  void SaveNextFile(bool process_all_remainder_items);
  void DoSavingProcess();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void OnDownloadDestroyed(DownloadItem* download) OVERRIDE;

  
  void FinalizeDownloadEntry();

  
  void StopObservation();

  
  
  
  
  static uint32 GetMaxPathLengthForDirectory(const base::FilePath& base_dir);

  static bool GetSafePureFileName(
      const base::FilePath& dir_path,
      const base::FilePath::StringType& file_name_ext,
      uint32 max_file_path_len,
      base::FilePath::StringType* pure_file_name);

  
  bool GenerateFileName(const std::string& disposition,
                        const GURL& url,
                        bool need_html_ext,
                        base::FilePath::StringType* generated_name);

  
  
  void GetAllSavableResourceLinksForCurrentPage();
  
  
  void GetSerializedHtmlDataForCurrentPageWithLocalLinks();

  
  SaveItem* LookupItemInProcessBySaveId(int32 save_id);

  
  void PutInProgressItemToSavedMap(SaveItem* save_item);

  
  GURL GetUrlToBeSaved();

  void CreateDirectoryOnFileThread(const base::FilePath& website_save_dir,
                                   const base::FilePath& download_save_dir,
                                   bool skip_dir_check,
                                   const std::string& mime_type,
                                   const std::string& accept_langs);
  void ContinueGetSaveInfo(const base::FilePath& suggested_path,
                           bool can_save_as_complete);
  void OnPathPicked(
      const base::FilePath& final_name,
      SavePageType type,
      const SavePackageDownloadCreatedCallback& cb);
  void OnReceivedSavableResourceLinksForCurrentPage(
      const std::vector<GURL>& resources_list,
      const std::vector<Referrer>& referrers_list,
      const std::vector<GURL>& frames_list);

  void OnReceivedSerializedHtmlData(const GURL& frame_url,
                                    const std::string& data,
                                    int32 status);

  typedef base::hash_map<std::string, SaveItem*> SaveUrlItemMap;
  
  SaveUrlItemMap in_progress_items_;
  
  SaveUrlItemMap saved_failed_items_;

  
  int in_process_count() const {
    return static_cast<int>(in_progress_items_.size());
  }

  
  
  int completed_count() const {
    return static_cast<int>(saved_success_items_.size() +
                            saved_failed_items_.size());
  }

  
  
  
  
  
  int64 CurrentSpeed() const;

  
  
  base::FilePath GetSuggestedNameForSaveAs(
      bool can_save_as_complete,
      const std::string& contents_mime_type,
      const std::string& accept_langs);

  
  
  static base::FilePath EnsureHtmlExtension(const base::FilePath& name);

  
  
  static base::FilePath EnsureMimeExtension(const base::FilePath& name,
      const std::string& contents_mime_type);

  
  
  static const base::FilePath::CharType* ExtensionForMimeType(
      const std::string& contents_mime_type);

  typedef std::queue<SaveItem*> SaveItemQueue;
  
  SaveItemQueue waiting_item_queue_;

  typedef base::hash_map<int32, SaveItem*> SavedItemMap;
  
  SavedItemMap saved_success_items_;

  
  SaveFileManager* file_manager_;

  
  DownloadManagerImpl* download_manager_;
  DownloadItemImpl* download_;

  
  GURL page_url_;
  base::FilePath saved_main_file_path_;
  base::FilePath saved_main_directory_path_;

  
  base::string16 title_;

  
  base::TimeTicks start_tick_;

  
  bool finished_;

  
  bool mhtml_finishing_;

  
  bool user_canceled_;

  
  bool disk_error_occurred_;

  
  SavePageType save_type_;

  
  size_t all_save_items_count_;

  typedef std::set<base::FilePath::StringType,
                   bool (*)(const base::FilePath::StringType&,
                            const base::FilePath::StringType&)> FileNameSet;
  
  FileNameSet file_name_set_;

  typedef base::hash_map<base::FilePath::StringType, uint32> FileNameCountMap;
  
  FileNameCountMap file_name_count_map_;

  
  
  WaitState wait_state_;

  
  
  const int contents_id_;

  
  const int unique_id_;

  
  
  bool wrote_to_completed_file_;
  bool wrote_to_failed_file_;

  friend class SavePackageTest;
  FRIEND_TEST_ALL_PREFIXES(SavePackageTest, TestSuggestedSaveNames);
  FRIEND_TEST_ALL_PREFIXES(SavePackageTest, TestLongSafePureFilename);

  DISALLOW_COPY_AND_ASSIGN(SavePackage);
};

}  

#endif  
