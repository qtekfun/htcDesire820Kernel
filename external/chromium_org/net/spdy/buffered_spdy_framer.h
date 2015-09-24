// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_BUFFERED_SPDY_FRAMER_H_
#define NET_SPDY_BUFFERED_SPDY_FRAMER_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/socket/next_proto.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_protocol.h"

namespace net {

NET_EXPORT_PRIVATE SpdyMajorVersion NextProtoToSpdyMajorVersion(
    NextProto next_proto);

class NET_EXPORT_PRIVATE BufferedSpdyFramerVisitorInterface {
 public:
  BufferedSpdyFramerVisitorInterface() {}

  
  virtual void OnError(SpdyFramer::SpdyError error_code) = 0;

  
  virtual void OnStreamError(SpdyStreamId stream_id,
                             const std::string& description) = 0;

  
  virtual void OnSynStream(SpdyStreamId stream_id,
                           SpdyStreamId associated_stream_id,
                           SpdyPriority priority,
                           uint8 credential_slot,
                           bool fin,
                           bool unidirectional,
                           const SpdyHeaderBlock& headers) = 0;

  
  virtual void OnSynReply(SpdyStreamId stream_id,
                          bool fin,
                          const SpdyHeaderBlock& headers) = 0;

  
  virtual void OnHeaders(SpdyStreamId stream_id,
                         bool fin,
                         const SpdyHeaderBlock& headers) = 0;

  
  virtual void OnDataFrameHeader(SpdyStreamId stream_id,
                                 size_t length,
                                 bool fin) = 0;

  
  
  
  
  
  
  
  virtual void OnStreamFrameData(SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len,
                                 bool fin) = 0;

  
  
  virtual void OnSettings(bool clear_persisted) = 0;

  
  
  virtual void OnSetting(SpdySettingsIds id, uint8 flags, uint32 value) = 0;

  
  virtual void OnPing(uint32 unique_id) = 0;

  
  virtual void OnRstStream(SpdyStreamId stream_id,
                           SpdyRstStreamStatus status) = 0;

  
  virtual void OnGoAway(SpdyStreamId last_accepted_stream_id,
                        SpdyGoAwayStatus status) = 0;

  
  virtual void OnWindowUpdate(SpdyStreamId stream_id,
                              uint32 delta_window_size) = 0;

  
  virtual void OnPushPromise(SpdyStreamId stream_id,
                             SpdyStreamId promised_stream_id) = 0;

