// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_SERVICE_WORKER_SERVICE_WORKER_DISPATCHER_H_
#define CONTENT_CHILD_SERVICE_WORKER_SERVICE_WORKER_DISPATCHER_H_

#include "base/id_map.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "third_party/WebKit/public/platform/WebServiceWorkerError.h"
#include "third_party/WebKit/public/platform/WebServiceWorkerProvider.h"
#include "webkit/child/worker_task_runner.h"

class GURL;

namespace blink {
class WebURL;
}

namespace IPC {
class Message;
}

namespace content {
class ServiceWorkerMessageFilter;
class ThreadSafeSender;
class WebServiceWorkerImpl;

class ServiceWorkerDispatcher : public webkit_glue::WorkerTaskRunner::Observer {
 public:
  explicit ServiceWorkerDispatcher(ThreadSafeSender* thread_safe_sender);
  virtual ~ServiceWorkerDispatcher();

  void OnMessageReceived(const IPC::Message& msg);
  bool Send(IPC::Message* msg);

  
  void RegisterServiceWorker(
      const GURL& pattern,
      const GURL& script_url,
      blink::WebServiceWorkerProvider::WebServiceWorkerCallbacks* callbacks);
  
  void UnregisterServiceWorker(
      const GURL& pattern,
      blink::WebServiceWorkerProvider::WebServiceWorkerCallbacks* callbacks);

  
  
  static ServiceWorkerDispatcher* ThreadSpecificInstance(
      ThreadSafeSender* thread_safe_sender);

 private:
  
  virtual void OnWorkerRunLoopStopped() OVERRIDE;

  
  void OnRegistered(int32 thread_id, int32 request_id, int64 registration_id);
  
  void OnUnregistered(int32 thread_id,
                      int32 request_id);
  void OnRegistrationError(int32 thread_id,
                           int32 request_id,
                           blink::WebServiceWorkerError::ErrorType error_type,
                           const base::string16& message);

  IDMap<blink::WebServiceWorkerProvider::WebServiceWorkerCallbacks,
        IDMapOwnPointer> pending_callbacks_;

  scoped_refptr<ThreadSafeSender> thread_safe_sender_;

  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerDispatcher);
};

}  

#endif  
