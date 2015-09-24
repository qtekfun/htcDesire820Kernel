// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_SOCKET_PERMISSION_ENTRY_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_SOCKET_PERMISSION_ENTRY_H_

#include <string>
#include <vector>

#include "content/public/common/socket_permission_request.h"
#include "ipc/ipc_param_traits.h"

template <class T> struct FuzzTraits;

namespace extensions {

class SocketPermissionEntry {
 public:
  enum HostType {
    ANY_HOST,
    HOSTS_IN_DOMAINS,
    SPECIFIC_HOSTS,
  };

  SocketPermissionEntry();
  ~SocketPermissionEntry();

  
  
  bool operator<(const SocketPermissionEntry& rhs) const;
  bool operator==(const SocketPermissionEntry& rhs) const;

  bool Check(const content::SocketPermissionRequest& request) const;

  
  
  
  
  
  
  
  
  
  
  
  
  static bool ParseHostPattern(
      content::SocketPermissionRequest::OperationType type,
      const std::string& pattern,
      SocketPermissionEntry* entry);

  static bool ParseHostPattern(
      content::SocketPermissionRequest::OperationType type,
      const std::vector<std::string>& pattern_tokens,
      SocketPermissionEntry* entry);

  
  bool IsAddressBoundType() const;

  std::string GetHostPatternAsString() const;
  HostType GetHostType() const;

  const content::SocketPermissionRequest& pattern() const { return pattern_; }
  bool match_subdomains() const { return match_subdomains_; }

 private:
  
  friend struct IPC::ParamTraits<SocketPermissionEntry>;
  friend struct FuzzTraits<SocketPermissionEntry>;

  
  content::SocketPermissionRequest pattern_;

  
  bool match_subdomains_;
};

}  

#endif  
