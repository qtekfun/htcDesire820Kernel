// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_DEBUG_DAEMON_CLIENT_H_
#define CHROMEOS_DBUS_DEBUG_DAEMON_CLIENT_H_

#include "base/callback.h"
#include "base/memory/ref_counted_memory.h"
#include "base/platform_file.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

#include <map>

namespace metrics {
class PerfDataProto;
}

namespace chromeos {

class CHROMEOS_EXPORT DebugDaemonClient : public DBusClient {
 public:
  virtual ~DebugDaemonClient();

  
  
  typedef base::Callback<void(bool succeeded)> GetDebugLogsCallback;

  
  
  virtual void GetDebugLogs(base::PlatformFile file,
                            const GetDebugLogsCallback& callback) = 0;

  
  
  typedef base::Callback<void(bool succeeded)> SetDebugModeCallback;

  
  
  
  virtual void SetDebugMode(const std::string& subsystem,
                            const SetDebugModeCallback& callback) = 0;

  
  typedef base::Callback<void(bool succeeded,
                              const std::vector<std::string>& routes)>
      GetRoutesCallback;
  virtual void GetRoutes(bool numeric, bool ipv6,
                         const GetRoutesCallback& callback) = 0;

  
  typedef base::Callback<void(bool succeeded, const std::string& status)>
      GetNetworkStatusCallback;

  
  virtual void GetNetworkStatus(const GetNetworkStatusCallback& callback) = 0;

  
  typedef base::Callback<void(bool succeeded, const std::string& status)>
      GetModemStatusCallback;

  
  virtual void GetModemStatus(const GetModemStatusCallback& callback) = 0;

  
  typedef base::Callback<void(bool succeeded, const std::string& status)>
      GetWiMaxStatusCallback;

  
  virtual void GetWiMaxStatus(const GetWiMaxStatusCallback& callback) = 0;

  
  
  
  
  typedef base::Callback<void(bool succeeded, const std::string& status)>
      GetNetworkInterfacesCallback;

  
  virtual void GetNetworkInterfaces(
      const GetNetworkInterfacesCallback& callback) = 0;

  
  
  typedef base::Callback<void(const std::vector<uint8>& data)>
      GetPerfDataCallback;

  
  virtual void GetPerfData(uint32_t duration,
                           const GetPerfDataCallback& callback) = 0;

  
  typedef base::Callback<void(bool succeeded,
                              const std::map<std::string, std::string>& logs)>
      GetLogsCallback;

  
  virtual void GetScrubbedLogs(const GetLogsCallback& callback) = 0;

  
  virtual void GetAllLogs(const GetLogsCallback& callback) = 0;

  
  virtual void GetUserLogFiles(const GetLogsCallback& callback) = 0;

  
  virtual void StartSystemTracing() = 0;

  
  
  typedef base::Callback<void(const scoped_refptr<base::RefCountedString>&
      result)> StopSystemTracingCallback;

  
  virtual bool RequestStopSystemTracing(const StopSystemTracingCallback&
      callback) = 0;

  
  static StopSystemTracingCallback EmptyStopSystemTracingCallback();

  
  
  
  
  
  typedef base::Callback<void(bool succeeded, const std::string& status)>
      TestICMPCallback;

  
  
  virtual void TestICMP(const std::string& ip_address,
                        const TestICMPCallback& callback) = 0;

  
  
  virtual void TestICMPWithOptions(
      const std::string& ip_address,
      const std::map<std::string, std::string>& options,
      const TestICMPCallback& callback) = 0;

  
  
  static DebugDaemonClient* Create();

 protected:
  
  DebugDaemonClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(DebugDaemonClient);
};

}  

#endif  
