// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_SAVE_PACKAGE_H_
#define CHROME_BROWSER_DOWNLOAD_SAVE_PACKAGE_H_
#pragma once

#include <queue>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "googleurl/src/gurl.h"

class DownloadItem;
class DownloadManager;
class GURL;
class MessageLoop;
class PrefService;
class Profile;
struct SaveFileCreateInfo;
class SaveFileManager;
class SaveItem;
class SavePackage;
struct SavePackageParam;
class TabContents;

namespace base {
class Thread;
class Time;
}

namespace net {
class URLRequestContextGetter;
}


class SavePackage : public base::RefCountedThreadSafe<SavePackage>,
                    public TabContentsObserver,
                    public SelectFileDialog::Listener {
 public:
  enum SavePackageType {
    
    SAVE_TYPE_UNKNOWN = -1,
    
    SAVE_AS_ONLY_HTML = 0,
    
    SAVE_AS_COMPLETE_HTML = 1
  };

  enum WaitState {
    
    INITIALIZE = 0,
    
    START_PROCESS,
    
    RESOURCES_LIST,
    
    NET_FILES,
    
    HTML_DATA,
    
    SUCCESSFUL,
    
    FAILED
  };

  
  
  
  explicit SavePackage(TabContents* tab_contents);

  
  
  
  SavePackage(TabContents* tab_contents,
              SavePackageType save_type,
              const FilePath& file_full_path,
              const FilePath& directory_full_path);

  
  
  
  
  bool Init();

  void Cancel(bool user_action);

  void Finish();

  
  void StartSave(const SaveFileCreateInfo* info);
  bool UpdateSaveProgress(int32 save_id, int64 size, bool write_success);
  void SaveFinished(int32 save_id, int64 size, bool is_success);
  void SaveFailed(const GURL& save_url);
  void SaveCanceled(SaveItem* save_item);

  
  
  int PercentComplete();

  
  void ShowDownloadInShell();

  bool canceled() const { return user_canceled_ || disk_error_occurred_; }
  bool finished() const { return finished_; }
  SavePackageType save_type() const { return save_type_; }
  int tab_id() const { return tab_id_; }
  int id() const { return unique_id_; }

  void GetSaveInfo();

  

  
  
  static void SetShouldPromptUser(bool should_prompt);

  
  static bool IsSavableURL(const GURL& url);

  
  
  static bool IsSavableContents(const std::string& contents_mime_type);

  
  virtual void FileSelected(const FilePath& path, int index, void* params);
  virtual void FileSelectionCanceled(void* params);

 private:
  friend class base::RefCountedThreadSafe<SavePackage>;

  
  SavePackage(TabContents* tab_contents,
              const FilePath& file_full_path,
              const FilePath& directory_full_path);

  ~SavePackage();

  
  void InternalInit();

  void Stop();
  void CheckFinish();
  void SaveNextFile(bool process_all_remainder_items);
  void DoSavingProcess();

  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  
  
  
  static uint32 GetMaxPathLengthForDirectory(const FilePath& base_dir);

  static bool GetSafePureFileName(const FilePath& dir_path,
                                  const FilePath::StringType& file_name_ext,
                                  uint32 max_file_path_len,
                                  FilePath::StringType* pure_file_name);

  
  bool GenerateFileName(const std::string& disposition,
                        const GURL& url,
                        bool need_html_ext,
                        FilePath::StringType* generated_name);

  
  
  void GetAllSavableResourceLinksForCurrentPage();
  
  
  void GetSerializedHtmlDataForCurrentPageWithLocalLinks();

  SaveItem* LookupItemInProcessBySaveId(int32 save_id);
  void PutInProgressItemToSavedMap(SaveItem* save_item);

  
  GURL GetUrlToBeSaved();

  void CreateDirectoryOnFileThread(const FilePath& website_save_dir,
                                   const FilePath& download_save_dir,
                                   const std::string& mime_type);
  void ContinueGetSaveInfo(const FilePath& suggested_path,
                           bool can_save_as_complete);
  void ContinueSave(const FilePath& final_name, int index);

  void OnReceivedSavableResourceLinksForCurrentPage(
      const std::vector<GURL>& resources_list,
      const std::vector<GURL>& referrers_list,
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

  
  static FilePath GetSaveDirPreference(PrefService* prefs);

  
  
  FilePath GetSuggestedNameForSaveAs(
      bool can_save_as_complete,
      const std::string& contents_mime_type);

  
  
  static FilePath EnsureHtmlExtension(const FilePath& name);

  
  
  static FilePath EnsureMimeExtension(const FilePath& name,
      const std::string& contents_mime_type);

  
  
  static const FilePath::CharType* ExtensionForMimeType(
      const std::string& contents_mime_type);

  typedef std::queue<SaveItem*> SaveItemQueue;
  
  SaveItemQueue waiting_item_queue_;

  typedef base::hash_map<int32, SaveItem*> SavedItemMap;
  
  SavedItemMap saved_success_items_;

  
  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  SaveFileManager* file_manager_;

  
  
  DownloadItem* download_;

  
  GURL page_url_;
  FilePath saved_main_file_path_;
  FilePath saved_main_directory_path_;

  
  string16 title_;

  
  bool finished_;

  
  bool user_canceled_;

  
  bool disk_error_occurred_;

  
  SavePackageType save_type_;

  
  size_t all_save_items_count_;

  typedef base::hash_set<FilePath::StringType> FileNameSet;
  
  FileNameSet file_name_set_;

  typedef base::hash_map<FilePath::StringType, uint32> FileNameCountMap;
  
  FileNameCountMap file_name_count_map_;

  
  
  WaitState wait_state_;

  
  
  const int tab_id_;

  
  const int unique_id_;

  
  scoped_refptr<SelectFileDialog> select_file_dialog_;

  friend class SavePackageTest;
  FRIEND_TEST_ALL_PREFIXES(SavePackageTest, TestSuggestedSaveNames);
  FRIEND_TEST_ALL_PREFIXES(SavePackageTest, TestLongSafePureFilename);

  ScopedRunnableMethodFactory<SavePackage> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(SavePackage);
};

#endif  
