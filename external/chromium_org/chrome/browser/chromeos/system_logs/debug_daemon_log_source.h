// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_DEBUG_DAEMON_LOG_SOURCE_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_DEBUG_DAEMON_LOG_SOURCE_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/system_logs/system_logs_fetcher_base.h"

class Profile;

namespace chromeos {

class DebugDaemonLogSource : public SystemLogsSource {
 public:
  explicit DebugDaemonLogSource(bool scrub);
  virtual ~DebugDaemonLogSource();

  
  
  
  
  virtual void Fetch(const SysLogsSourceCallback& callback) OVERRIDE;

 private:
  typedef std::map<std::string, std::string> KeyValueMap;

  
  void OnGetRoutes(bool succeeded, const std::vector<std::string>& routes);
  void OnGetNetworkStatus(bool succeeded, const std::string& status);
  void OnGetModemStatus(bool succeeded, const std::string& status);
  void OnGetWiMaxStatus(bool succeeded, const std::string& status);
  void OnGetLogs(bool succeeded,
                 const KeyValueMap& logs);
  void OnGetUserLogFiles(bool succeeded,
                         const KeyValueMap& logs);

  
  
  static void ReadUserLogFiles(
      const KeyValueMap& user_log_files,
      const std::vector<Profile*>& last_used_profiles,
      SystemLogsResponse* response);

  
  
  void MergeResponse(SystemLogsResponse* response);

  
  void RequestCompleted();

  scoped_ptr<SystemLogsResponse> response_;
  SysLogsSourceCallback callback_;
  int num_pending_requests_;
  bool scrub_;
  base::WeakPtrFactory<DebugDaemonLogSource> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DebugDaemonLogSource);
};


}  

#endif  
