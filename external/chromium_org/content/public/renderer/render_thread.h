// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_THREAD_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_THREAD_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/shared_memory.h"
#include "content/common/content_export.h"
#include "content/public/common/user_metrics_action.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sender.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

class GURL;

namespace base {
class MessageLoop;
class MessageLoopProxy;
}

namespace IPC {
class SyncChannel;
class SyncMessageFilter;
}

namespace v8 {
class Extension;
}

namespace content {

class RenderProcessObserver;
class ResourceDispatcherDelegate;

class CONTENT_EXPORT RenderThread : public IPC::Sender {
 public:
  
  
  static RenderThread* Get();

  RenderThread();
  virtual ~RenderThread();

  virtual base::MessageLoop* GetMessageLoop() = 0;
  virtual IPC::SyncChannel* GetChannel() = 0;
  virtual std::string GetLocale() = 0;
  virtual IPC::SyncMessageFilter* GetSyncMessageFilter() = 0;
  virtual scoped_refptr<base::MessageLoopProxy> GetIOMessageLoopProxy() = 0;

  
  
  virtual void AddRoute(int32 routing_id, IPC::Listener* listener) = 0;
  virtual void RemoveRoute(int32 routing_id) = 0;
  virtual int GenerateRoutingID() = 0;

  
  virtual void AddFilter(IPC::ChannelProxy::MessageFilter* filter) = 0;
  virtual void RemoveFilter(IPC::ChannelProxy::MessageFilter* filter) = 0;

  
  virtual void AddObserver(RenderProcessObserver* observer) = 0;
  virtual void RemoveObserver(RenderProcessObserver* observer) = 0;

  
  virtual void SetResourceDispatcherDelegate(
      ResourceDispatcherDelegate* delegate) = 0;

  
  virtual void WidgetHidden() = 0;
  virtual void WidgetRestored() = 0;

  
  
  virtual void EnsureWebKitInitialized() = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void RecordAction(const UserMetricsAction& action) = 0;

  
  
  
  virtual void RecordComputedAction(const std::string& action) = 0;

  
  
  virtual scoped_ptr<base::SharedMemory> HostAllocateSharedMemoryBuffer(
      size_t buffer_size) = 0;

  
  virtual void RegisterExtension(v8::Extension* extension) = 0;

  
  virtual void ScheduleIdleHandler(int64 initial_delay_ms) = 0;

  
  virtual void IdleHandler() = 0;

  
  virtual int64 GetIdleNotificationDelayInMs() const = 0;
  virtual void SetIdleNotificationDelayInMs(
      int64 idle_notification_delay_in_ms) = 0;

  
  virtual void ToggleWebKitSharedTimer(bool suspend) = 0;

  virtual void UpdateHistograms(int sequence_number) = 0;

  
  virtual int PostTaskToAllWebWorkers(const base::Closure& closure) = 0;

  
  
  
  virtual bool ResolveProxy(const GURL& url, std::string* proxy_list) = 0;

#if defined(OS_WIN)
  
  
  virtual void PreCacheFont(const LOGFONT& log_font) = 0;

  
  virtual void ReleaseCachedFonts() = 0;
#endif
};

}  

#endif  
