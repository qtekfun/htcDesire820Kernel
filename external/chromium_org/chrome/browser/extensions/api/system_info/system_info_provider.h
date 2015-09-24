// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_SYSTEM_INFO_PROVIDER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_SYSTEM_INFO_PROVIDER_H_

#include <queue>

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/threading/sequenced_worker_pool.h"
#include "content/public/browser/browser_thread.h"

namespace extensions {

class SystemInfoProvider
    : public base::RefCountedThreadSafe<SystemInfoProvider> {
 public:
  
  
  
  typedef base::Callback<void(bool)> QueryInfoCompletionCallback;
  typedef std::queue<QueryInfoCompletionCallback> CallbackQueue;

  SystemInfoProvider();

  
  
  virtual void PrepareQueryOnUIThread();

  
  
  
  
  
  
  
  
  virtual void InitializeProvider(const base::Closure& do_query_info_callback);

  
  
  
  
  
  
  void StartQueryInfo(const QueryInfoCompletionCallback& callback);

 protected:
  virtual ~SystemInfoProvider();

 private:
  friend class base::RefCountedThreadSafe<SystemInfoProvider>;

  
  
  
  virtual bool QueryInfo() = 0;

  
  
  void OnQueryCompleted(bool success);

  void StartQueryInfoPostInitialization();

  
  
  CallbackQueue callbacks_;

  
  bool is_waiting_for_completion_;

  
  
  scoped_refptr<base::SequencedTaskRunner> worker_pool_;

  DISALLOW_COPY_AND_ASSIGN(SystemInfoProvider);
};

}  

#endif  
