// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_FRAMER_H_
#define NET_SPDY_SPDY_FRAMER_H_

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/sys_byteorder.h"
#include "net/base/net_export.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_protocol.h"


typedef struct z_stream_s z_stream;  

namespace net {

class HttpProxyClientSocketPoolTest;
class HttpNetworkLayer;
class HttpNetworkTransactionTest;
class SpdyHttpStreamTest;
class SpdyNetworkTransactionTest;
class SpdyProxyClientSocketTest;
class SpdySessionTest;
class SpdyStreamTest;
class SpdyWebSocketStreamTest;
class WebSocketJobTest;

class SpdyFramer;
class SpdyFrameBuilder;
class SpdyFramerTest;

namespace test {

class TestSpdyVisitor;

}  

typedef std::map<std::string, std::string> SpdyHeaderBlock;

class NET_EXPORT_PRIVATE SettingsFlagsAndId {
 public:
  static SettingsFlagsAndId FromWireFormat(int version, uint32 wire);

  SettingsFlagsAndId() : flags_(0), id_(0) {}

  
  SettingsFlagsAndId(uint8 flags, uint32 id);

  uint32 GetWireFormat(int version) const;

  uint32 id() const { return id_; }
  uint8 flags() const { return flags_; }

 private:
  static void ConvertFlagsAndIdForSpdy2(uint32* val);

  uint8 flags_;
  uint32 id_;
};

typedef std::pair<SpdySettingsFlags, uint32> SettingsFlagsAndValue;
typedef std::map<SpdySettingsIds, SettingsFlagsAndValue> SettingsMap;

struct NET_EXPORT_PRIVATE SpdyCredential {
  SpdyCredential();
  ~SpdyCredential();

  uint16 slot;
  std::vector<std::string> certs;
  std::string proof;
};

struct NET_EXPORT_PRIVATE SpdySettingsScratch {
  SpdySettingsScratch() { Reset(); }

  void Reset() {
    setting_buf_len = 0;
    last_setting_id = 0;
  }

  
  char setting_buf[8];

  
  size_t setting_buf_len;

  
  
  
  uint32 last_setting_id;
};

class NET_EXPORT_PRIVATE SpdyFramerVisitorInterface {
 public:
  virtual ~SpdyFramerVisitorInterface() {}

  
  virtual void OnError(SpdyFramer* framer) = 0;

  
  
  
  virtual void OnDataFrameHeader(SpdyStreamId stream_id,
                                 size_t length,
                                 bool fin) = 0;

  
  
  
  
  
  
  virtual void OnStreamFrameData(SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len,
                                 bool fin) = 0;

  
  
  
  
  
  
  
  
  
  virtual bool OnControlFrameHeaderData(SpdyStreamId stream_id,
                                        const char* header_data,
                                        size_t len) = 0;

  
  
  
  virtual void OnSynStream(SpdyStreamId stream_id,
                           SpdyStreamId associated_stream_id,
                           SpdyPriority priority,
                           uint8 credential_slot,
                           bool fin,
                           bool unidirectional) = 0;

  
  
  
  virtual void OnSynReply(SpdyStreamId stream_id, bool fin) = 0;

  
  virtual void OnRstStream(SpdyStreamId stream_id,
                           SpdyRstStreamStatus status) = 0;

  
  
  virtual void OnSettings(bool clear_persisted) {}

  
  
  virtual void OnSetting(SpdySettingsIds id, uint8 flags, uint32 value) = 0;

  
  virtual void OnPing(uint32 unique_id) = 0;

  
  virtual void OnGoAway(SpdyStreamId last_accepted_stream_id,
                        SpdyGoAwayStatus status) = 0;

  
  
  
  virtual void OnHeaders(SpdyStreamId stream_id, bool fin) = 0;

  
  virtual void OnWindowUpdate(SpdyStreamId stream_id,
                              uint32 delta_window_size) = 0;

  
  
  
  
  
  
  
  virtual bool OnCredentialFrameData(const char* credential_data,
                                     size_t len) = 0;

  
  virtual void OnBlocked(SpdyStreamId stream_id) {}

  
  
  
  virtual void OnPushPromise(SpdyStreamId stream_id,
                             SpdyStreamId promised_stream_id) = 0;
};

class NET_EXPORT_PRIVATE SpdyFramerDebugVisitorInterface {
 public:
  virtual ~SpdyFramerDebugVisitorInterface() {}

  
  
  
  
