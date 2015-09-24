// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BREAKPAD_TOOLS_CRASH_SERVICE_H_
#define COMPONENTS_BREAKPAD_TOOLS_CRASH_SERVICE_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/synchronization/lock.h"

namespace google_breakpad {

class CrashReportSender;
class CrashGenerationServer;
class ClientInfo;

}

namespace breakpad {

class CrashService {
 public:
  CrashService();
  ~CrashService();

  
  
  
  
  bool Initialize(const base::FilePath& operating_dir,
                  const base::FilePath& dumps_path);

  
  
  
  
  
  
  static const char kMaxReports[];
  
  
  
  
  static const char kNoWindow[];
  
  
  
  
  static const char kReporterTag[];
  
  // Override the directory to which crash dump files will be written.
  static const char kDumpsDir[];
  
  
  
  static const char kPipeName[];

  
  int requests_handled() const {
    return requests_handled_;
  }
  
  int clients_connected() const {
    return clients_connected_;
  }
  
  int clients_terminated() const {
    return clients_terminated_;
  }

  
  
  
  int ProcessingLoop();

 private:
  static void OnClientConnected(void* context,
                                const google_breakpad::ClientInfo* client_info);

  static void OnClientDumpRequest(
      void* context,
      const google_breakpad::ClientInfo* client_info,
      const std::wstring* file_path);

  static void OnClientExited(void* context,
                             const google_breakpad::ClientInfo* client_info);

  
  
  static unsigned long __stdcall AsyncSendDump(void* context);

  
  
  
  PSECURITY_DESCRIPTOR GetSecurityDescriptorForLowIntegrity();

  google_breakpad::CrashGenerationServer* dumper_;
  google_breakpad::CrashReportSender* sender_;

  
  std::wstring reporter_tag_;

  
  int requests_handled_;
  int requests_sent_;
  volatile long clients_connected_;
  volatile long clients_terminated_;
  base::Lock sending_;

  DISALLOW_COPY_AND_ASSIGN(CrashService);
};

}  

#endif  
