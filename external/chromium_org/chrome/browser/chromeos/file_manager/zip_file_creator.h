// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_ZIP_FILE_CREATOR_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_ZIP_FILE_CREATOR_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/utility_process_host_client.h"
#include "extensions/common/extension.h"

namespace file_manager {

class ZipFileCreator : public content::UtilityProcessHostClient {
 public:
  class Observer {
   public:
    virtual void OnZipDone(bool success) = 0;

   protected:
    virtual ~Observer() {}
  };

  
  ZipFileCreator(Observer* observer,
                 const base::FilePath& src_dir,
                 const std::vector<base::FilePath>& src_relative_paths,
                 const base::FilePath& dest_file);

  
  void Start();

 private:
  friend class ProcessHostClient;

  virtual ~ZipFileCreator();

  
  void OpenFileHandleOnBlockingThreadPool();

  
  void StartProcessOnIOThread(base::PlatformFile dest_file);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;

  
  void OnCreateZipFileSucceeded();
  void OnCreateZipFileFailed();

  void ReportDone(bool success);

  
  content::BrowserThread::ID thread_identifier_;

  
  Observer* observer_;

  
  base::FilePath src_dir_;

  
  
  std::vector<base::FilePath> src_relative_paths_;

  
  base::FilePath dest_file_;

  
  bool got_response_;
};

}  

#endif  