  virtual void OnSendCompressedFrame(SpdyStreamId stream_id,
                                     SpdyFrameType type,
                                     size_t payload_len,
                                     size_t frame_len) {}

  
  
  
  virtual void OnReceiveCompressedFrame(SpdyStreamId stream_id,
                                        SpdyFrameType type,
                                        size_t frame_len) {}
};

class NET_EXPORT_PRIVATE SpdyFramer {
 public:
  
  
  
  enum SpdyState {
    SPDY_ERROR,
    SPDY_RESET,
    SPDY_AUTO_RESET,
    SPDY_READING_COMMON_HEADER,
    SPDY_CONTROL_FRAME_PAYLOAD,
    SPDY_IGNORE_REMAINING_PAYLOAD,
    SPDY_FORWARD_STREAM_FRAME,
    SPDY_CONTROL_FRAME_BEFORE_HEADER_BLOCK,
    SPDY_CONTROL_FRAME_HEADER_BLOCK,
    SPDY_CREDENTIAL_FRAME_PAYLOAD,
    SPDY_SETTINGS_FRAME_PAYLOAD,
  };

  
  enum SpdyError {
    SPDY_NO_ERROR,
    SPDY_INVALID_CONTROL_FRAME,        
    SPDY_CONTROL_PAYLOAD_TOO_LARGE,    
    SPDY_ZLIB_INIT_FAILURE,            
    SPDY_UNSUPPORTED_VERSION,          
    SPDY_DECOMPRESS_FAILURE,           
    SPDY_COMPRESS_FAILURE,             
    SPDY_CREDENTIAL_FRAME_CORRUPT,     
    SPDY_INVALID_DATA_FRAME_FLAGS,     
    SPDY_INVALID_CONTROL_FRAME_FLAGS,  

    LAST_ERROR,  
  };

  
  static const SpdyStreamId kInvalidStream;

  
  
  
  static const size_t kHeaderDataChunkMaxSize;

  
  static void WriteHeaderBlock(SpdyFrameBuilder* frame,
                               const int spdy_version,
                               const SpdyHeaderBlock* headers);

  
  
  static size_t GetSerializedLength(const int spdy_version,
                                    const SpdyHeaderBlock* headers);

  
  explicit SpdyFramer(SpdyMajorVersion version);
  virtual ~SpdyFramer();

  
  
  
  
  void set_visitor(SpdyFramerVisitorInterface* visitor) {
    visitor_ = visitor;
  }

  
  
  
  void set_debug_visitor(SpdyFramerDebugVisitorInterface* debug_visitor) {
    debug_visitor_ = debug_visitor;
  }

  
  
  
  size_t ProcessInput(const char* data, size_t len);

  
  
  void Reset();

  
  SpdyError error_code() const { return error_code_; }
  SpdyState state() const { return state_; }
  bool HasError() const { return state_ == SPDY_ERROR; }

  
  
  
  
  size_t ParseHeaderBlockInBuffer(const char* header_data,
                                size_t header_length,
                                SpdyHeaderBlock* block) const;

  
  
  
  
  
  
  SpdyFrame* CreateDataFrame(SpdyStreamId stream_id, const char* data,
                             uint32 len, SpdyDataFlags flags) const;
  SpdySerializedFrame* SerializeData(const SpdyDataIR& data) const;
  
  SpdySerializedFrame* SerializeDataFrameHeader(const SpdyDataIR& data) const;

  
  
  
  
  
  
  
  
  
  
  SpdyFrame* CreateSynStream(SpdyStreamId stream_id,
                             SpdyStreamId associated_stream_id,
                             SpdyPriority priority,
                             uint8 credential_slot,
                             SpdyControlFlags flags,
                             const SpdyHeaderBlock* headers);
  SpdySerializedFrame* SerializeSynStream(const SpdySynStreamIR& syn_stream);

  
  
  
  
  
  
