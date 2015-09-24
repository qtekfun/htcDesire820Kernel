// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HELPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HELPER_H_

#include <deque>
#include <map>

#include "base/atomic_sequence_num.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/process/process.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/common/window_container_type.h"
#include "third_party/WebKit/public/web/WebPopupType.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/surface/transport_dib.h"

namespace IPC {
class Message;
}

namespace base {
class TimeDelta;
}

struct ViewHostMsg_CreateWindow_Params;
struct ViewMsg_SwapOut_Params;

namespace content {
class ResourceDispatcherHostImpl;
class SessionStorageNamespace;

class RenderWidgetHelper
    : public base::RefCountedThreadSafe<RenderWidgetHelper,
                                        BrowserThread::DeleteOnIOThread> {
 public:
  RenderWidgetHelper();

  void Init(int render_process_id,
            ResourceDispatcherHostImpl* resource_dispatcher_host);

  
  int GetNextRoutingID();

  

  
  
  
  static RenderWidgetHelper* FromProcessHostID(int render_process_host_id);

  

  
  
  
  void ResumeDeferredNavigation(const GlobalRequestID& request_id);
  bool WaitForBackingStoreMsg(int render_widget_id,
                              const base::TimeDelta& max_delay,
                              IPC::Message* msg);
  
  
  void ResumeRequestsForView(int route_id);

#if defined(OS_POSIX) && !defined(TOOLKIT_GTK) && !defined(OS_ANDROID)
  
  TransportDIB* MapTransportDIB(TransportDIB::Id dib_id);
#endif

  

  
  void DidReceiveBackingStoreMsg(const IPC::Message& msg);

  void CreateNewWindow(
      const ViewHostMsg_CreateWindow_Params& params,
      bool no_javascript_access,
      base::ProcessHandle render_process,
      int* route_id,
      int* main_frame_route_id,
      int* surface_id,
      SessionStorageNamespace* session_storage_namespace);
  void CreateNewWidget(int opener_id,
                       blink::WebPopupType popup_type,
                       int* route_id,
                       int* surface_id);
  void CreateNewFullscreenWidget(int opener_id, int* route_id, int* surface_id);

#if defined(OS_POSIX)
  
  
  
  
  
  void AllocTransportDIB(uint32 size,
                         bool cache_in_browser,
                         TransportDIB::Handle* result);

  
  void FreeTransportDIB(TransportDIB::Id dib_id);
#endif

 private:
  
  
  class BackingStoreMsgProxy;
  friend class BackingStoreMsgProxy;
  friend class base::RefCountedThreadSafe<RenderWidgetHelper>;
  friend struct BrowserThread::DeleteOnThread<BrowserThread::IO>;
  friend class base::DeleteHelper<RenderWidgetHelper>;

  typedef std::deque<BackingStoreMsgProxy*> BackingStoreMsgProxyQueue;
  
  typedef base::hash_map<int, BackingStoreMsgProxyQueue >
      BackingStoreMsgProxyMap;

  ~RenderWidgetHelper();

  
  void OnDiscardBackingStoreMsg(BackingStoreMsgProxy* proxy);

  
  void OnDispatchBackingStoreMsg(BackingStoreMsgProxy* proxy);

  
  void OnCreateWindowOnUI(
      const ViewHostMsg_CreateWindow_Params& params,
      int route_id,
      int main_frame_route_id,
      SessionStorageNamespace* session_storage_namespace);

  
  void OnResumeRequestsForView(int route_id);

  
  void OnCreateWidgetOnUI(int opener_id,
                          int route_id,
                          blink::WebPopupType popup_type);

  
  void OnCreateFullscreenWidgetOnUI(int opener_id, int route_id);

  
  
  void OnResumeDeferredNavigation(const GlobalRequestID& request_id);

#if defined(OS_POSIX)
  
  void ClearAllocatedDIBs();

  
  
  base::Lock allocated_dibs_lock_;
  std::map<TransportDIB::Id, int> allocated_dibs_;
#endif

  
  
  
  
  BackingStoreMsgProxyMap pending_paints_;
  base::Lock pending_paints_lock_;

  int render_process_id_;

  
  base::WaitableEvent event_;

  
  base::AtomicSequenceNumber next_routing_id_;

  ResourceDispatcherHostImpl* resource_dispatcher_host_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHelper);
};

}  

#endif  
