// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "content/common/content_param_traits_macros.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/resource_response.h"
#include "ipc/ipc_message_macros.h"
#include "net/base/request_priority.h"
#include "net/http/http_response_info.h"
#include "webkit/common/resource_request_body.h"

#ifndef CONTENT_COMMON_RESOURCE_MESSAGES_H_
#define CONTENT_COMMON_RESOURCE_MESSAGES_H_

namespace net {
struct LoadTimingInfo;
}

namespace webkit_glue {
struct ResourceDevToolsInfo;
}

namespace IPC {

template <>
struct ParamTraits<scoped_refptr<net::HttpResponseHeaders> > {
  typedef scoped_refptr<net::HttpResponseHeaders> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct CONTENT_EXPORT ParamTraits<webkit_common::DataElement> {
  typedef webkit_common::DataElement param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<scoped_refptr<webkit_glue::ResourceDevToolsInfo> > {
  typedef scoped_refptr<webkit_glue::ResourceDevToolsInfo> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<net::LoadTimingInfo> {
  typedef net::LoadTimingInfo param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<scoped_refptr<webkit_glue::ResourceRequestBody> > {
  typedef scoped_refptr<webkit_glue::ResourceRequestBody> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

}  

#endif  


#define IPC_MESSAGE_START ResourceMsgStart
#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

IPC_ENUM_TRAITS_MAX_VALUE( \
    net::HttpResponseInfo::ConnectionInfo, \
    net::HttpResponseInfo::NUM_OF_CONNECTION_INFOS - 1)

IPC_STRUCT_TRAITS_BEGIN(content::ResourceResponseHead)
  IPC_STRUCT_TRAITS_PARENT(webkit_glue::ResourceResponseInfo)
  IPC_STRUCT_TRAITS_MEMBER(error_code)
  IPC_STRUCT_TRAITS_MEMBER(request_start)
  IPC_STRUCT_TRAITS_MEMBER(response_start)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::SyncLoadResult)
  IPC_STRUCT_TRAITS_PARENT(content::ResourceResponseHead)
  IPC_STRUCT_TRAITS_MEMBER(final_url)
  IPC_STRUCT_TRAITS_MEMBER(data)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(webkit_glue::ResourceResponseInfo)
  IPC_STRUCT_TRAITS_MEMBER(request_time)
  IPC_STRUCT_TRAITS_MEMBER(response_time)
  IPC_STRUCT_TRAITS_MEMBER(headers)
  IPC_STRUCT_TRAITS_MEMBER(mime_type)
  IPC_STRUCT_TRAITS_MEMBER(charset)
  IPC_STRUCT_TRAITS_MEMBER(security_info)
  IPC_STRUCT_TRAITS_MEMBER(content_length)
  IPC_STRUCT_TRAITS_MEMBER(encoded_data_length)
  IPC_STRUCT_TRAITS_MEMBER(appcache_id)
  IPC_STRUCT_TRAITS_MEMBER(appcache_manifest_url)
  IPC_STRUCT_TRAITS_MEMBER(load_timing)
  IPC_STRUCT_TRAITS_MEMBER(devtools_info)
  IPC_STRUCT_TRAITS_MEMBER(download_file_path)
  IPC_STRUCT_TRAITS_MEMBER(was_fetched_via_spdy)
  IPC_STRUCT_TRAITS_MEMBER(was_npn_negotiated)
  IPC_STRUCT_TRAITS_MEMBER(was_alternate_protocol_available)
  IPC_STRUCT_TRAITS_MEMBER(connection_info)
  IPC_STRUCT_TRAITS_MEMBER(was_fetched_via_proxy)
  IPC_STRUCT_TRAITS_MEMBER(npn_negotiated_protocol)
  IPC_STRUCT_TRAITS_MEMBER(socket_address)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(ResourceHostMsg_Request)
  
  IPC_STRUCT_MEMBER(std::string, method)

  
  IPC_STRUCT_MEMBER(GURL, url)

  
  
  
  
  
  IPC_STRUCT_MEMBER(GURL, first_party_for_cookies)

  
  IPC_STRUCT_MEMBER(GURL, referrer)

  
  IPC_STRUCT_MEMBER(blink::WebReferrerPolicy, referrer_policy)

  
  IPC_STRUCT_MEMBER(std::string, headers)

  
  IPC_STRUCT_MEMBER(int, load_flags)

  
  
  
  
  
  IPC_STRUCT_MEMBER(int, origin_pid)

  
  
  IPC_STRUCT_MEMBER(ResourceType::Type, resource_type)

  
  IPC_STRUCT_MEMBER(net::RequestPriority, priority)

  
  IPC_STRUCT_MEMBER(uint32, request_context)

  
  
  IPC_STRUCT_MEMBER(int, appcache_host_id)

  
  IPC_STRUCT_MEMBER(scoped_refptr<webkit_glue::ResourceRequestBody>,
                    request_body)

  IPC_STRUCT_MEMBER(bool, download_to_file)

  
  IPC_STRUCT_MEMBER(bool, has_user_gesture)

  
  IPC_STRUCT_MEMBER(int, render_frame_id)

  
  IPC_STRUCT_MEMBER(bool, is_main_frame)

  
  
  IPC_STRUCT_MEMBER(int64, frame_id)

  
  IPC_STRUCT_MEMBER(bool, parent_is_main_frame)

  
  
  IPC_STRUCT_MEMBER(int64, parent_frame_id)

  IPC_STRUCT_MEMBER(content::PageTransition, transition_type)

  
  
  IPC_STRUCT_MEMBER(bool, should_replace_current_entry)

  
  
  
  
  IPC_STRUCT_MEMBER(int, transferred_request_child_id)
  IPC_STRUCT_MEMBER(int, transferred_request_request_id)

  
  IPC_STRUCT_MEMBER(bool, allow_download)
IPC_STRUCT_END()


IPC_MESSAGE_CONTROL2(ResourceMsg_ReceivedResponse,
                     int ,
                     content::ResourceResponseHead)

IPC_MESSAGE_CONTROL2(ResourceMsg_ReceivedCachedMetadata,
                     int ,
                     std::vector<char> )

IPC_MESSAGE_CONTROL3(ResourceMsg_UploadProgress,
                     int ,
                     int64 ,
                     int64 )

IPC_MESSAGE_CONTROL3(ResourceMsg_ReceivedRedirect,
                     int ,
                     GURL ,
                     content::ResourceResponseHead)

IPC_MESSAGE_CONTROL4(ResourceMsg_SetDataBuffer,
                     int ,
                     base::SharedMemoryHandle ,
                     int ,
                     base::ProcessId )

IPC_MESSAGE_CONTROL4(ResourceMsg_DataReceived,
                     int ,
                     int ,
                     int ,
                     int )

IPC_MESSAGE_CONTROL3(ResourceMsg_DataDownloaded,
                     int ,
                     int ,
                     int )

IPC_MESSAGE_CONTROL5(ResourceMsg_RequestComplete,
                     int ,
                     int ,
                     bool ,
                     std::string ,
                     base::TimeTicks )


IPC_MESSAGE_ROUTED2(ResourceHostMsg_RequestResource,
                    int ,
                    ResourceHostMsg_Request)

IPC_MESSAGE_CONTROL1(ResourceHostMsg_CancelRequest,
                     int )

IPC_MESSAGE_CONTROL3(ResourceHostMsg_FollowRedirect,
                     int ,
                     bool ,
                     GURL )

IPC_SYNC_MESSAGE_ROUTED2_1(ResourceHostMsg_SyncLoad,
                           int ,
                           ResourceHostMsg_Request,
                           content::SyncLoadResult)

IPC_MESSAGE_CONTROL1(ResourceHostMsg_DataReceived_ACK,
                     int )

IPC_MESSAGE_CONTROL1(ResourceHostMsg_DataDownloaded_ACK,
                     int )

IPC_MESSAGE_CONTROL1(ResourceHostMsg_UploadProgress_ACK,
                     int )

IPC_MESSAGE_CONTROL1(ResourceHostMsg_ReleaseDownloadedFile,
                     int )

IPC_MESSAGE_CONTROL2(ResourceHostMsg_DidChangePriority,
                     int ,
                     net::RequestPriority)