  SpdyFrame* CreateSynReply(SpdyStreamId stream_id,
                            SpdyControlFlags flags,
                            const SpdyHeaderBlock* headers);
  SpdySerializedFrame* SerializeSynReply(const SpdySynReplyIR& syn_reply);

  SpdyFrame* CreateRstStream(SpdyStreamId stream_id,
                             SpdyRstStreamStatus status) const;
  SpdySerializedFrame* SerializeRstStream(
      const SpdyRstStreamIR& rst_stream) const;

  
  
  SpdyFrame* CreateSettings(const SettingsMap& values) const;
  SpdySerializedFrame* SerializeSettings(const SpdySettingsIR& settings) const;

  
  
  SpdyFrame* CreatePingFrame(uint32 unique_id) const;
  SpdySerializedFrame* SerializePing(const SpdyPingIR& ping) const;

  
  
  
  
  SpdyFrame* CreateGoAway(SpdyStreamId last_accepted_stream_id,
                          SpdyGoAwayStatus status) const;
  SpdySerializedFrame* SerializeGoAway(const SpdyGoAwayIR& goaway) const;

  
  
  
  SpdyFrame* CreateHeaders(SpdyStreamId stream_id,
                           SpdyControlFlags flags,
                           const SpdyHeaderBlock* headers);
  SpdySerializedFrame* SerializeHeaders(const SpdyHeadersIR& headers);

  
  
  SpdyFrame* CreateWindowUpdate(
      SpdyStreamId stream_id,
      uint32 delta_window_size) const;
  SpdySerializedFrame* SerializeWindowUpdate(
      const SpdyWindowUpdateIR& window_update) const;

  
  
  
  SpdyFrame* CreateCredentialFrame(const SpdyCredential& credential) const;
  SpdySerializedFrame* SerializeCredential(
      const SpdyCredentialIR& credential) const;

  
  
  
  
  SpdySerializedFrame* SerializeBlocked(const SpdyBlockedIR& blocked) const;

  
  
  
  
  SpdyFrame* CreatePushPromise(SpdyStreamId stream_id,
                               SpdyStreamId promised_stream_id,
                               const SpdyHeaderBlock* headers);
  SpdySerializedFrame* SerializePushPromise(
      const SpdyPushPromiseIR& push_promise);

  
  
  
  
  static bool ParseCredentialData(const char* data, size_t len,
                                  SpdyCredential* credential);

  
  SpdySerializedFrame* SerializeFrame(const SpdyFrameIR& frame);

  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  SpdyFrame* CompressFrame(const SpdyFrame& frame);

  
  void set_enable_compression(bool value) {
    enable_compression_ = value;
  }

  
  void set_display_protocol(const std::string& protocol) {
    display_protocol_ = protocol;
  }

  
  size_t GetDataFrameMinimumSize() const;
  size_t GetControlFrameHeaderSize() const;
  size_t GetSynStreamMinimumSize() const;
  size_t GetSynReplyMinimumSize() const;
  size_t GetRstStreamSize() const;
  size_t GetSettingsMinimumSize() const;
  size_t GetPingSize() const;
  size_t GetGoAwaySize() const;
  size_t GetHeadersMinimumSize() const;
  size_t GetWindowUpdateSize() const;
  size_t GetCredentialMinimumSize() const;
  size_t GetBlockedSize() const;
  size_t GetPushPromiseMinimumSize() const;

  
  size_t GetFrameMinimumSize() const;

  
  size_t GetFrameMaximumSize() const;

  
  size_t GetDataFrameMaximumPayload() const;

  
  static const char* StateToString(int state);
  static const char* ErrorCodeToString(int error_code);
  static const char* StatusCodeToString(int status_code);
  static const char* FrameTypeToString(SpdyFrameType type);

  SpdyMajorVersion protocol_version() const { return spdy_version_; }

  bool probable_http_response() const { return probable_http_response_; }

  SpdyPriority GetLowestPriority() const { return spdy_version_ < 3 ? 3 : 7; }
  SpdyPriority GetHighestPriority() const { return 0; }

  
  
  
  bool IncrementallyDecompressControlFrameHeaderData(
      SpdyStreamId stream_id,
      const char* data,
      size_t len);

