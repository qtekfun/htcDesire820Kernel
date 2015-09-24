// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_UTIL_USER_SETTINGS_H_
#define CHROME_BROWSER_SYNC_UTIL_USER_SETTINGS_H_
#pragma once

#include <map>
#include <string>

#include "base/synchronization/lock.h"
#include "build/build_config.h"

extern "C" struct sqlite3;

class FilePath;

namespace browser_sync {

void ExecOrDie(sqlite3* dbhandle, const char *query);
std::string APEncode(const std::string& in);
std::string APDecode(const std::string& in);

class URLFactory;

class UserSettings {
 public:
  
  
  UserSettings();
  ~UserSettings();
  
  bool Init(const FilePath& settings_path);
  void StoreHashedPassword(const std::string& email,
                           const std::string& password);
  bool VerifyAgainstStoredHash(const std::string& email,
                               const std::string& password);

  
  void SwitchUser(const std::string& email);

  
  
  
  void SetAuthTokenForService(const std::string& email,
                              const std::string& service_name,
                              const std::string& long_lived_service_token);
  
  void ClearAllServiceTokens();

  
  bool GetLastUser(std::string* username);

  
  
  bool GetLastUserAndServiceToken(const std::string& service_name,
                                  std::string* username,
                                  std::string* service_token);

  void RemoveAllGuestSettings();

  void StoreEmailForSignin(const std::string& signin,
                           const std::string& primary_email);

  
  
  
  bool GetEmailForSignin(std::string* signin);

  std::string email() const;

  
  
  
  std::string GetClientId();

 protected:
  struct ScopedDBHandle {
    explicit ScopedDBHandle(UserSettings* settings);
    inline sqlite3* get() const { return *handle_; }
    base::AutoLock mutex_lock_;
    sqlite3** const handle_;
  };

  friend struct ScopedDBHandle;
  friend class URLFactory;

  void MigrateOldVersionsAsNeeded(sqlite3* const handle, int current_version);

 private:
  std::string email_;
  mutable base::Lock mutex_;  

  
  sqlite3* dbhandle_;
  base::Lock dbhandle_mutex_;

  
  
  
#ifndef OS_WIN
  typedef std::map<std::string, std::string> ServiceTokenMap;
  ServiceTokenMap service_tokens_;
#endif  

  DISALLOW_COPY_AND_ASSIGN(UserSettings);
};

}  

#endif  
