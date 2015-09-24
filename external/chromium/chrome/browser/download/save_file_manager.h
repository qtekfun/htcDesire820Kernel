// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_SAVE_FILE_MANAGER_H__
#define CHROME_BROWSER_DOWNLOAD_SAVE_FILE_MANAGER_H__
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/download/save_types.h"

namespace net {
class IOBuffer;
}
class FilePath;
class GURL;
class SaveFile;
class SavePackage;
class ResourceDispatcherHost;
class Task;

namespace net {
class URLRequestContextGetter;
}

class SaveFileManager
    : public base::RefCountedThreadSafe<SaveFileManager> {
 public:
  explicit SaveFileManager(ResourceDispatcherHost* rdh);

  
  void Shutdown();

  
  int GetNextId();

  
  
  void SaveURL(const GURL& url,
               const GURL& referrer,
               int render_process_host_id,
               int render_view_id,
               SaveFileCreateInfo::SaveFileSource save_source,
               const FilePath& file_full_path,
               net::URLRequestContextGetter* request_context_getter,
               SavePackage* save_package);

  
  void StartSave(SaveFileCreateInfo* info);
  void UpdateSaveProgress(int save_id, net::IOBuffer* data, int size);
  void SaveFinished(int save_id,
                    const GURL& save_url,
                    int render_process_id,
                    bool is_success);

  
  
  void CancelSave(int save_id);

  
  
  void RemoveSaveFile(int save_id, const GURL& save_url,
                      SavePackage* package);

#if !defined(OS_MACOSX)
  
  
  
  void OnShowSavedFileInShell(const FilePath full_path);
#endif

  
  void DeleteDirectoryOrFile(const FilePath& full_path, bool is_dir);

  
  
  void SaveLocalFile(const GURL& original_file_url,
                     int save_id,
                     int render_process_id);

  
  
  
  void RenameAllFiles(
      const FinalNameList& final_names,
      const FilePath& resource_dir,
      int render_process_id,
      int render_view_id,
      int save_package_id);

  
  
  void RemoveSavedFileFromFileMap(const SaveIDList & save_ids);

 private:
  friend class base::RefCountedThreadSafe<SaveFileManager>;

  ~SaveFileManager();

  
  void OnShutdown();

  
  static SavePackage* GetSavePackageFromRenderIds(int render_process_id,
                                                  int review_view_id);

  
  
  void RegisterStartingRequest(const GURL& save_url,
                               SavePackage* save_package);
  
  
  SavePackage* UnregisterStartingRequest(const GURL& save_url,
                                         int tab_id);

  
  SavePackage* LookupPackage(int save_id);

  
  
  SaveFile* LookupSaveFile(int save_id);

  
  void SendCancelRequest(int save_id);

  

  
  
  void OnStartSave(const SaveFileCreateInfo* info);
  
  
  void OnUpdateSaveProgress(int save_id,
                            int64 bytes_so_far,
                            bool write_success);
  
  
  void OnSaveFinished(int save_id, int64 bytes_so_far, bool is_success);
  
  
  void OnErrorFinished(const GURL& save_url, int tab_id);
  
  void OnFinishSavePageJob(int render_process_id,
                           int render_view_id,
                           int save_package_id);

  

  
  void OnDeleteDirectoryOrFile(const FilePath& full_path, bool is_dir);

  

  
  void OnSaveURL(const GURL& url,
                 const GURL& referrer,
                 int render_process_host_id,
                 int render_view_id,
                 net::URLRequestContextGetter* request_context_getter);
  
  void OnRequireSaveJobFromOtherSource(SaveFileCreateInfo* info);
  
  
  void ExecuteCancelSaveRequest(int render_process_id, int request_id);

  
  int next_id_;

  
  typedef base::hash_map<int, SaveFile*> SaveFileMap;
  SaveFileMap save_file_map_;

  ResourceDispatcherHost* resource_dispatcher_host_;

  
  
  typedef base::hash_map<int, SavePackage*> SavePackageMap;
  SavePackageMap packages_;

  
  
  
  
  
  
  
  
  
  
  typedef base::hash_map<std::string, SavePackage*> StartingRequestsMap;
  typedef base::hash_map<int, StartingRequestsMap> TabToStartingRequestsMap;
  TabToStartingRequestsMap tab_starting_requests_;

  DISALLOW_COPY_AND_ASSIGN(SaveFileManager);
};

#endif  