 protected:
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, BasicCompression);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, ControlFrameSizesAreValidated);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, HeaderCompression);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, DecompressUncompressedFrame);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, ExpandBuffer_HeapSmash);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, HugeHeaderBlock);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, UnclosedStreamDataCompressors);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest,
                           UnclosedStreamDataCompressorsOneByteAtATime);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest,
                           UncompressLargerThanFrameBufferInitialSize);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, ReadLargeSettingsFrame);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest,
                           ReadLargeSettingsFrameInSmallChunks);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, ControlFrameAtMaxSizeLimit);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, ControlFrameTooLarge);
  friend class net::HttpNetworkLayer;  
  friend class net::HttpNetworkTransactionTest;
  friend class net::HttpProxyClientSocketPoolTest;
  friend class net::SpdyHttpStreamTest;
  friend class net::SpdyNetworkTransactionTest;
  friend class net::SpdyProxyClientSocketTest;
  friend class net::SpdySessionTest;
  friend class net::SpdyStreamTest;
  friend class net::SpdyWebSocketStreamTest;
  friend class net::WebSocketJobTest;
  friend class test::TestSpdyVisitor;

 private:
  
  
  size_t ProcessCommonHeader(const char* data, size_t len);
  size_t ProcessControlFramePayload(const char* data, size_t len);
  size_t ProcessCredentialFramePayload(const char* data, size_t len);
  size_t ProcessControlFrameBeforeHeaderBlock(const char* data, size_t len);
  size_t ProcessControlFrameHeaderBlock(const char* data, size_t len);
  size_t ProcessSettingsFramePayload(const char* data, size_t len);
  size_t ProcessDataFramePayload(const char* data, size_t len);

  
  void ProcessControlFrameHeader(uint16 control_frame_type_field);
  bool ProcessSetting(const char* data);  

  
  
  size_t GetSerializedLength(const SpdyHeaderBlock& headers);

  
  z_stream* GetHeaderCompressor();
  z_stream* GetHeaderDecompressor();

 private:
  
  
  
  bool IncrementallyDeliverControlFrameHeaderData(SpdyStreamId stream_id,
                                                  const char* data,
                                                  size_t len);

  
  
  
  
  
  
  size_t UpdateCurrentFrameBuffer(const char** data, size_t* len,
                                  size_t max_bytes);

  void WriteHeaderBlockToZ(const SpdyHeaderBlock* headers,
                           z_stream* out) const;

  void SerializeNameValueBlockWithoutCompression(
      SpdyFrameBuilder* builder,
      const SpdyNameValueBlock& name_value_block) const;

  
  void SerializeNameValueBlock(
      SpdyFrameBuilder* builder,
      const SpdyFrameWithNameValueBlockIR& frame);

  
  void set_error(SpdyError error);

  
  
  
  
  size_t GetControlFrameBufferMaxSize() const {
    
    
    
    if (spdy_version_ == SPDY2) {
      return 64 * 1024;
    }
    if (spdy_version_ == SPDY3) {
      return 16 * 1024 * 1024;
    }
    
    
    return 16 * 1024;
  }

  
  
  
  
  static const size_t kControlFrameBufferSize;

  SpdyState state_;
  SpdyState previous_state_;
  SpdyError error_code_;
  size_t remaining_data_length_;

  
  
  
  size_t remaining_control_header_;

  scoped_ptr<char[]> current_frame_buffer_;
  
  size_t current_frame_buffer_length_;

  
  SpdyFrameType current_frame_type_;

  
  uint8 current_frame_flags_;

  
  uint32 current_frame_length_;

  
  SpdyStreamId current_frame_stream_id_;

  
  
  
  SpdySettingsScratch settings_scratch_;

  bool enable_compression_;  
  
  scoped_ptr<z_stream> header_compressor_;
  scoped_ptr<z_stream> header_decompressor_;

  SpdyFramerVisitorInterface* visitor_;
  SpdyFramerDebugVisitorInterface* debug_visitor_;

  std::string display_protocol_;

  
  const SpdyMajorVersion spdy_version_;

  
  
  
  
  
  
  bool syn_frame_processed_;

  
  
  
  
  
  bool probable_http_response_;
};

}  

#endif  
