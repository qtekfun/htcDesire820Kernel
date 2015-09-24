// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_RESOURCE_SCHEDULER_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_SCHEDULER_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"
#include "net/base/priority_queue.h"
#include "net/base/request_priority.h"

namespace net {
class HostPortPair;
class URLRequest;
}

namespace content {
class ResourceThrottle;

class CONTENT_EXPORT ResourceScheduler : public base::NonThreadSafe {
 public:
  ResourceScheduler();
  ~ResourceScheduler();

  
  
  
  scoped_ptr<ResourceThrottle> ScheduleRequest(
      int child_id, int route_id, net::URLRequest* url_request);

  

  
  void OnClientCreated(int child_id, int route_id);

  
  void OnClientDeleted(int child_id, int route_id);

  

  
  void OnNavigate(int child_id, int route_id);

  
  
  void OnWillInsertBody(int child_id, int route_id);

 private:
  class RequestQueue;
  class ScheduledResourceRequest;
  struct Client;

  typedef int64 ClientId;
  typedef std::map<ClientId, Client*> ClientMap;
  typedef std::set<ScheduledResourceRequest*> RequestSet;

  
  void RemoveRequest(ScheduledResourceRequest* request);

  
  void StartRequest(ScheduledResourceRequest* request, Client* client);

  
  
  
  
  
  
  void ReprioritizeRequest(ScheduledResourceRequest* request,
                           net::RequestPriority new_priority);

  
  
  void LoadAnyStartablePendingRequests(Client* client);

  
  
  void GetNumDelayableRequestsInFlight(
      Client* client,
      const net::HostPortPair& active_request_host,
      size_t* total_delayable,
      size_t* total_for_active_host) const;

  enum ShouldStartReqResult {
    DO_NOT_START_REQUEST_AND_STOP_SEARCHING = -2,
    DO_NOT_START_REQUEST_AND_KEEP_SEARCHING = -1,
    START_REQUEST = 1,
  };

  
  
  ShouldStartReqResult ShouldStartRequest(ScheduledResourceRequest* request,
                                          Client* client) const;

  
  ClientId MakeClientId(int child_id, int route_id);

  ClientMap client_map_;
  RequestSet unowned_requests_;
};

}  

#endif  
