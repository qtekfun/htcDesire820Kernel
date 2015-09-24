// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OWNERSHIP_STATUS_CHECKER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OWNERSHIP_STATUS_CHECKER_H_
#pragma once

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop_proxy.h"
#include "chrome/browser/chromeos/login/ownership_service.h"

namespace chromeos {

class OwnershipStatusChecker {
 public:
  
  
  typedef Callback1<OwnershipService::Status>::Type Callback;

  explicit OwnershipStatusChecker(Callback* callback);
  ~OwnershipStatusChecker();

 private:
  
  class Core : public base::RefCountedThreadSafe<Core> {
   public:
    explicit Core(Callback* callback);

    
    void Check();

    
    void Cancel();

   private:
    void CheckOnFileThread();
    void ReportResult(OwnershipService::Status status);

    scoped_ptr<Callback> callback_;
    scoped_refptr<base::MessageLoopProxy> origin_loop_;

    DISALLOW_COPY_AND_ASSIGN(Core);
  };

  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(OwnershipStatusChecker);
};

}  

#endif  
