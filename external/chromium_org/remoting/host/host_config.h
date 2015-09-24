// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_CONFIG_H_
#define REMOTING_HOST_HOST_CONFIG_H_

#include <string>

#include "base/basictypes.h"

namespace base {
class DictionaryValue;
}  

namespace remoting {


extern const char kHostEnabledConfigPath[];
extern const char kHostOwnerConfigPath[];
extern const char kXmppLoginConfigPath[];
extern const char kXmppAuthTokenConfigPath[];
extern const char kOAuthRefreshTokenConfigPath[];
extern const char kXmppAuthServiceConfigPath[];
extern const char kHostIdConfigPath[];
extern const char kHostNameConfigPath[];
extern const char kHostSecretHashConfigPath[];
extern const char kPrivateKeyConfigPath[];
extern const char kUsageStatsConsentConfigPath[];

class HostConfig {
 public:
  HostConfig() {}
  virtual ~HostConfig() {}

  virtual bool GetString(const std::string& path,
                         std::string* out_value) const = 0;
  virtual bool GetBoolean(const std::string& path, bool* out_value) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(HostConfig);
};

class MutableHostConfig : public HostConfig {
 public:
  MutableHostConfig() {}

  
  
  virtual void SetString(const std::string& path,
                         const std::string& in_value) = 0;
  virtual void SetBoolean(const std::string& path, bool in_value) = 0;

  
  
  
  virtual bool CopyFrom(const base::DictionaryValue* dictionary) = 0;

  
  virtual bool Save() = 0;

  DISALLOW_COPY_AND_ASSIGN(MutableHostConfig);
};

}  

#endif  
