// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_CHILD_RESOURCE_LOADER_BRIDGE_H_
#define WEBKIT_CHILD_RESOURCE_LOADER_BRIDGE_H_

#include <utility>

#include "build/build_config.h"
#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#endif
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "base/values.h"
#include "net/base/request_priority.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "url/gurl.h"
#include "webkit/child/webkit_child_export.h"
#include "webkit/common/resource_response_info.h"
#include "webkit/common/resource_type.h"

namespace webkit_glue {
class ResourceRequestBody;

class ResourceLoaderBridge {
 public:
  
  
  struct WEBKIT_CHILD_EXPORT RequestInfo {
    RequestInfo();
    ~RequestInfo();

    
    std::string method;

    
    GURL url;

    
    
    GURL first_party_for_cookies;

    
    
    GURL referrer;

    
    blink::WebReferrerPolicy referrer_policy;

    
    
    
    
    std::string headers;

    
    int load_flags;

    
    int requestor_pid;

    
    
    ResourceType::Type request_type;

    
    net::RequestPriority priority;

    
    uint32 request_context;

    
    int appcache_host_id;

    
    int routing_id;

    
    
    bool download_to_file;

    
    bool has_user_gesture;

    
    blink::WebURLRequest::ExtraData* extra_data;

   private:
    DISALLOW_COPY_AND_ASSIGN(RequestInfo);
  };

  
  
  struct SyncLoadResponse : ResourceResponseInfo {
    WEBKIT_CHILD_EXPORT SyncLoadResponse();
    WEBKIT_CHILD_EXPORT ~SyncLoadResponse();

    
    int error_code;

    
    
    GURL url;

    
    std::string data;
  };

  
  
  
  
  
  
  
  class WEBKIT_CHILD_EXPORT Peer {
   public:
    
    
    virtual void OnUploadProgress(uint64 position, uint64 size) = 0;

    
    
    
    
    
    
    
    virtual bool OnReceivedRedirect(const GURL& new_url,
                                    const ResourceResponseInfo& info,
                                    bool* has_new_first_party_for_cookies,
                                    GURL* new_first_party_for_cookies) = 0;

    
    
    virtual void OnReceivedResponse(const ResourceResponseInfo& info) = 0;

    
    
    
    
    
    
    
    
    virtual void OnDownloadedData(int len, int encoded_data_length) = 0;

    
    
    
    
    
    
    virtual void OnReceivedData(const char* data,
                                int data_length,
                                int encoded_data_length) = 0;

    
    
    virtual void OnReceivedCachedMetadata(const char* data, int len) { }

    
    
    virtual void OnCompletedRequest(
        int error_code,
        bool was_ignored_by_handler,
        const std::string& security_info,
        const base::TimeTicks& completion_time) = 0;

   protected:
    virtual ~Peer() {}
  };

  
  
  WEBKIT_CHILD_EXPORT virtual ~ResourceLoaderBridge();

  
  
  virtual void SetRequestBody(ResourceRequestBody* request_body) = 0;

  
  
  virtual bool Start(Peer* peer) = 0;

  
  
  
  
  virtual void Cancel() = 0;

  
  
  virtual void SetDefersLoading(bool value) = 0;

  
  
  
  virtual void DidChangePriority(net::RequestPriority new_priority) = 0;

  
  
  
  
  
  
  
  virtual void SyncLoad(SyncLoadResponse* response) = 0;

 protected:
  
  
  
  
  WEBKIT_CHILD_EXPORT ResourceLoaderBridge();

 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceLoaderBridge);
};

}  

#endif  
