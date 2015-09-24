// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PACK_EXTENSION_JOB_H_
#define CHROME_BROWSER_EXTENSIONS_PACK_EXTENSION_JOB_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "chrome/browser/extensions/extension_creator.h"
#include "content/public/browser/browser_thread.h"

namespace extensions {

class PackExtensionJob : public base::RefCountedThreadSafe<PackExtensionJob> {
 public:
  
  class Client {
   public:
    virtual void OnPackSuccess(const base::FilePath& crx_file,
                               const base::FilePath& key_file) = 0;
    virtual void OnPackFailure(const std::string& message,
                               ExtensionCreator::ErrorType error_type) = 0;

   protected:
    virtual ~Client() {}
  };

  PackExtensionJob(Client* client,
                   const base::FilePath& root_directory,
                   const base::FilePath& key_file,
                   int run_flags);

  
  void Start();

  
  
  void ClearClient();

  
  static base::string16 StandardSuccessMessage(const base::FilePath& crx_file,
                                         const base::FilePath& key_file);

  void set_asynchronous(bool async) { asynchronous_ = async; }

 private:
  friend class base::RefCountedThreadSafe<PackExtensionJob>;

  virtual ~PackExtensionJob();

  
  void Run();
  void ReportSuccessOnClientThread();
  void ReportFailureOnClientThread(const std::string& error,
                                   ExtensionCreator::ErrorType error_type);

  content::BrowserThread::ID client_thread_id_;
  Client* client_;
  base::FilePath root_directory_;
  base::FilePath key_file_;
  base::FilePath crx_file_out_;
  base::FilePath key_file_out_;
  bool asynchronous_;
  int run_flags_;  
                   

  DISALLOW_COPY_AND_ASSIGN(PackExtensionJob);
};

}  

#endif  
