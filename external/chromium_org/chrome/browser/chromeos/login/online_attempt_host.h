// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ONLINE_ATTEMPT_HOST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ONLINE_ATTEMPT_HOST_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/login/auth_attempt_state_resolver.h"

class Profile;

namespace chromeos {

class AuthAttemptState;
class OnlineAttempt;
struct UserContext;

class OnlineAttemptHost : public AuthAttemptStateResolver {
 public:
  class Delegate {
    public:
     
     virtual void OnChecked(const std::string &username, bool success) = 0;
  };

  explicit OnlineAttemptHost(Delegate *delegate);
  virtual ~OnlineAttemptHost();

  
  
  
  
  void Check(Profile* profile,
             const UserContext& user_context);

  
  void Reset();

  
  
  virtual void Resolve() OVERRIDE;

  
  void ResolveOnUIThread(bool success);

 private:
  Delegate* delegate_;
  std::string current_attempt_hash_;
  std::string current_username_;
  scoped_ptr<OnlineAttempt> online_attempt_;
  scoped_ptr<AuthAttemptState> state_;
  base::WeakPtrFactory<OnlineAttemptHost> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(OnlineAttemptHost);
};

}  

#endif  

