// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_BROWSER_RENDER_PROCESS_HOST_IMPL_H_
#define CONTENT_BROWSER_RENDERER_HOST_BROWSER_RENDER_PROCESS_HOST_IMPL_H_

#include <map>
#include <queue>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/process/process.h"
#include "base/timer/timer.h"
#include "content/browser/child_process_launcher.h"
#include "content/browser/geolocation/geolocation_dispatcher_host.h"
#include "content/browser/power_monitor_message_broadcaster.h"
#include "content/common/content_export.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/browser/gpu_data_manager_observer.h"
#include "content/public/browser/render_process_host.h"
#include "ipc/ipc_channel_proxy.h"
#include "ui/surface/transport_dib.h"

class CommandLine;
struct ViewHostMsg_CompositorSurfaceBuffersSwapped_Params;

namespace base {
class MessageLoop;
}

namespace gfx {
class Size;
}

namespace content {
class AudioRendererHost;
class BrowserDemuxerAndroid;
class GeolocationDispatcherHost;
class GpuMessageFilter;
class MessagePortMessageFilter;
class PeerConnectionTrackerHost;
class RendererMainThread;
class RenderWidgetHelper;
class RenderWidgetHost;
class RenderWidgetHostImpl;
class RenderWidgetHostViewFrameSubscriber;
class StoragePartition;
class StoragePartitionImpl;

class CONTENT_EXPORT RenderProcessHostImpl
    : public RenderProcessHost,
      public ChildProcessLauncher::Client,
      public GpuDataManagerObserver {
 public:
  RenderProcessHostImpl(BrowserContext* browser_context,
                        StoragePartitionImpl* storage_partition_impl,
                        bool supports_browser_plugin,
                        bool is_guest);
  virtual ~RenderProcessHostImpl();

  
  virtual void EnableSendQueue() OVERRIDE;
  virtual bool Init() OVERRIDE;
  virtual int GetNextRoutingID() OVERRIDE;
  virtual void AddRoute(int32 routing_id, IPC::Listener* listener) OVERRIDE;
  virtual void RemoveRoute(int32 routing_id) OVERRIDE;
  virtual void AddObserver(RenderProcessHostObserver* observer) OVERRIDE;
  virtual void RemoveObserver(RenderProcessHostObserver* observer) OVERRIDE;
  virtual bool WaitForBackingStoreMsg(int render_widget_id,
                                      const base::TimeDelta& max_delay,
                                      IPC::Message* msg) OVERRIDE;
  virtual void ReceivedBadMessage() OVERRIDE;
  virtual void WidgetRestored() OVERRIDE;
  virtual void WidgetHidden() OVERRIDE;
  virtual int VisibleWidgetCount() const OVERRIDE;
  virtual bool IsGuest() const OVERRIDE;
  virtual StoragePartition* GetStoragePartition() const OVERRIDE;
  virtual bool FastShutdownIfPossible() OVERRIDE;
  virtual void DumpHandles() OVERRIDE;
  virtual base::ProcessHandle GetHandle() const OVERRIDE;
  virtual TransportDIB* GetTransportDIB(TransportDIB::Id dib_id) OVERRIDE;
  virtual TransportDIB* MapTransportDIB(TransportDIB::Id dib_id) OVERRIDE;
  virtual BrowserContext* GetBrowserContext() const OVERRIDE;
  virtual bool InSameStoragePartition(
      StoragePartition* partition) const OVERRIDE;
  virtual int GetID() const OVERRIDE;
  virtual bool HasConnection() const OVERRIDE;
  virtual void SetIgnoreInputEvents(bool ignore_input_events) OVERRIDE;
  virtual bool IgnoreInputEvents() const OVERRIDE;
  virtual void Cleanup() OVERRIDE;
  virtual void AddPendingView() OVERRIDE;
  virtual void RemovePendingView() OVERRIDE;
  virtual void SetSuddenTerminationAllowed(bool enabled) OVERRIDE;
  virtual bool SuddenTerminationAllowed() const OVERRIDE;
  virtual IPC::ChannelProxy* GetChannel() OVERRIDE;
  virtual void AddFilter(BrowserMessageFilter* filter) OVERRIDE;
  virtual bool FastShutdownForPageCount(size_t count) OVERRIDE;
  virtual bool FastShutdownStarted() const OVERRIDE;
  virtual base::TimeDelta GetChildProcessIdleTime() const OVERRIDE;
  virtual void SurfaceUpdated(int32 surface_id) OVERRIDE;
  virtual void ResumeRequestsForView(int route_id) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual void OnProcessLaunched() OVERRIDE;

  scoped_refptr<AudioRendererHost> audio_renderer_host() const;

  
  
  void ResumeDeferredNavigation(const GlobalRequestID& request_id);

  
  
  void mark_child_process_activity_time() {
    child_process_activity_time_ = base::TimeTicks::Now();
  }

  
  
  int GetActiveViewCount();

  
  
  void BeginFrameSubscription(
      int route_id,
      scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber);
  void EndFrameSubscription(int route_id);

  scoped_refptr<GeolocationDispatcherHost>
      geolocation_dispatcher_host() const {
    return make_scoped_refptr(geolocation_dispatcher_host_);
  }

  
  
  static void RegisterHost(int host_id, RenderProcessHost* host);
  static void UnregisterHost(int host_id);

  
  
  static bool IsSuitableHost(RenderProcessHost* host,
                             BrowserContext* browser_context,
                             const GURL& site_url);

  
  
  
  
  
  
  static RenderProcessHost* GetProcessHostForSite(
      BrowserContext* browser_context,
      const GURL& url);

  
  
  
  
  
  static void RegisterProcessHostForSite(
      BrowserContext* browser_context,
      RenderProcessHost* process,
      const GURL& url);

  static base::MessageLoop* GetInProcessRendererThreadForTesting();

  
  static void ShutDownInProcessRenderer();

#if defined(OS_ANDROID)
  const scoped_refptr<BrowserDemuxerAndroid>& browser_demuxer_android() {
    return browser_demuxer_android_;
  }
#endif

  MessagePortMessageFilter* message_port_message_filter() const {
    return message_port_message_filter_;
  }

  void SetIsGuestForTesting(bool is_guest) {
    is_guest_ = is_guest;
  }

 protected:
  
  
  scoped_ptr<IPC::ChannelProxy> channel_;

  
  bool fast_shutdown_started_;

  
  bool deleting_soon_;

#ifndef NDEBUG
  
  bool is_self_deleted_;
#endif

  
  
  
  int32 pending_views_;

 private:
  friend class VisitRelayingRenderProcessHost;

  
  void CreateMessageFilters();

  
  void OnShutdownRequest();
  void OnDumpHandlesDone();
  void SuddenTerminationChanged(bool enabled);
  void OnUserMetricsRecordAction(const std::string& action);
  void OnSavedPageAsMHTML(int job_id, int64 mhtml_file_size);

  
  void OnCompositorSurfaceBuffersSwappedNoHost(
      const ViewHostMsg_CompositorSurfaceBuffersSwapped_Params& params);

  
  
  void AppendRendererCommandLine(CommandLine* command_line) const;

  
  
  
  void PropagateBrowserCommandLineToRenderer(const CommandLine& browser_cmd,
                                             CommandLine* renderer_cmd) const;

  
  void SetBackgrounded(bool backgrounded);

  
  void ProcessDied(bool already_dead);

  virtual void OnGpuSwitching() OVERRIDE;

  
  
  IDMap<IPC::Listener> listeners_;

  
  
  
  int32 visible_widgets_;

  
  bool backgrounded_;

  
  
  scoped_refptr<RenderWidgetHelper> widget_helper_;

  
  
  
  
  
  
  GpuMessageFilter* gpu_message_filter_;

  
  scoped_refptr<MessagePortMessageFilter> message_port_message_filter_;

  
  std::map<TransportDIB::Id, TransportDIB*> cached_dibs_;

  enum {
    
    MAX_MAPPED_TRANSPORT_DIBS = 3,
  };

  void ClearTransportDIBCache();
  
  base::DelayTimer<RenderProcessHostImpl> cached_dibs_cleaner_;

  
  scoped_ptr<base::Thread> in_process_renderer_;

  
  
  bool is_initialized_;

  
  scoped_ptr<ChildProcessLauncher> child_process_launcher_;

  
  
  
  
  std::queue<IPC::Message*> queued_messages_;

  
  int id_;

  BrowserContext* browser_context_;

  
  StoragePartitionImpl* storage_partition_impl_;

  
  ObserverList<RenderProcessHostObserver> observers_;

  
  
  
  
  
  
  bool sudden_termination_allowed_;

  
  
  bool ignore_input_events_;

  
  base::TimeTicks child_process_activity_time_;

  
  
  bool supports_browser_plugin_;

  
  
  bool is_guest_;

  
  
  scoped_refptr<PeerConnectionTrackerHost> peer_connection_tracker_host_;

  
  
  bool gpu_observer_registered_;

  
  PowerMonitorMessageBroadcaster power_monitor_broadcaster_;

  scoped_refptr<AudioRendererHost> audio_renderer_host_;

#if defined(OS_ANDROID)
  scoped_refptr<BrowserDemuxerAndroid> browser_demuxer_android_;
#endif

  
  GeolocationDispatcherHost* geolocation_dispatcher_host_;

  DISALLOW_COPY_AND_ASSIGN(RenderProcessHostImpl);
};

}  

#endif  
