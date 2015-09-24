// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_ENGINE_MCS_CLIENT_H_
#define GOOGLE_APIS_GCM_ENGINE_MCS_CLIENT_H_

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "google_apis/gcm/base/gcm_export.h"
#include "google_apis/gcm/base/mcs_message.h"
#include "google_apis/gcm/engine/connection_handler.h"
#include "google_apis/gcm/engine/rmq_store.h"

namespace google {
namespace protobuf {
class MessageLite;
}  
}  

namespace mcs_proto {
class LoginRequest;
}

namespace gcm {

class ConnectionFactory;
struct ReliablePacketInfo;

class GCM_EXPORT MCSClient {
 public:
  enum State {
    UNINITIALIZED,    
    LOADING,          
    LOADED,           
    CONNECTING,       
    CONNECTED,        
  };

  
  
  
  typedef base::Callback<
      void(bool success,
           uint64 restored_android_id,
           uint64 restored_security_token)> InitializationCompleteCallback;
  
  typedef base::Callback<void(const MCSMessage& message)>
      OnMessageReceivedCallback;
  
  
  
  
  typedef base::Callback<void(const std::string& message_id)>
      OnMessageSentCallback;

  MCSClient(const base::FilePath& rmq_path,
            ConnectionFactory* connection_factory,
            scoped_refptr<base::SequencedTaskRunner> blocking_task_runner);
  virtual ~MCSClient();

  
  
  
  
  
  
  
  
  
  void Initialize(const InitializationCompleteCallback& initialization_callback,
                  const OnMessageReceivedCallback& message_received_callback,
                  const OnMessageSentCallback& message_sent_callback);

  
  
  
  
  
  
  
  void Login(uint64 android_id, uint64 security_token);

  
  
  
  void SendMessage(const MCSMessage& message, bool use_rmq);

  
  
  
  void Destroy();

  
  State state() const { return state_; }

 private:
  typedef uint32 StreamId;
  typedef std::string PersistentId;
  typedef std::vector<StreamId> StreamIdList;
  typedef std::vector<PersistentId> PersistentIdList;
  typedef std::map<StreamId, PersistentId> StreamIdToPersistentIdMap;
  typedef linked_ptr<ReliablePacketInfo> MCSPacketInternal;

  
  
  
  
  void ResetStateAndBuildLoginRequest(mcs_proto::LoginRequest* request);

  
  void SendHeartbeat();

  
  void OnRMQLoadFinished(const RMQStore::LoadResult& result);
  void OnRMQUpdateFinished(bool success);

  
  void MaybeSendMessage();

  
  
  void SendPacketToWire(ReliablePacketInfo* packet_info);

  
  void HandleMCSDataMesssage(
      scoped_ptr<google::protobuf::MessageLite> protobuf);

  
  void HandlePacketFromWire(scoped_ptr<google::protobuf::MessageLite> protobuf);

  
  
  
  void HandleStreamAck(StreamId last_stream_id_received_);
  
  
  void HandleSelectiveAck(const PersistentIdList& id_list);
  
  
  void HandleServerConfirmedReceipt(StreamId device_stream_id);

  
  
  virtual PersistentId GetNextPersistentId();

  
  State state_;

  
  InitializationCompleteCallback initialization_callback_;
  OnMessageReceivedCallback message_received_callback_;
  OnMessageSentCallback message_sent_callback_;

  
  uint64 android_id_;
  uint64 security_token_;

  
  ConnectionFactory* connection_factory_;

  
  
  ConnectionHandler* connection_handler_;

  
  
  

  
  std::deque<MCSPacketInternal> to_send_;
  std::deque<MCSPacketInternal> to_resend_;

  
  StreamId last_device_to_server_stream_id_received_;
  
  StreamId last_server_to_device_stream_id_received_;
  
  
  StreamId stream_id_out_;
  
  
  
  StreamId stream_id_in_;

  
  
  StreamIdToPersistentIdMap unacked_server_ids_;

  
  
  
  
  std::map<StreamId, PersistentIdList> acked_server_ids_;

  
  
  
  PersistentIdList restored_unackeds_server_ids_;

  
  RMQStore rmq_store_;

  
  
  base::TimeDelta heartbeat_interval_;
  
  base::Timer heartbeat_timer_;

  
  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  base::WeakPtrFactory<MCSClient> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MCSClient);
};

} 

#endif  
