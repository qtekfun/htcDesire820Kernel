// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_SELF_DELETER_HELPER_H_
#define TOOLS_ANDROID_FORWARDER2_SELF_DELETER_HELPER_H_

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"

namespace base {

class SingleThreadTaskRunner;

}  

namespace forwarder2 {

template <typename T>
class SelfDeleterHelper {
 public:
  typedef base::Callback<void (scoped_ptr<T>)> DeletionCallback;

  SelfDeleterHelper(T* self_deleting_object,
                    const DeletionCallback& deletion_callback)
      : weak_ptr_factory_(this),
        construction_runner_(base::MessageLoopProxy::current()),
        self_deleting_object_(self_deleting_object),
        deletion_callback_(deletion_callback) {
  }

  ~SelfDeleterHelper() {
    DCHECK(construction_runner_->RunsTasksOnCurrentThread());
  }

  void MaybeSelfDeleteSoon() {
    DCHECK(!construction_runner_->RunsTasksOnCurrentThread());
    construction_runner_->PostTask(
        FROM_HERE,
        base::Bind(&SelfDeleterHelper::SelfDelete,
                   weak_ptr_factory_.GetWeakPtr()));
  }

 private:
  void SelfDelete() {
    DCHECK(construction_runner_->RunsTasksOnCurrentThread());
    deletion_callback_.Run(make_scoped_ptr(self_deleting_object_));
  }

  base::WeakPtrFactory<SelfDeleterHelper<T> > weak_ptr_factory_;
  const scoped_refptr<base::SingleThreadTaskRunner> construction_runner_;
  T* const self_deleting_object_;
  const DeletionCallback deletion_callback_;

  DISALLOW_COPY_AND_ASSIGN(SelfDeleterHelper);
};

}  

#endif  
