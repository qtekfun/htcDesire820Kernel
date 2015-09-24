// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_SPAWNED_TEST_SERVER_LOCAL_TEST_SERVER_H_
#define NET_TEST_SPAWNED_TEST_SERVER_LOCAL_TEST_SERVER_H_

#include <string>

#include "base/file_util.h"
#include "base/process/process_handle.h"
#include "net/test/spawned_test_server/base_test_server.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif

class CommandLine;

namespace net {

class LocalTestServer : public BaseTestServer {
 public:
  
  
  LocalTestServer(Type type,
                  const std::string& host,
                  const base::FilePath& document_root);

  
  
  LocalTestServer(Type type,
                  const SSLOptions& ssl_options,
                  const base::FilePath& document_root);

  virtual ~LocalTestServer();

  
  bool Start() WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool StartInBackground() WARN_UNUSED_RESULT;

  
  
  bool BlockUntilStarted() WARN_UNUSED_RESULT;

  
  bool Stop();

  
  virtual bool SetPythonPath() const WARN_UNUSED_RESULT;

  
  
  virtual bool GetTestServerPath(base::FilePath* testserver_path) const
      WARN_UNUSED_RESULT;

  
  
  virtual bool AddCommandLineArguments(CommandLine* command_line) const
      WARN_UNUSED_RESULT;

  
  
  base::FilePath GetDocumentRoot() const { return document_root(); };

 private:
  bool Init(const base::FilePath& document_root);

  
  bool LaunchPython(const base::FilePath& testserver_path) WARN_UNUSED_RESULT;

  
  bool WaitToStart() WARN_UNUSED_RESULT;

  
  base::ProcessHandle process_handle_;

#if defined(OS_WIN)
  
  base::win::ScopedHandle job_handle_;

  
  base::win::ScopedHandle child_read_fd_;

  
  base::win::ScopedHandle child_write_fd_;
#endif

#if defined(OS_POSIX)
  
  int child_fd_;
  file_util::ScopedFD child_fd_closer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(LocalTestServer);
};

}  

#endif  
