// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_BASE_MCS_UTIL_H_
#define GOOGLE_APIS_GCM_BASE_MCS_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "google_apis/gcm/base/gcm_export.h"
#include "google_apis/gcm/protocol/mcs.pb.h"

namespace net {
class StreamSocket;
}

namespace gcm {

enum MCSProtoTag {
  kHeartbeatPingTag = 0,
  kHeartbeatAckTag,
  kLoginRequestTag,
  kLoginResponseTag,
  kCloseTag,
  kMessageStanzaTag,
  kPresenceStanzaTag,
  kIqStanzaTag,
  kDataMessageStanzaTag,
  kBatchPresenceStanzaTag,
  kStreamErrorStanzaTag,
  kHttpRequestTag,
  kHttpResponseTag,
  kBindAccountRequestTag,
  kBindAccountResponseTag,
  kTalkMetadataTag,
  kNumProtoTypes,
};

enum MCSIqStanzaExtension {
  kSelectiveAck = 12,
  kStreamAck = 13,
};

GCM_EXPORT scoped_ptr<mcs_proto::LoginRequest> BuildLoginRequest(
    uint64 auth_id,
    uint64 auth_token);

GCM_EXPORT scoped_ptr<mcs_proto::IqStanza> BuildStreamAck();
GCM_EXPORT scoped_ptr<mcs_proto::IqStanza> BuildSelectiveAck(
    const std::vector<std::string>& acked_ids);

GCM_EXPORT scoped_ptr<google::protobuf::MessageLite>
    BuildProtobufFromTag(uint8 tag);
GCM_EXPORT int GetMCSProtoTag(const google::protobuf::MessageLite& message);

GCM_EXPORT std::string GetPersistentId(
    const google::protobuf::MessageLite& message);
GCM_EXPORT void SetPersistentId(
    const std::string& persistent_id,
    google::protobuf::MessageLite* message);
GCM_EXPORT uint32 GetLastStreamIdReceived(
    const google::protobuf::MessageLite& protobuf);
GCM_EXPORT void SetLastStreamIdReceived(
    uint32 last_stream_id_received,
    google::protobuf::MessageLite* protobuf);

}  

#endif  
