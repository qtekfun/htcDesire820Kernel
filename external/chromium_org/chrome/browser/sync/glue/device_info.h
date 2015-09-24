// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DEVICE_INFO_H_
#define CHROME_BROWSER_SYNC_GLUE_DEVICE_INFO_H_

#include <string>

#include "base/basictypes.h"
#include "base/bind.h"
#include "sync/protocol/sync.pb.h"

namespace base {
class DictionaryValue;
}

namespace chrome {
class VersionInfo;
}

namespace browser_sync {

class DeviceInfo {
 public:
  DeviceInfo(const std::string& guid,
             const std::string& client_name,
             const std::string& chrome_version,
             const std::string& sync_user_agent,
             const sync_pb::SyncEnums::DeviceType device_type);
  ~DeviceInfo();

  
  
  
  
  const std::string& guid() const;

  
  const std::string& client_name() const;

  
  const std::string& chrome_version() const;

  
  
  
  const std::string& sync_user_agent() const;

  
  const std::string& public_id() const;

  
  sync_pb::SyncEnums::DeviceType device_type() const;

  
  std::string GetOSString() const;

  
  std::string GetDeviceTypeString() const;

  
  bool Equals(const DeviceInfo& other) const;

  
  
  
  
  void set_public_id(std::string id);

  
  
  base::DictionaryValue* ToValue();

  static sync_pb::SyncEnums::DeviceType GetLocalDeviceType();

  
  
  static void CreateLocalDeviceInfo(
      const std::string& guid,
      base::Callback<void(const DeviceInfo& local_info)> callback);

  
  static void GetClientName(
      base::Callback<void(const std::string& local_info)> callback);

  
  
  
  static std::string MakeUserAgentForSyncApi(
      const chrome::VersionInfo& version_info);

 private:
  static void GetClientNameContinuation(
      base::Callback<void(const std::string& local_info)> callback,
      const std::string& session_name);

  static void CreateLocalDeviceInfoContinuation(
      const std::string& guid,
      base::Callback<void(const DeviceInfo& local_info)> callback,
      const std::string& session_name);

  const std::string guid_;

  const std::string client_name_;

  const std::string chrome_version_;

  const std::string sync_user_agent_;

  const sync_pb::SyncEnums::DeviceType device_type_;

  
  
  
  
  std::string public_id_;

  DISALLOW_COPY_AND_ASSIGN(DeviceInfo);
};

}  

#endif  
