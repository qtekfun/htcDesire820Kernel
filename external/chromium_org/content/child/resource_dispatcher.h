// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_CHILD_RESOURCE_DISPATCHER_H_
#define CONTENT_CHILD_RESOURCE_DISPATCHER_H_

#include <deque>
#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "webkit/child/resource_loader_bridge.h"

namespace content {
class ResourceDispatcherDelegate;
struct ResourceResponseHead;

class CONTENT_EXPORT ResourceDispatcher : public IPC::Listener {
 public:
  explicit ResourceDispatcher(IPC::Sender* sender);
  virtual ~ResourceDispatcher();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  
  webkit_glue::ResourceLoaderBridge* CreateBridge(
      const webkit_glue::ResourceLoaderBridge::RequestInfo& request_info);

  
  
  int AddPendingRequest(webkit_glue::ResourceLoaderBridge::Peer* callback,
                        ResourceType::Type resource_type,
                        int origin_pid,
                        const GURL& frame_origin,
                        const GURL& request_url);

  
  
  bool RemovePendingRequest(int request_id);

  
  void CancelPendingRequest(int request_id);

  IPC::Sender* message_sender() const {
    return message_sender_;
  }

  
  void SetDefersLoading(int request_id, bool value);

  
  void DidChangePriority(int routing_id, int request_id,
                         net::RequestPriority new_priority);

  
  
  void set_delegate(ResourceDispatcherDelegate* delegate) {
    delegate_ = delegate;
  }

  
  void set_io_timestamp(base::TimeTicks io_timestamp) {
    io_timestamp_ = io_timestamp;
  }

 private:
  friend class ResourceDispatcherTest;

  typedef std::deque<IPC::Message*> MessageQueue;
  struct PendingRequestInfo {
    PendingRequestInfo();

    PendingRequestInfo(webkit_glue::ResourceLoaderBridge::Peer* peer,
                       ResourceType::Type resource_type,
                       int origin_pid,
                       const GURL& frame_origin,
                       const GURL& request_url);

    ~PendingRequestInfo();

    webkit_glue::ResourceLoaderBridge::Peer* peer;
    ResourceType::Type resource_type;
    
    
    
    int origin_pid;
    MessageQueue deferred_message_queue;
    bool is_deferred;
    
    GURL url;
    
    GURL frame_origin;
    
    GURL response_url;
    linked_ptr<IPC::Message> pending_redirect_message;
    base::TimeTicks request_start;
    base::TimeTicks response_start;
    base::TimeTicks completion_time;
    linked_ptr<base::SharedMemory> buffer;
    int buffer_size;
  };
  typedef base::hash_map<int, PendingRequestInfo> PendingRequestList;

  
  
  PendingRequestInfo* GetPendingRequestInfo(int request_id);

  
  void FollowPendingRedirect(int request_id, PendingRequestInfo& request_info);

  
  void OnUploadProgress(
      int request_id,
      int64 position,
      int64 size);
  void OnReceivedResponse(int request_id, const ResourceResponseHead&);
  void OnReceivedCachedMetadata(int request_id, const std::vector<char>& data);
  void OnReceivedRedirect(
      int request_id,
      const GURL& new_url,
      const ResourceResponseHead& response_head);
  void OnSetDataBuffer(
      int request_id,
      base::SharedMemoryHandle shm_handle,
      int shm_size,
      base::ProcessId renderer_pid);
  void OnReceivedData(
      int request_id,
      int data_offset,
      int data_length,
      int encoded_data_length);
  void OnDownloadedData(
      int request_id,
      int data_len,
      int encoded_data_length);
  void OnRequestComplete(
      int request_id,
      int error_code,
      bool was_ignored_by_handler,
      const std::string& security_info,
      const base::TimeTicks& completion_time);

  
  void DispatchMessage(const IPC::Message& message);

  
  
  void FlushDeferredMessages(int request_id);

  void ToResourceResponseInfo(
      const PendingRequestInfo& request_info,
      const ResourceResponseHead& browser_info,
      webkit_glue::ResourceResponseInfo* renderer_info) const;

  base::TimeTicks ToRendererCompletionTime(
      const PendingRequestInfo& request_info,
      const base::TimeTicks& browser_completion_time) const;

  
  
  
  base::TimeTicks ConsumeIOTimestamp();

  
  static bool IsResourceDispatcherMessage(const IPC::Message& message);

  
  
  
  
  static void ReleaseResourcesInDataMessage(const IPC::Message& message);

  
  
  
  static void ReleaseResourcesInMessageQueue(MessageQueue* queue);

  IPC::Sender* message_sender_;

  
  PendingRequestList pending_requests_;

  base::WeakPtrFactory<ResourceDispatcher> weak_factory_;

  ResourceDispatcherDelegate* delegate_;

  
  base::TimeTicks io_timestamp_;

  DISALLOW_COPY_AND_ASSIGN(ResourceDispatcher);
};

}  

#endif  
