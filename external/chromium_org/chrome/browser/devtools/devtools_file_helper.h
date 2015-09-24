// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEVTOOLS_DEVTOOLS_FILE_HELPER_H_
#define CHROME_BROWSER_DEVTOOLS_DEVTOOLS_FILE_HELPER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"

class Profile;

namespace base {
class FilePath;
}

namespace content {
class WebContents;
}

class DevToolsFileHelper {
 public:
  struct FileSystem {
    FileSystem();
    FileSystem(const std::string& file_system_name,
               const std::string& root_url,
               const std::string& file_system_path);

    std::string file_system_name;
    std::string root_url;
    std::string file_system_path;
  };

  DevToolsFileHelper(content::WebContents* web_contents, Profile* profile);
  ~DevToolsFileHelper();

  typedef base::Callback<void(void)> SaveCallback;
  typedef base::Callback<void(void)> AppendCallback;
  typedef base::Callback<
      void(const std::vector<DevToolsFileHelper::FileSystem>&)>
      RequestFileSystemsCallback;
  typedef base::Callback<void(const DevToolsFileHelper::FileSystem&)>
      AddFileSystemCallback;
  typedef base::Callback<void(const base::string16&,
                              const base::Callback<void(bool)>&)>
      ShowInfoBarCallback;

  
  
  
  void Save(const std::string& url,
            const std::string& content,
            bool save_as,
            const SaveCallback& saveCallback,
            const SaveCallback& cancelCallback);

  
  
  
  
  void Append(const std::string& url,
              const std::string& content,
              const AppendCallback& callback);

  
  
  
  
  
  
  
  
  
  
  void AddFileSystem(const AddFileSystemCallback& callback,
                     const ShowInfoBarCallback& show_info_bar_callback);

  
  
  
  
  
  
  
  
  void UpgradeDraggedFileSystemPermissions(
      const std::string& file_system_url,
      const AddFileSystemCallback& callback,
      const ShowInfoBarCallback& show_info_bar_callback);

  
  
  
  void RequestFileSystems(const RequestFileSystemsCallback& callback);

  
  void RemoveFileSystem(const std::string& file_system_path);

  
  
  bool IsFileSystemAdded(const std::string& file_system_path);

 private:
  void SaveAsFileSelected(const std::string& url,
                          const std::string& content,
                          const SaveCallback& callback,
                          const base::FilePath& path);
  void SaveAsFileSelectionCanceled(const SaveCallback& callback);
  void InnerAddFileSystem(
      const AddFileSystemCallback& callback,
      const ShowInfoBarCallback& show_info_bar_callback,
      const base::FilePath& path);
  void AddUserConfirmedFileSystem(
      const AddFileSystemCallback& callback,
      const base::FilePath& path,
      bool allowed);
  void RestoreValidatedFileSystems(
      const RequestFileSystemsCallback& callback,
      const std::vector<base::FilePath>& file_paths);

  content::WebContents* web_contents_;
  Profile* profile_;
  typedef std::map<std::string, base::FilePath> PathsMap;
  PathsMap saved_files_;
  base::WeakPtrFactory<DevToolsFileHelper> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(DevToolsFileHelper);
};

#endif  
