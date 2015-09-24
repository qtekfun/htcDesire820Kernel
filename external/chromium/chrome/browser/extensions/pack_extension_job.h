// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PACK_EXTENSION_JOB_H_
#define CHROME_BROWSER_EXTENSIONS_PACK_EXTENSION_JOB_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "content/browser/browser_thread.h"


class PackExtensionJob : public base::RefCountedThreadSafe<PackExtensionJob> {
 public:

  
  class Client {
   public:
    virtual void OnPackSuccess(const FilePath& crx_file,
                               const FilePath& key_file) = 0;
    virtual void OnPackFailure(const std::string& message) = 0;

   protected:
    virtual ~Client() {}
  };

  PackExtensionJob(Client* client,
                   const FilePath& root_directory,
                   const FilePath& key_file);

  
  void Start();

  
  
  void ClearClient();

  
  static string16 StandardSuccessMessage(const FilePath& crx_file,
                                         const FilePath& key_file);

  void set_asynchronous(bool async) { asynchronous_ = async; }

 private:
  friend class base::RefCountedThreadSafe<PackExtensionJob>;

  virtual ~PackExtensionJob();

  
  void Run();
  void ReportSuccessOnClientThread();
  void ReportFailureOnClientThread(const std::string& error);

  BrowserThread::ID client_thread_id_;
  Client* client_;
  FilePath root_directory_;
  FilePath key_file_;
  FilePath crx_file_out_;
  FilePath key_file_out_;
  bool asynchronous_;

  DISALLOW_COPY_AND_ASSIGN(PackExtensionJob);
};

#endif  
