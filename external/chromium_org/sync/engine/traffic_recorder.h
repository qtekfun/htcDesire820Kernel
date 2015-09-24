// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_TRAFFIC_RECORDER_H_
#define CHROME_BROWSER_SYNC_ENGINE_TRAFFIC_RECORDER_H_

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "base/values.h"
#include "sync/base/sync_export.h"
#include "sync/protocol/sync.pb.h"

namespace sync_pb {
class ClientToServerResponse;
class ClientToServerMessage;
}

namespace syncer {

class SYNC_EXPORT_PRIVATE TrafficRecorder {
 public:
  enum TrafficMessageType {
    CLIENT_TO_SERVER_MESSAGE,
    CLIENT_TO_SERVER_RESPONSE,
    UNKNOWN_MESSAGE_TYPE
  };

  struct SYNC_EXPORT_PRIVATE TrafficRecord {
    
    std::string message;
    TrafficMessageType message_type;
    
    
    
    bool truncated;

    TrafficRecord(const std::string& message,
                  TrafficMessageType message_type,
                  bool truncated,
                  base::Time time);
    TrafficRecord();
    ~TrafficRecord();
    base::DictionaryValue* ToValue() const;

    
    base::Time timestamp;
  };

  TrafficRecorder(unsigned int max_messages, unsigned int max_message_size);
  virtual ~TrafficRecorder();

  void RecordClientToServerMessage(const sync_pb::ClientToServerMessage& msg);
  void RecordClientToServerResponse(
      const sync_pb::ClientToServerResponse& response);
  base::ListValue* ToValue() const;

  const std::deque<TrafficRecord>& records() {
    return records_;
  }

 private:
  void AddTrafficToQueue(TrafficRecord* record);
  void StoreProtoInQueue(const ::google::protobuf::MessageLite& msg,
                         TrafficMessageType type);

  
  virtual base::Time GetTime();

  
  unsigned int max_messages_;

  
  unsigned int max_message_size_;
  std::deque<TrafficRecord> records_;
  DISALLOW_COPY_AND_ASSIGN(TrafficRecorder);
};

}  

#endif  

