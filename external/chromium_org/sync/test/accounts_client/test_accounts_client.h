// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_ACCOUNTS_CLIENT_TEST_ACCOUNTS_CLIENT_H_
#define SYNC_TEST_ACCOUNTS_CLIENT_TEST_ACCOUNTS_CLIENT_H_

#include <string>
#include <vector>

#include "base/message_loop/message_loop.h"
#include "url/gurl.h"

using std::string;
using std::vector;

struct AccountSession {
  AccountSession();
  ~AccountSession();

  string username;
  string account_space;
  string session_id;
  string expiration_time;
};

class TestAccountsClient {
 public:
  
  
  
  TestAccountsClient(const string& server,
                     const string& account_space,
                     const vector<string>& usernames);

  virtual ~TestAccountsClient();

  
  
  
  bool ClaimAccount(AccountSession* session);

  
  
  
  void ReleaseAccount(const AccountSession& session);

  
  virtual bool SendRequest(const GURL& url, string* response);

 private:
  GURL CreateGURLWithPath(const string& path);
  base::MessageLoopForIO io_loop_;
  const string server_;
  const string account_space_;
  vector<string> usernames_;
};

#endif  
