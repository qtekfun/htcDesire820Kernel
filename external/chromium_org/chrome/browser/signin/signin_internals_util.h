// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_INTERNALS_UTIL_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_INTERNALS_UTIL_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/values.h"

namespace signin_internals_util {

extern const char kSigninPrefPrefix[];
extern const char kTokenPrefPrefix[];

const size_t kTruncateTokenStringLength = 6;

enum {
  SIGNIN_FIELDS_BEGIN = 0,
  UNTIMED_FIELDS_BEGIN = SIGNIN_FIELDS_BEGIN
};

enum UntimedSigninStatusField {
  USERNAME = UNTIMED_FIELDS_BEGIN,
  SID,
  LSID,
  UNTIMED_FIELDS_END
};

enum {
  UNTIMED_FIELDS_COUNT = UNTIMED_FIELDS_END - UNTIMED_FIELDS_BEGIN,
  TIMED_FIELDS_BEGIN = UNTIMED_FIELDS_END
};

enum TimedSigninStatusField {
  SIGNIN_TYPE = TIMED_FIELDS_BEGIN,
  CLIENT_LOGIN_STATUS,
  OAUTH_LOGIN_STATUS,
  GET_USER_INFO_STATUS,
  UBER_TOKEN_STATUS,
  MERGE_SESSION_STATUS,
  TIMED_FIELDS_END
};

enum {
  TIMED_FIELDS_COUNT = TIMED_FIELDS_END - TIMED_FIELDS_BEGIN,
  SIGNIN_FIELDS_END = TIMED_FIELDS_END,
  SIGNIN_FIELDS_COUNT = SIGNIN_FIELDS_END - SIGNIN_FIELDS_BEGIN
};

struct TokenInfo {
  std::string truncated_token;  
  std::string status;  
  std::string time;  
  int64 time_internal;  
  std::string service;  

  TokenInfo(const std::string& truncated_token,
            const std::string& status,
            const std::string& time,
            const int64& time_internal,
            const std::string& service);
  TokenInfo();
  ~TokenInfo();

  DictionaryValue* ToValue();
};

typedef std::map<std::string, TokenInfo> TokenInfoMap;

std::string TokenPrefPath(const std::string& service_name);

typedef std::pair<std::string, std::string> TimedSigninStatusValue;

std::string SigninStatusFieldToString(UntimedSigninStatusField field);
std::string SigninStatusFieldToString(TimedSigninStatusField field);

struct SigninStatus {
  std::vector<std::string> untimed_signin_fields;
  std::vector<TimedSigninStatusValue> timed_signin_fields;
  TokenInfoMap token_info_map;

  SigninStatus();
  ~SigninStatus();

  
  
  
  
  
  
  
  
  
  
  
  
  scoped_ptr<DictionaryValue> ToValue();
};

class SigninDiagnosticsObserver {
 public:
  
  virtual void NotifySigninValueChanged(const UntimedSigninStatusField& field,
                                        const std::string& value) {}
  virtual void NotifySigninValueChanged(const TimedSigninStatusField& field,
                                        const std::string& value) {}
  
  virtual void NotifyTokenReceivedSuccess(const std::string& token_name,
                                          const std::string& token,
                                          bool update_time) {}
  virtual void NotifyTokenReceivedFailure(const std::string& token_name,
                                          const std::string& error) {}
  virtual void NotifyClearStoredToken(const std::string& token_name) {}};

std::string GetTruncatedHash(const std::string& str);

} 

#endif  
