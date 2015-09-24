// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_QUOTA_DISPATCHER_H_
#define CONTENT_CHILD_QUOTA_DISPATCHER_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/id_map.h"
#include "base/memory/ref_counted.h"
#include "webkit/child/worker_task_runner.h"
#include "webkit/common/quota/quota_types.h"

class GURL;

namespace IPC {
class Message;
}

namespace blink {
class WebStorageQuotaCallbacks;
}

namespace content {

class ThreadSafeSender;
class QuotaMessageFilter;

class QuotaDispatcher : public webkit_glue::WorkerTaskRunner::Observer {
 public:
  class Callback {
   public:
    virtual ~Callback() {}
    virtual void DidQueryStorageUsageAndQuota(int64 usage, int64 quota) = 0;
    virtual void DidGrantStorageQuota(int64 granted_quota) = 0;
    virtual void DidFail(quota::QuotaStatusCode status) = 0;
  };

  QuotaDispatcher(ThreadSafeSender* thread_safe_sender,
                  QuotaMessageFilter* quota_message_filter);
  virtual ~QuotaDispatcher();

  
  
  static QuotaDispatcher* ThreadSpecificInstance(
      ThreadSafeSender* thread_safe_sender,
      QuotaMessageFilter* quota_message_filter);

  
  virtual void OnWorkerRunLoopStopped() OVERRIDE;

  void OnMessageReceived(const IPC::Message& msg);

  void QueryStorageUsageAndQuota(const GURL& gurl,
                                 quota::StorageType type,
                                 Callback* callback);
  void RequestStorageQuota(int render_view_id,
                           const GURL& gurl,
                           quota::StorageType type,
                           int64 requested_size,
                           Callback* callback);

  
  static Callback* CreateWebStorageQuotaCallbacksWrapper(
      blink::WebStorageQuotaCallbacks* callbacks);

 private:
  
  void DidQueryStorageUsageAndQuota(int request_id,
                                    int64 current_usage,
                                    int64 current_quota);
  void DidGrantStorageQuota(int request_id,
                            int64 granted_quota);
  void DidFail(int request_id,
               quota::QuotaStatusCode error);

  IDMap<Callback, IDMapOwnPointer> pending_quota_callbacks_;

  scoped_refptr<ThreadSafeSender> thread_safe_sender_;
  scoped_refptr<QuotaMessageFilter> quota_message_filter_;

  DISALLOW_COPY_AND_ASSIGN(QuotaDispatcher);
};

}  

#endif  
