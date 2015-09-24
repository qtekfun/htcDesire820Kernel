// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_NET_LOG_TEMP_FILE_H_
#define CHROME_BROWSER_NET_NET_LOG_TEMP_FILE_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace net {
class NetLogLogger;
}

class ChromeNetLog;

class NetLogTempFile {
 public:
  
  enum Command {
    DO_START,  
    DO_STOP,   
  };

  virtual ~NetLogTempFile();  

  
  void ProcessCommand(Command command);

  
  
  
  bool GetFilePath(base::FilePath* path);

  
  
  base::DictionaryValue* GetState();

 protected:
  
  
  explicit NetLogTempFile(ChromeNetLog* chrome_net_log);

  
  
  virtual bool GetNetExportLogDirectory(base::FilePath* path);

  
  virtual bool NetExportLogExists();

 private:
  friend class ChromeNetLog;
  friend class NetLogTempFileTest;

  
  FRIEND_TEST_ALL_PREFIXES(NetLogTempFileTest, EnsureInitFailure);
  FRIEND_TEST_ALL_PREFIXES(NetLogTempFileTest, EnsureInitAllowStart);
  FRIEND_TEST_ALL_PREFIXES(NetLogTempFileTest, EnsureInitAllowStartOrSend);
  FRIEND_TEST_ALL_PREFIXES(NetLogTempFileTest, ProcessCommandDoStartAndStop);
  FRIEND_TEST_ALL_PREFIXES(NetLogTempFileTest, DoStartClearsFile);
  FRIEND_TEST_ALL_PREFIXES(NetLogTempFileTest, CheckAddEvent);

  
  
  enum State {
    STATE_UNINITIALIZED,
    STATE_ALLOW_START,       
    STATE_ALLOW_STOP,        
    STATE_ALLOW_START_SEND,  
  };

  
  
  
  
  bool EnsureInit();

  
  
  
  void StartNetLog();

  
  
  void StopNetLog();

  
  
  
  bool GetNetExportLog();

  
  State state() const { return state_; }

  State state_;  

  
  // overwritten by unit tests.
  base::FilePath::StringType log_filename_;

  base::FilePath log_path_;  

  
  
  scoped_ptr<net::NetLogLogger> net_log_logger_;

  
  
  ChromeNetLog* chrome_net_log_;

  DISALLOW_COPY_AND_ASSIGN(NetLogTempFile);
};

#endif  
