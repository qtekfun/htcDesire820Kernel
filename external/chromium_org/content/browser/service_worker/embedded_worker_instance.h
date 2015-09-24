// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SERVICE_WORKER_EMBEDDED_WORKER_INSTANCE_H_
#define CONTENT_BROWSER_SERVICE_WORKER_EMBEDDED_WORKER_INSTANCE_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

class GURL;

namespace content {

class EmbeddedWorkerRegistry;

class CONTENT_EXPORT EmbeddedWorkerInstance {
 public:
  enum Status {
    STOPPED,
    STARTING,
    RUNNING,
    STOPPING,
  };

  ~EmbeddedWorkerInstance();

  
  
  
  
  bool Start(int64 service_worker_version_id,
             const GURL& script_url);

  
  
  
  
  bool Stop();

  
  
  void AddProcessReference(int process_id);
  void ReleaseProcessReference(int process_id);

  int embedded_worker_id() const { return embedded_worker_id_; }
  Status status() const { return status_; }
  int process_id() const { return process_id_; }
  int thread_id() const { return thread_id_; }

 private:
  friend class EmbeddedWorkerRegistry;
  FRIEND_TEST_ALL_PREFIXES(EmbeddedWorkerInstanceTest, StartAndStop);

  typedef std::map<int, int> ProcessRefMap;

  
  
  EmbeddedWorkerInstance(EmbeddedWorkerRegistry* registry,
                         int embedded_worker_id);

  
  
  
  
  void OnStarted(int thread_id);

  
  
  
  
  void OnStopped();

  
  
  bool ChooseProcess();

  scoped_refptr<EmbeddedWorkerRegistry> registry_;
  const int embedded_worker_id_;
  Status status_;

  
  int process_id_;
  int thread_id_;

  ProcessRefMap process_refs_;

  DISALLOW_COPY_AND_ASSIGN(EmbeddedWorkerInstance);
};

}  

#endif  
