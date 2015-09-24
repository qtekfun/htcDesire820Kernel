// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_RESOURCE_REQUEST_INFO_IMPL_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_REQUEST_INFO_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "content/public/browser/resource_request_info.h"
#include "content/public/common/referrer.h"
#include "net/base/load_states.h"
#include "webkit/common/resource_type.h"

namespace content {
class CrossSiteResourceHandler;
class DetachableResourceHandler;
class ResourceContext;
class ResourceMessageFilter;
struct GlobalRequestID;
struct GlobalRoutingID;

class ResourceRequestInfoImpl : public ResourceRequestInfo,
                                public base::SupportsUserData::Data {
 public:
  
  CONTENT_EXPORT static ResourceRequestInfoImpl* ForRequest(
      net::URLRequest* request);

  
  CONTENT_EXPORT static const ResourceRequestInfoImpl* ForRequest(
      const net::URLRequest* request);

  CONTENT_EXPORT ResourceRequestInfoImpl(
      int process_type,
      int child_id,
      int route_id,
      int origin_pid,
      int request_id,
      int render_frame_id,
      bool is_main_frame,
      int64 frame_id,
      bool parent_is_main_frame,
      int64 parent_frame_id,
      ResourceType::Type resource_type,
      PageTransition transition_type,
      bool should_replace_current_entry,
      bool is_download,
      bool is_stream,
      bool allow_download,
      bool has_user_gesture,
      blink::WebReferrerPolicy referrer_policy,
      ResourceContext* context,
      base::WeakPtr<ResourceMessageFilter> filter,
      bool is_async);
  virtual ~ResourceRequestInfoImpl();

  
  virtual ResourceContext* GetContext() const OVERRIDE;
  virtual int GetChildID() const OVERRIDE;
  virtual int GetRouteID() const OVERRIDE;
  virtual int GetOriginPID() const OVERRIDE;
  virtual int GetRequestID() const OVERRIDE;
  virtual int GetRenderFrameID() const OVERRIDE;
  virtual bool IsMainFrame() const OVERRIDE;
  virtual int64 GetFrameID() const OVERRIDE;
  virtual bool ParentIsMainFrame() const OVERRIDE;
  virtual int64 GetParentFrameID() const OVERRIDE;
  virtual ResourceType::Type GetResourceType() const OVERRIDE;
  virtual blink::WebReferrerPolicy GetReferrerPolicy() const OVERRIDE;
  virtual PageTransition GetPageTransition() const OVERRIDE;
  virtual bool HasUserGesture() const OVERRIDE;
  virtual bool WasIgnoredByHandler() const OVERRIDE;
  virtual bool GetAssociatedRenderView(int* render_process_id,
                                       int* render_view_id) const OVERRIDE;
  virtual bool IsAsync() const OVERRIDE;
  virtual bool IsDownload() const OVERRIDE;


  CONTENT_EXPORT void AssociateWithRequest(net::URLRequest* request);

  CONTENT_EXPORT GlobalRequestID GetGlobalRequestID() const;
  GlobalRoutingID GetGlobalRoutingID() const;

  
  ResourceMessageFilter* filter() const {
    return filter_.get();
  }

  
  
  
  
  void UpdateForTransfer(int child_id,
                         int route_id,
                         int origin_pid,
                         int request_id,
                         int64 frame_id,
                         int64 parent_frame_id,
                         base::WeakPtr<ResourceMessageFilter> filter);

  
  CrossSiteResourceHandler* cross_site_handler() {
    return cross_site_handler_;
  }
  void set_cross_site_handler(CrossSiteResourceHandler* h) {
    cross_site_handler_ = h;
  }

  
  
  
  bool should_replace_current_entry() const {
    return should_replace_current_entry_;
  }

  
  DetachableResourceHandler* detachable_handler() const {
    return detachable_handler_;
  }
  void set_detachable_handler(DetachableResourceHandler* h) {
    detachable_handler_ = h;
  }

  
  int process_type() const { return process_type_; }

  
  bool allow_download() const { return allow_download_; }

  
  void set_is_download(bool download) { is_download_ = download; }

  
  bool is_stream() const { return is_stream_; }
  void set_is_stream(bool stream) { is_stream_ = stream; }

  void set_was_ignored_by_handler(bool value) {
    was_ignored_by_handler_ = value;
  }

  
  
  int memory_cost() const { return memory_cost_; }
  void set_memory_cost(int cost) { memory_cost_ = cost; }

 private:
  FRIEND_TEST_ALL_PREFIXES(ResourceDispatcherHostTest,
                           DeletedFilterDetached);
  FRIEND_TEST_ALL_PREFIXES(ResourceDispatcherHostTest,
                           DeletedFilterDetachedRedirect);
  
  CrossSiteResourceHandler* cross_site_handler_;
  DetachableResourceHandler* detachable_handler_;

  int process_type_;
  int child_id_;
  int route_id_;
  int origin_pid_;
  int request_id_;
  int render_frame_id_;
  bool is_main_frame_;
  int64 frame_id_;
  bool parent_is_main_frame_;
  int64 parent_frame_id_;
  bool should_replace_current_entry_;
  bool is_download_;
  bool is_stream_;
  bool allow_download_;
  bool has_user_gesture_;
  bool was_ignored_by_handler_;
  ResourceType::Type resource_type_;
  PageTransition transition_type_;
  int memory_cost_;
  blink::WebReferrerPolicy referrer_policy_;
  ResourceContext* context_;
  
  
  base::WeakPtr<ResourceMessageFilter> filter_;
  bool is_async_;

  DISALLOW_COPY_AND_ASSIGN(ResourceRequestInfoImpl);
};

}  

#endif  
