// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RENDER_PROCESS_HOST_H_
#define CONTENT_PUBLIC_BROWSER_RENDER_PROCESS_HOST_H_

#include "base/basictypes.h"
#include "base/id_map.h"
#include "base/process/kill.h"
#include "base/process/process_handle.h"
#include "base/supports_user_data.h"
#include "content/common/content_export.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sender.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/surface/transport_dib.h"

class GURL;
struct ViewMsg_SwapOut_Params;

namespace base {
class TimeDelta;
}

namespace content {
class BrowserContext;
class BrowserMessageFilter;
class RenderProcessHostObserver;
class RenderWidgetHost;
class StoragePartition;

typedef base::Thread* (*RendererMainThreadFactoryFunction)(
    const std::string& id);

class CONTENT_EXPORT RenderProcessHost : public IPC::Sender,
                                         public IPC::Listener,
                                         public base::SupportsUserData {
 public:
  typedef IDMap<RenderProcessHost>::iterator iterator;

  
  struct RendererClosedDetails {
    RendererClosedDetails(base::ProcessHandle handle,
                          base::TerminationStatus status,
                          int exit_code) {
      this->handle = handle;
      this->status = status;
      this->exit_code = exit_code;
    }
    base::ProcessHandle handle;
    base::TerminationStatus status;
    int exit_code;
  };

  

  virtual ~RenderProcessHost() {}

  
  
  
  
  virtual bool Init() = 0;

  
  virtual int GetNextRoutingID() = 0;

  
  
  
  
  virtual void AddRoute(int32 routing_id, IPC::Listener* listener) = 0;
  virtual void RemoveRoute(int32 routing_id) = 0;

  
  
  
  virtual void AddObserver(RenderProcessHostObserver* observer) = 0;
  virtual void RemoveObserver(RenderProcessHostObserver* observer) = 0;

  
  
  
  virtual bool WaitForBackingStoreMsg(int render_widget_id,
                                      const base::TimeDelta& max_delay,
                                      IPC::Message* msg) = 0;

  
  virtual void ReceivedBadMessage() = 0;

  
  
  virtual void WidgetRestored() = 0;
  virtual void WidgetHidden() = 0;
  virtual int VisibleWidgetCount() const = 0;

  
  
  virtual bool IsGuest() const = 0;

  
  
  
  
  
  virtual StoragePartition* GetStoragePartition() const = 0;

  
  
  
  
  virtual bool FastShutdownIfPossible() = 0;

  
  virtual bool FastShutdownStarted() const = 0;

  
  virtual void DumpHandles() = 0;

  
  
  
  
  
  
  
  virtual base::ProcessHandle GetHandle() const = 0;

  

  
  
  
  
  
  virtual TransportDIB* GetTransportDIB(TransportDIB::Id dib_id) = 0;

  
  
  virtual TransportDIB* MapTransportDIB(TransportDIB::Id dib_id) = 0;

  
  virtual content::BrowserContext* GetBrowserContext() const = 0;

  
  
  virtual bool InSameStoragePartition(StoragePartition* partition) const = 0;

  
  
  
  
  
  
  virtual int GetID() const = 0;

  
  
  virtual bool HasConnection() const = 0;

  
  
  virtual void EnableSendQueue() = 0;

  
  virtual IPC::ChannelProxy* GetChannel() = 0;

  
  virtual void AddFilter(BrowserMessageFilter* filter) = 0;

  
  virtual bool FastShutdownForPageCount(size_t count) = 0;

  
  
  
  virtual void SetIgnoreInputEvents(bool ignore_input_events) = 0;
  virtual bool IgnoreInputEvents() const = 0;

  
  virtual void Cleanup() = 0;

  
  
  
  virtual void AddPendingView() = 0;
  virtual void RemovePendingView() = 0;

  
  virtual void SetSuddenTerminationAllowed(bool allowed) = 0;
  
  virtual bool SuddenTerminationAllowed() const = 0;

  
  
  
  
  virtual base::TimeDelta GetChildProcessIdleTime() const = 0;

  
  
  
  virtual void SurfaceUpdated(int32 surface_id) = 0;

  
  
  virtual void ResumeRequestsForView(int route_id) = 0;

  

  
  
  
  
  
  

  static bool run_renderer_in_process();

  
  
  static void SetRunRendererInProcess(bool value);

  
  
  static iterator AllHostsIterator();

  
  
  static RenderProcessHost* FromID(int render_process_id);

  
  
  
  static bool ShouldUseProcessPerSite(content::BrowserContext* browser_context,
                                      const GURL& url);

  
  
  static bool ShouldTryToUseExistingProcessHost(
      content::BrowserContext* browser_context, const GURL& site_url);

  
  
  
  
  
  
  static RenderProcessHost* GetExistingProcessHost(
      content::BrowserContext* browser_context, const GURL& site_url);

  
  
  
  
  
  static void SetMaxRendererProcessCount(size_t count);

  
  
  static size_t GetMaxRendererProcessCount();

  static void RegisterRendererMainThreadFactory(
      RendererMainThreadFactoryFunction create);
};

}  

#endif  
