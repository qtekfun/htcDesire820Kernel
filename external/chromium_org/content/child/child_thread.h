// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_CHILD_THREAD_H_
#define CONTENT_CHILD_CHILD_THREAD_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/power_monitor/power_monitor.h"
#include "base/tracked_objects.h"
#include "content/common/content_export.h"
#include "content/common/message_router.h"
#include "ipc/ipc_message.h"  
#include "webkit/child/resource_loader_bridge.h"

namespace base {
class MessageLoop;

namespace debug {
class TraceMemoryController;
}  
}  

namespace IPC {
class SyncChannel;
class SyncMessageFilter;
}  

namespace blink {
class WebFrame;
}  

namespace content {
class ChildHistogramMessageFilter;
class ChildResourceMessageFilter;
class FileSystemDispatcher;
class ServiceWorkerDispatcher;
class ServiceWorkerMessageFilter;
class QuotaDispatcher;
class QuotaMessageFilter;
class ResourceDispatcher;
class SocketStreamDispatcher;
class ThreadSafeSender;
class WebSocketDispatcher;

class CONTENT_EXPORT ChildThread : public IPC::Listener, public IPC::Sender {
 public:
  
  ChildThread();
  
  explicit ChildThread(const std::string& channel_name);
  
  
  
  
  
  virtual ~ChildThread();
  virtual void Shutdown();

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  void AddRoute(int32 routing_id, IPC::Listener* listener);
  void RemoveRoute(int32 routing_id);

  IPC::SyncChannel* channel() { return channel_.get(); }

  
  
  virtual webkit_glue::ResourceLoaderBridge* CreateBridge(
      const webkit_glue::ResourceLoaderBridge::RequestInfo& request_info);

  
  
  
  
  base::SharedMemory* AllocateSharedMemory(size_t buf_size);

  
  
  static base::SharedMemory* AllocateSharedMemory(size_t buf_size,
                                                  IPC::Sender* sender);

  ResourceDispatcher* resource_dispatcher() const {
    return resource_dispatcher_.get();
  }

  SocketStreamDispatcher* socket_stream_dispatcher() const {
    return socket_stream_dispatcher_.get();
  }

  WebSocketDispatcher* websocket_dispatcher() const {
    return websocket_dispatcher_.get();
  }

  FileSystemDispatcher* file_system_dispatcher() const {
    return file_system_dispatcher_.get();
  }

  ServiceWorkerDispatcher* service_worker_dispatcher() const {
    return service_worker_dispatcher_.get();
  }

  QuotaDispatcher* quota_dispatcher() const {
    return quota_dispatcher_.get();
  }

  IPC::SyncMessageFilter* sync_message_filter() const {
    return sync_message_filter_.get();
  }

  
  
  
  ThreadSafeSender* thread_safe_sender() const {
    return thread_safe_sender_.get();
  }

  ChildHistogramMessageFilter* child_histogram_message_filter() const {
    return histogram_message_filter_.get();
  }

  ServiceWorkerMessageFilter* service_worker_message_filter() const {
    return service_worker_message_filter_.get();
  }

  QuotaMessageFilter* quota_message_filter() const {
    return quota_message_filter_.get();
  }

  base::MessageLoop* message_loop() const { return message_loop_; }

  
  static ChildThread* current();

#if defined(OS_ANDROID)
  
  
  static void ShutdownThread();
#endif

 protected:
  friend class ChildProcess;

  
  void OnProcessFinalRelease();

  virtual bool OnControlMessageReceived(const IPC::Message& msg);

  void set_on_channel_error_called(bool on_channel_error_called) {
    on_channel_error_called_ = on_channel_error_called;
  }

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

 private:
  void Init();

  
  void OnShutdown();
  void OnSetProfilerStatus(tracked_objects::ThreadData::Status status);
  void OnGetChildProfilerData(int sequence_number);
  void OnDumpHandles();
#ifdef IPC_MESSAGE_LOG_ENABLED
  void OnSetIPCLoggingEnabled(bool enable);
#endif
#if defined(USE_TCMALLOC)
  void OnGetTcmallocStats();
#endif

  void EnsureConnected();

  std::string channel_name_;
  scoped_ptr<IPC::SyncChannel> channel_;

  
  scoped_refptr<IPC::SyncMessageFilter> sync_message_filter_;

  scoped_refptr<ThreadSafeSender> thread_safe_sender_;

  
  MessageRouter router_;

  
  scoped_ptr<ResourceDispatcher> resource_dispatcher_;

  
  scoped_ptr<SocketStreamDispatcher> socket_stream_dispatcher_;

  scoped_ptr<WebSocketDispatcher> websocket_dispatcher_;

  
  
  bool on_channel_error_called_;

  base::MessageLoop* message_loop_;

  scoped_ptr<FileSystemDispatcher> file_system_dispatcher_;

  scoped_ptr<ServiceWorkerDispatcher> service_worker_dispatcher_;

  scoped_ptr<QuotaDispatcher> quota_dispatcher_;

  scoped_refptr<ChildHistogramMessageFilter> histogram_message_filter_;

  scoped_refptr<ChildResourceMessageFilter> resource_message_filter_;

  scoped_refptr<ServiceWorkerMessageFilter> service_worker_message_filter_;

  scoped_refptr<QuotaMessageFilter> quota_message_filter_;

  base::WeakPtrFactory<ChildThread> channel_connected_factory_;

  
  
  scoped_ptr<base::debug::TraceMemoryController> trace_memory_controller_;

  scoped_ptr<base::PowerMonitor> power_monitor_;

  bool in_browser_process_;

  DISALLOW_COPY_AND_ASSIGN(ChildThread);
};

}  

#endif  
