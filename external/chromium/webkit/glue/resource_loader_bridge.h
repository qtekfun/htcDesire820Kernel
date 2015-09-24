// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_RESOURCE_LOADER_BRIDGE_H_
#define WEBKIT_GLUE_RESOURCE_LOADER_BRIDGE_H_

#include <utility>
#include <vector>

#include "build/build_config.h"
#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#endif
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/time.h"
#include "base/values.h"
#include "googleurl/src/gurl.h"
#include "net/base/host_port_pair.h"
#include "net/url_request/url_request_status.h"
#include "webkit/glue/resource_type.h"

namespace net {
class HttpResponseHeaders;
}

namespace webkit_glue {

struct ResourceLoadTimingInfo {
  ResourceLoadTimingInfo();
  ~ResourceLoadTimingInfo();

  
  
  base::Time base_time;

  
  
  int32 proxy_start;

  
  
  int32 proxy_end;

  
  int32 dns_start;

  
  int32 dns_end;

  
  
  int32 connect_start;

  
  
  int32 connect_end;

  
  
  int32 ssl_start;

  
  int32 ssl_end;

  
  int32 send_start;

  
  int32 send_end;

  
  
  int32 receive_headers_start;

  
  
  int32 receive_headers_end;
};

struct ResourceDevToolsInfo : base::RefCounted<ResourceDevToolsInfo> {
  typedef std::vector<std::pair<std::string, std::string> >
      HeadersVector;

  ResourceDevToolsInfo();
  ~ResourceDevToolsInfo();

  int32 http_status_code;
  std::string http_status_text;
  HeadersVector request_headers;
  HeadersVector response_headers;
};

struct ResourceResponseInfo {
  ResourceResponseInfo();
  ~ResourceResponseInfo();

  
  
  base::Time request_time;

  
  
  base::Time response_time;

  
  scoped_refptr<net::HttpResponseHeaders> headers;

  
  std::string mime_type;

  
  
  std::string charset;

  
  
  std::string security_info;

  
  int64 content_length;

  
  
  int64 encoded_data_length;

  
  int64 appcache_id;

  
  
  GURL appcache_manifest_url;

  
  
  uint32 connection_id;

  
  bool connection_reused;

  
  
  ResourceLoadTimingInfo load_timing;

  
  
  
  scoped_refptr<ResourceDevToolsInfo> devtools_info;

  
  
  
  FilePath download_file_path;

  
  bool was_fetched_via_spdy;

  
  bool was_npn_negotiated;

  
  
  bool was_alternate_protocol_available;

  
  
  
  bool was_fetched_via_proxy;

  
  net::HostPortPair socket_address;
};

class ResourceLoaderBridge {
 public:
  
  struct RequestInfo {
    RequestInfo();
    ~RequestInfo();

    
    std::string method;

    
    GURL url;

    
    
    GURL first_party_for_cookies;

    
    
    GURL referrer;

    std::string frame_origin;
    std::string main_frame_origin;

    
    
    
    
    std::string headers;

    
    int load_flags;

    
    int requestor_pid;

    
    
    ResourceType::Type request_type;

    
    uint32 request_context;

    
    int appcache_host_id;

    
    int routing_id;

    
    
    bool download_to_file;

    
    bool has_user_gesture;
  };

  
  
  struct SyncLoadResponse : ResourceResponseInfo {
    SyncLoadResponse();
    ~SyncLoadResponse();

    
    net::URLRequestStatus status;

    
    
    GURL url;

    
    std::string data;
  };

  
  
  
  
  
  
  
  class Peer {
   public:
    virtual ~Peer() {}

    
    
    virtual void OnUploadProgress(uint64 position, uint64 size) = 0;

    
    
    
    
    
    
    
    virtual bool OnReceivedRedirect(const GURL& new_url,
                                    const ResourceResponseInfo& info,
                                    bool* has_new_first_party_for_cookies,
                                    GURL* new_first_party_for_cookies) = 0;

    
    
    virtual void OnReceivedResponse(const ResourceResponseInfo& info) = 0;

    
    
    
    
    virtual void OnDownloadedData(int len) = 0;

    
    
    
    
    
    virtual void OnReceivedData(const char* data,
                                int data_length,
                                int encoded_data_length) = 0;

    
    
    virtual void OnReceivedCachedMetadata(const char* data, int len) { }

    
    
    virtual void OnCompletedRequest(const net::URLRequestStatus& status,
                                    const std::string& security_info,
                                    const base::Time& completion_time) = 0;
  };

  
  
  virtual ~ResourceLoaderBridge();

  
  
  
  
  static ResourceLoaderBridge* Create(const RequestInfo& request_info);

  
  
  virtual void AppendDataToUpload(const char* data, int data_len) = 0;

  
  
  void AppendFileToUpload(const FilePath& file_path) {
    AppendFileRangeToUpload(file_path, 0, kuint64max, base::Time());
  }

  
  
  virtual void AppendFileRangeToUpload(
      const FilePath& file_path,
      uint64 offset,
      uint64 length,
      const base::Time& expected_modification_time) = 0;

  
  
  virtual void AppendBlobToUpload(const GURL& blob_url) = 0;

  
  
  
  virtual void SetUploadIdentifier(int64 identifier) = 0;

  
  
  virtual bool Start(Peer* peer) = 0;

  
  
  
  
  virtual void Cancel() = 0;

  
  
  virtual void SetDefersLoading(bool value) = 0;

  
  
  
  
  
  
  
  virtual void SyncLoad(SyncLoadResponse* response) = 0;

 protected:
  
  ResourceLoaderBridge();

 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceLoaderBridge);
};

}  

#endif  
