// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_REFCOUNTED_BROWSER_CONTEXT_KEYED_SERVICE_H_
#define COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_REFCOUNTED_BROWSER_CONTEXT_KEYED_SERVICE_H_

#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner_helpers.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_export.h"
#include "content/public/browser/browser_thread.h"

class RefcountedBrowserContextKeyedService;

namespace impl {

struct BROWSER_CONTEXT_KEYED_SERVICE_EXPORT
    RefcountedBrowserContextKeyedServiceTraits {
  static void Destruct(const RefcountedBrowserContextKeyedService* obj);
};

}  

class BROWSER_CONTEXT_KEYED_SERVICE_EXPORT RefcountedBrowserContextKeyedService
    : public base::RefCountedThreadSafe<
          RefcountedBrowserContextKeyedService,
          impl::RefcountedBrowserContextKeyedServiceTraits> {
 public:
  
  
  
  
  
  virtual void ShutdownOnUIThread() = 0;

 protected:
  
  
  RefcountedBrowserContextKeyedService();

  
  
  
  explicit RefcountedBrowserContextKeyedService(
      const content::BrowserThread::ID thread_id);

  
  
  virtual ~RefcountedBrowserContextKeyedService();

 private:
  friend struct impl::RefcountedBrowserContextKeyedServiceTraits;
  friend class base::DeleteHelper<RefcountedBrowserContextKeyedService>;
  friend class base::RefCountedThreadSafe<RefcountedBrowserContextKeyedService,
      impl::RefcountedBrowserContextKeyedServiceTraits>;

  
  bool requires_destruction_on_thread_;
  content::BrowserThread::ID thread_id_;
};

#endif  
