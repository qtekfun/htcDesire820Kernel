// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_TPM_PASSWORD_FETCHER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_TPM_PASSWORD_FETCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/dbus_method_call_status.h"

namespace chromeos {

class TpmPasswordFetcherDelegate {
 public:
  virtual ~TpmPasswordFetcherDelegate() {}
  virtual void OnPasswordFetched(const std::string& tpm_password) = 0;
};

class TpmPasswordFetcher {
 public:
  
  
  explicit TpmPasswordFetcher(TpmPasswordFetcherDelegate* delegate);
  ~TpmPasswordFetcher();

  
  
  void Fetch();

 private:
  
  void OnTpmIsReady(DBusMethodCallStatus call_status, bool tpm_is_ready);

  
  void OnTpmGetPassword(DBusMethodCallStatus call_status,
                        const std::string& password);

  
  void RescheduleFetch();

  base::WeakPtrFactory<TpmPasswordFetcher> weak_factory_;
  TpmPasswordFetcherDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TpmPasswordFetcher);
};

}  

#endif  
