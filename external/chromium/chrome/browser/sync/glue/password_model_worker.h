// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_PASSWORD_MODEL_WORKER_H_
#define CHROME_BROWSER_SYNC_GLUE_PASSWORD_MODEL_WORKER_H_
#pragma once

#include "chrome/browser/sync/engine/model_safe_worker.h"

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"

class PasswordStore;

namespace base {
class WaitableEvent;
}

namespace browser_sync {

class PasswordModelWorker : public browser_sync::ModelSafeWorker {
 public:
  explicit PasswordModelWorker(PasswordStore* password_store);
  virtual ~PasswordModelWorker();

  
  virtual void DoWorkAndWaitUntilDone(Callback0::Type* work);
  virtual ModelSafeGroup GetModelSafeGroup();
  virtual bool CurrentThreadIsWorkThread();

 private:
  void CallDoWorkAndSignalTask(Callback0::Type* work,
                               base::WaitableEvent* done);

  scoped_refptr<PasswordStore> password_store_;
  DISALLOW_COPY_AND_ASSIGN(PasswordModelWorker);
};

}  

#endif  