 protected:
  virtual ~BufferedSpdyFramerVisitorInterface() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(BufferedSpdyFramerVisitorInterface);
};

class NET_EXPORT_PRIVATE BufferedSpdyFramer
    : public SpdyFramerVisitorInterface {
 public:
  BufferedSpdyFramer(SpdyMajorVersion version,
                     bool enable_compression);
  virtual ~BufferedSpdyFramer();

  
  
  
  
  void set_visitor(BufferedSpdyFramerVisitorInterface* visitor);

  
  
  
  void set_debug_visitor(SpdyFramerDebugVisitorInterface* debug_visitor);

  
  virtual void OnError(SpdyFramer* spdy_framer) OVERRIDE;
  virtual void OnSynStream(SpdyStreamId stream_id,
                           SpdyStreamId associated_stream_id,
                           SpdyPriority priority,
                           uint8 credential_slot,
                           bool fin,
                           bool unidirectional) OVERRIDE;
  virtual void OnSynReply(SpdyStreamId stream_id, bool fin) OVERRIDE;
  virtual void OnHeaders(SpdyStreamId stream_id, bool fin) OVERRIDE;
  virtual bool OnCredentialFrameData(const char* frame_data,
                                     size_t len) OVERRIDE;
  virtual bool OnControlFrameHeaderData(SpdyStreamId stream_id,
                                        const char* header_data,
                                        size_t len) OVERRIDE;
  virtual void OnStreamFrameData(SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len,
                                 bool fin) OVERRIDE;
  virtual void OnSettings(bool clear_persisted) OVERRIDE;
  virtual void OnSetting(
      SpdySettingsIds id, uint8 flags, uint32 value) OVERRIDE;
  virtual void OnPing(uint32 unique_id) OVERRIDE;
  virtual void OnRstStream(SpdyStreamId stream_id,
                           SpdyRstStreamStatus status) OVERRIDE;
  virtual void OnGoAway(SpdyStreamId last_accepted_stream_id,
                        SpdyGoAwayStatus status) OVERRIDE;
  virtual void OnWindowUpdate(SpdyStreamId stream_id,
                              uint32 delta_window_size) OVERRIDE;
  virtual void OnPushPromise(SpdyStreamId stream_id,
                             SpdyStreamId promised_stream_id) OVERRIDE;
  virtual void OnDataFrameHeader(SpdyStreamId stream_id,
                                 size_t length,
                                 bool fin) OVERRIDE;

  
  size_t ProcessInput(const char* data, size_t len);
  SpdyMajorVersion protocol_version();
  void Reset();
  SpdyFramer::SpdyError error_code() const;
  SpdyFramer::SpdyState state() const;
  bool MessageFullyRead();
  bool HasError();
  SpdyFrame* CreateSynStream(SpdyStreamId stream_id,
                             SpdyStreamId associated_stream_id,
                             SpdyPriority priority,
                             uint8 credential_slot,
                             SpdyControlFlags flags,
                             const SpdyHeaderBlock* headers);
  SpdyFrame* CreateSynReply(SpdyStreamId stream_id,
                            SpdyControlFlags flags,
                            const SpdyHeaderBlock* headers);
  SpdyFrame* CreateRstStream(SpdyStreamId stream_id,
                             SpdyRstStreamStatus status) const;
  SpdyFrame* CreateSettings(const SettingsMap& values) const;
  SpdyFrame* CreatePingFrame(uint32 unique_id) const;
  SpdyFrame* CreateGoAway(
      SpdyStreamId last_accepted_stream_id,
      SpdyGoAwayStatus status) const;
  SpdyFrame* CreateHeaders(SpdyStreamId stream_id,
                           SpdyControlFlags flags,
                           const SpdyHeaderBlock* headers);
  SpdyFrame* CreateWindowUpdate(
      SpdyStreamId stream_id,
      uint32 delta_window_size) const;
  SpdyFrame* CreateDataFrame(SpdyStreamId stream_id,
                             const char* data,
                             uint32 len,
                             SpdyDataFlags flags);

  
  SpdySerializedFrame* SerializeFrame(const SpdyFrameIR& frame) {
    return spdy_framer_.SerializeFrame(frame);
  }

  SpdyPriority GetHighestPriority() const;

  size_t GetDataFrameMinimumSize() const {
    return spdy_framer_.GetDataFrameMinimumSize();
  }

  size_t GetControlFrameHeaderSize() const {
    return spdy_framer_.GetControlFrameHeaderSize();
  }

  size_t GetSynStreamMinimumSize() const {
    return spdy_framer_.GetSynStreamMinimumSize();
  }

  size_t GetFrameMinimumSize() const {
    return spdy_framer_.GetFrameMinimumSize();
  }

  size_t GetFrameMaximumSize() const {
    return spdy_framer_.GetFrameMaximumSize();
  }

  size_t GetDataFrameMaximumPayload() const {
    return spdy_framer_.GetDataFrameMaximumPayload();
  }

  int frames_received() const { return frames_received_; }

 private:
  
  enum { kHeaderBufferSize = 32 * 1024 };

  void InitHeaderStreaming(SpdyStreamId stream_id);

  SpdyFramer spdy_framer_;
  BufferedSpdyFramerVisitorInterface* visitor_;

  
  char header_buffer_[kHeaderBufferSize];
  size_t header_buffer_used_;
  bool header_buffer_valid_;
  SpdyStreamId header_stream_id_;
  int frames_received_;

  
  
  struct ControlFrameFields {
    SpdyFrameType type;
    SpdyStreamId stream_id;
    SpdyStreamId associated_stream_id;
    SpdyPriority priority;
    uint8 credential_slot;
    bool fin;
    bool unidirectional;
  };
  scoped_ptr<ControlFrameFields> control_frame_fields_;

  DISALLOW_COPY_AND_ASSIGN(BufferedSpdyFramer);
};

}  

#endif  
