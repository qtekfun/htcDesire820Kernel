// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_LOADER_RESOURCE_DISPATCHER_HOST_IMPL_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_DISPATCHER_HOST_IMPL_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/browser/download/download_resource_handler.h"
#include "content/browser/loader/global_routing_id.h"
#include "content/browser/loader/offline_policy.h"
#include "content/browser/loader/resource_loader.h"
#include "content/browser/loader/resource_loader_delegate.h"
#include "content/browser/loader/resource_scheduler.h"
#include "content/common/content_export.h"
#include "content/public/browser/child_process_data.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_url_parameters.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/resource_dispatcher_host.h"
#include "ipc/ipc_message.h"
#include "net/cookies/canonical_cookie.h"
#include "net/url_request/url_request.h"
#include "webkit/common/resource_type.h"

class ResourceHandler;
struct ResourceHostMsg_Request;

namespace net {
class URLRequestJobFactory;
}

namespace webkit_blob {
class ShareableFileReference;
}

namespace content {
class ResourceContext;
class ResourceDispatcherHostDelegate;
class ResourceMessageDelegate;
class ResourceMessageFilter;
class ResourceRequestInfoImpl;
class SaveFileManager;
class WebContentsImpl;
struct DownloadSaveInfo;
struct Referrer;

class CONTENT_EXPORT ResourceDispatcherHostImpl
    : public ResourceDispatcherHost,
      public ResourceLoaderDelegate {
 public:
  ResourceDispatcherHostImpl();
  virtual ~ResourceDispatcherHostImpl();

  
  
  static ResourceDispatcherHostImpl* Get();

  
  virtual void SetDelegate(ResourceDispatcherHostDelegate* delegate) OVERRIDE;
  virtual void SetAllowCrossOriginAuthPrompt(bool value) OVERRIDE;
  virtual net::Error BeginDownload(
      scoped_ptr<net::URLRequest> request,
      const Referrer& referrer,
      bool is_content_initiated,
      ResourceContext* context,
      int child_id,
      int route_id,
      bool prefer_cache,
      scoped_ptr<DownloadSaveInfo> save_info,
      uint32 download_id,
      const DownloadStartedCallback& started_callback) OVERRIDE;
  virtual void ClearLoginDelegateForRequest(net::URLRequest* request) OVERRIDE;
  virtual void BlockRequestsForRoute(int child_id, int route_id) OVERRIDE;
  virtual void ResumeBlockedRequestsForRoute(
      int child_id, int route_id) OVERRIDE;

  
  
  void Shutdown();

  
  void AddResourceContext(ResourceContext* context);

  
  void RemoveResourceContext(ResourceContext* context);

  
  
  
  void CancelRequestsForContext(ResourceContext* context);

  
  
  bool OnMessageReceived(const IPC::Message& message,
                         ResourceMessageFilter* filter,
                         bool* message_was_ok);

  
  
  void BeginSaveFile(const GURL& url,
                     const Referrer& referrer,
                     int child_id,
                     int route_id,
                     ResourceContext* context);

  
  
  void CancelRequest(int child_id,
                     int request_id,
                     bool from_renderer);

  
  
  void MarkAsTransferredNavigation(const GlobalRequestID& id,
                                   const GURL& target_url);

  
  void ResumeDeferredNavigation(const GlobalRequestID& id);

  
  int pending_requests() const {
    return static_cast<int>(pending_loaders_.size());
  }

  
  void set_max_outstanding_requests_cost_per_process(int limit) {
    max_outstanding_requests_cost_per_process_ = limit;
  }
  void set_max_num_in_flight_requests_per_process(int limit) {
    max_num_in_flight_requests_per_process_ = limit;
  }
  void set_max_num_in_flight_requests(int limit) {
    max_num_in_flight_requests_ = limit;
  }

  
  
  static const int kAvgBytesPerOutstandingRequest = 4400;

  SaveFileManager* save_file_manager() const {
    return save_file_manager_.get();
  }

  
  void OnRenderViewHostCreated(int child_id, int route_id);

  
  void OnRenderViewHostDeleted(int child_id, int route_id);

  
  void CancelRequestsForProcess(int child_id);

  void OnUserGesture(WebContentsImpl* contents);

  
  net::URLRequest* GetURLRequest(const GlobalRequestID& request_id);

  void RemovePendingRequest(int child_id, int request_id);

  
  void CancelBlockedRequestsForRoute(int child_id, int route_id);

  
  
  
  
  void RegisterDownloadedTempFile(
      int child_id, int request_id,
      webkit_blob::ShareableFileReference* reference);
  void UnregisterDownloadedTempFile(int child_id, int request_id);

  
  bool Send(IPC::Message* message);

  
  
  bool allow_cross_origin_auth_prompt();

  ResourceDispatcherHostDelegate* delegate() {
    return delegate_;
  }

  
  
  
  
  scoped_ptr<ResourceHandler> CreateResourceHandlerForDownload(
      net::URLRequest* request,
      bool is_content_initiated,
      bool must_download,
      uint32 id,
      scoped_ptr<DownloadSaveInfo> save_info,
      const DownloadUrlParameters::OnStartedCallback& started_cb);

  
  
  scoped_ptr<ResourceHandler> MaybeInterceptAsStream(
      net::URLRequest* request,
      ResourceResponse* response);

  void ClearSSLClientAuthHandlerForRequest(net::URLRequest* request);

  ResourceScheduler* scheduler() { return scheduler_.get(); }

  
  
  
  
  bool HasSufficientResourcesForRequest(const net::URLRequest* request_);

  
  
  
  void FinishedWithResourcesForRequest(const net::URLRequest* request_);

 private:
  FRIEND_TEST_ALL_PREFIXES(ResourceDispatcherHostTest,
                           TestBlockedRequestsProcessDies);
  FRIEND_TEST_ALL_PREFIXES(ResourceDispatcherHostTest,
                           CalculateApproximateMemoryCost);
  FRIEND_TEST_ALL_PREFIXES(ResourceDispatcherHostTest,
                           DetachableResourceTimesOut);
  FRIEND_TEST_ALL_PREFIXES(ResourceDispatcherHostTest,
                           TestProcessCancelDetachableTimesOut);

  class ShutdownTask;

  struct OustandingRequestsStats {
    int memory_cost;
    int num_requests;
  };

  friend class ShutdownTask;
  friend class ResourceMessageDelegate;

  
  virtual ResourceDispatcherHostLoginDelegate* CreateLoginDelegate(
      ResourceLoader* loader,
      net::AuthChallengeInfo* auth_info) OVERRIDE;
  virtual bool AcceptAuthRequest(
      ResourceLoader* loader,
      net::AuthChallengeInfo* auth_info) OVERRIDE;
  virtual bool AcceptSSLClientCertificateRequest(
      ResourceLoader* loader,
      net::SSLCertRequestInfo* cert_info) OVERRIDE;
  virtual bool HandleExternalProtocol(ResourceLoader* loader,
                                      const GURL& url) OVERRIDE;
  virtual void DidStartRequest(ResourceLoader* loader) OVERRIDE;
  virtual void DidReceiveRedirect(ResourceLoader* loader,
                                  const GURL& new_url) OVERRIDE;
  virtual void DidReceiveResponse(ResourceLoader* loader) OVERRIDE;
  virtual void DidFinishLoading(ResourceLoader* loader) OVERRIDE;

  
  
  
  
  static bool RenderViewForRequest(const net::URLRequest* request,
                                   int* render_process_host_id,
                                   int* render_view_host_id);

  
  void OnInit();

  
  void OnShutdown();

  
  void BeginRequestInternal(scoped_ptr<net::URLRequest> request,
                            scoped_ptr<ResourceHandler> handler);

  void StartLoading(ResourceRequestInfoImpl* info,
                    const linked_ptr<ResourceLoader>& loader);

  
  
  
  
  
  
  

  
  
  OustandingRequestsStats GetOutstandingRequestsStats(
      const ResourceRequestInfoImpl& info);

  
  
  void UpdateOutstandingRequestsStats(const ResourceRequestInfoImpl& info,
                                      const OustandingRequestsStats& stats);

  
  
  OustandingRequestsStats IncrementOutstandingRequestsMemory(
      int count,
      const ResourceRequestInfoImpl& info);

  
  
  
  OustandingRequestsStats IncrementOutstandingRequestsCount(
      int count,
      const ResourceRequestInfoImpl& info);

  
  static int CalculateApproximateMemoryCost(net::URLRequest* request);

  
  
  void CancelRequestsForRoute(int child_id, int route_id);

  
  
  
  
  
  
  
  typedef std::map<GlobalRequestID, linked_ptr<ResourceLoader> > LoaderMap;

  
  
  
  void RemovePendingLoader(const LoaderMap::iterator& iter);

  
  
  void UpdateLoadStates();

  
  void ProcessBlockedRequestsForRoute(int child_id,
                                      int route_id,
                                      bool cancel_requests);

  void OnRequestResource(const IPC::Message& msg,
                         int request_id,
                         const ResourceHostMsg_Request& request_data);
  void OnSyncLoad(int request_id,
                  const ResourceHostMsg_Request& request_data,
                  IPC::Message* sync_result);

  
  
  void UpdateRequestForTransfer(int child_id,
                                int route_id,
                                int request_id,
                                const ResourceHostMsg_Request& request_data,
                                const linked_ptr<ResourceLoader>& loader);

  void BeginRequest(int request_id,
                    const ResourceHostMsg_Request& request_data,
                    IPC::Message* sync_result,  
                    int route_id);  

  
  
  scoped_ptr<ResourceHandler> CreateResourceHandler(
      net::URLRequest* request,
      const ResourceHostMsg_Request& request_data,
      IPC::Message* sync_result,
      int route_id,
      int process_type,
      int child_id,
      ResourceContext* resource_context);

  void OnDataDownloadedACK(int request_id);
  void OnUploadProgressACK(int request_id);
  void OnCancelRequest(int request_id);
  void OnReleaseDownloadedFile(int request_id);

  
  
  ResourceRequestInfoImpl* CreateRequestInfo(
      int child_id,
      int route_id,
      bool download,
      ResourceContext* context);

  
  enum HttpAuthRelationType {
    HTTP_AUTH_RELATION_TOP,            
    HTTP_AUTH_RELATION_SAME_DOMAIN,    
    HTTP_AUTH_RELATION_BLOCKED_CROSS,  
    HTTP_AUTH_RELATION_ALLOWED_CROSS,  
    HTTP_AUTH_RELATION_LAST
  };

  HttpAuthRelationType HttpAuthRelationTypeOf(const GURL& request_url,
                                              const GURL& first_party);

  
  
  
  
  bool IsTransferredNavigation(
      const GlobalRequestID& transferred_request_id) const;

  ResourceLoader* GetLoader(const GlobalRequestID& id) const;
  ResourceLoader* GetLoader(int child_id, int request_id) const;

  
  
  void RegisterResourceMessageDelegate(const GlobalRequestID& id,
                                       ResourceMessageDelegate* delegate);
  void UnregisterResourceMessageDelegate(const GlobalRequestID& id,
                                         ResourceMessageDelegate* delegate);

  int BuildLoadFlagsForRequest(const ResourceHostMsg_Request& request_data,
                               int child_id,
                               bool is_sync_load);

  LoaderMap pending_loaders_;

  
  
  
  typedef std::map<int, scoped_refptr<webkit_blob::ShareableFileReference> >
      DeletableFilesMap;  
  typedef std::map<int, DeletableFilesMap>
      RegisteredTempFiles;  
  RegisteredTempFiles registered_temp_files_;

  
  
  scoped_ptr<base::RepeatingTimer<ResourceDispatcherHostImpl> >
      update_load_states_timer_;

  
  scoped_refptr<SaveFileManager> save_file_manager_;

  
  
  
  
  
  
  
  int request_id_;

  
  bool is_shutdown_;

  typedef std::vector<linked_ptr<ResourceLoader> > BlockedLoadersList;
  typedef std::map<GlobalRoutingID, BlockedLoadersList*> BlockedLoadersMap;
  BlockedLoadersMap blocked_loaders_map_;

  
  
  typedef std::map<int, OustandingRequestsStats> OutstandingRequestsStatsMap;
  OutstandingRequestsStatsMap outstanding_requests_stats_map_;

  
  
  int num_in_flight_requests_;

  
  
  
  
  int max_num_in_flight_requests_;

  
  
  
  
  int max_num_in_flight_requests_per_process_;

  
  
  
  
  
  
  
  int max_outstanding_requests_cost_per_process_;

  
  
  
  base::TimeTicks last_user_gesture_time_;

  
  
  ResourceMessageFilter* filter_;

  ResourceDispatcherHostDelegate* delegate_;

  bool allow_cross_origin_auth_prompt_;

  
  
  std::set<const ResourceContext*> active_resource_contexts_;

  typedef std::map<GlobalRequestID,
                   ObserverList<ResourceMessageDelegate>*> DelegateMap;
  DelegateMap delegate_map_;

  scoped_ptr<ResourceScheduler> scheduler_;

  typedef std::map<GlobalRoutingID, OfflinePolicy*> OfflineMap;

  OfflineMap offline_policy_map_;

  DISALLOW_COPY_AND_ASSIGN(ResourceDispatcherHostImpl);
};

}  

#endif  
