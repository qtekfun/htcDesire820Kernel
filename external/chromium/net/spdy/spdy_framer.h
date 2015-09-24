// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_FRAMER_H_
#define NET_SPDY_SPDY_FRAMER_H_
#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/sys_byteorder.h"
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
}

namespace spdy {

class SpdyFramer;
class SpdyFramerTest;

namespace test {
class TestSpdyVisitor;
void FramerSetEnableCompressionHelper(SpdyFramer* framer, bool compress);
}  

typedef std::map<std::string, std::string> SpdyHeaderBlock;

typedef std::pair<spdy::SettingsFlagsAndId, uint32> SpdySetting;
typedef std::list<SpdySetting> SpdySettings;

class SpdyFramerVisitorInterface {
 public:
  virtual ~SpdyFramerVisitorInterface() {}

  
  virtual void OnError(SpdyFramer* framer) = 0;

  
  virtual void OnControl(const SpdyControlFrame* frame) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual bool OnControlFrameHeaderData(SpdyStreamId stream_id,
                                        const char* header_data,
                                        size_t len) = 0;

  
  
  
  virtual void OnDataFrameHeader(const SpdyDataFrame* frame) = 0;

  
  
  
  
  
  
  virtual void OnStreamFrameData(SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len) = 0;
};

class SpdyFramer {
 public:
  
  
  
  enum SpdyState {
    SPDY_ERROR,
    SPDY_DONE,
    SPDY_RESET,
    SPDY_AUTO_RESET,
    SPDY_READING_COMMON_HEADER,
    SPDY_INTERPRET_CONTROL_FRAME_COMMON_HEADER,
    SPDY_CONTROL_FRAME_PAYLOAD,
    SPDY_IGNORE_REMAINING_PAYLOAD,
    SPDY_FORWARD_STREAM_FRAME,
    SPDY_CONTROL_FRAME_BEFORE_HEADER_BLOCK,
    SPDY_CONTROL_FRAME_HEADER_BLOCK,
  };

  
  enum SpdyError {
    SPDY_NO_ERROR,
    SPDY_INVALID_CONTROL_FRAME,      
    SPDY_CONTROL_PAYLOAD_TOO_LARGE,  
    SPDY_ZLIB_INIT_FAILURE,          
    SPDY_UNSUPPORTED_VERSION,        
    SPDY_DECOMPRESS_FAILURE,         
    SPDY_COMPRESS_FAILURE,           

    LAST_ERROR,  
  };

  
  static const SpdyStreamId kInvalidStream;

  
  
  
  static const size_t kHeaderDataChunkMaxSize;

  
  SpdyFramer();
  virtual ~SpdyFramer();

  
  
  
  
  void set_visitor(SpdyFramerVisitorInterface* visitor) {
    visitor_ = visitor;
  }

  
  
  
  size_t ProcessInput(const char* data, size_t len);

  
  
  void Reset();

  
  SpdyError error_code() const { return error_code_; }
  SpdyState state() const { return state_; }

  bool MessageFullyRead() {
    return state_ == SPDY_DONE || state_ == SPDY_AUTO_RESET;
  }
  bool HasError() { return state_ == SPDY_ERROR; }

  
  
  
  
  bool ParseHeaderBlock(const SpdyFrame* frame, SpdyHeaderBlock* block);

  
  
  
  
  static bool ParseHeaderBlockInBuffer(const char* header_data,
                                       size_t header_length,
                                       SpdyHeaderBlock* block);

  
  
  
  
  
  
  
  
  SpdySynStreamControlFrame* CreateSynStream(SpdyStreamId stream_id,
                                             SpdyStreamId associated_stream_id,
                                             int priority,
                                             SpdyControlFlags flags,
                                             bool compressed,
                                             const SpdyHeaderBlock* headers);

  
  
  
  
  
  
  SpdySynReplyControlFrame* CreateSynReply(SpdyStreamId stream_id,
                                           SpdyControlFlags flags,
                                           bool compressed,
                                           const SpdyHeaderBlock* headers);

  static SpdyRstStreamControlFrame* CreateRstStream(SpdyStreamId stream_id,
                                                    SpdyStatusCodes status);

  
  
  
  static SpdySettingsControlFrame* CreateSettings(const SpdySettings& values);

  static SpdyNoOpControlFrame* CreateNopFrame();

  
  
  static SpdyPingControlFrame* CreatePingFrame(uint32 unique_id);

  
  
  
  
  static SpdyGoAwayControlFrame* CreateGoAway(
      SpdyStreamId last_accepted_stream_id);

  
  
  
  SpdyHeadersControlFrame* CreateHeaders(SpdyStreamId stream_id,
                                         SpdyControlFlags flags,
                                         bool compressed,
                                         const SpdyHeaderBlock* headers);

  
  
  static SpdyWindowUpdateControlFrame* CreateWindowUpdate(
      SpdyStreamId stream_id,
      uint32 delta_window_size);

  
  
  static bool ParseSettings(const SpdySettingsControlFrame* frame,
      SpdySettings* settings);

  
  
  
  
  
  
  
  SpdyDataFrame* CreateDataFrame(SpdyStreamId stream_id, const char* data,
                                 uint32 len, SpdyDataFlags flags);

  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  SpdyFrame* CompressFrame(const SpdyFrame& frame);

  
  
  
  
  
  SpdyFrame* DecompressFrame(const SpdyFrame& frame);

  
  
  SpdyFrame* DuplicateFrame(const SpdyFrame& frame);

  
  bool IsCompressible(const SpdyFrame& frame) const;

  
  
  static size_t GetMinimumControlFrameSize(SpdyControlType type);

  
  
  
  static SpdyStreamId GetControlFrameStreamId(
      const SpdyControlFrame* control_frame);

  
  void set_enable_compression(bool value);

  
  
  void set_validate_control_frame_sizes(bool value);
  static void set_enable_compression_default(bool value);

  
  static const char* StateToString(int state);
  static const char* ErrorCodeToString(int error_code);
  static const char* StatusCodeToString(int status_code);
  static const char* ControlTypeToString(SpdyControlType type);
  static void set_protocol_version(int version) { spdy_version_= version; }
  static int protocol_version() { return spdy_version_; }

  
  static const char kDictionary[];
  static const int kDictionarySize;

 protected:
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, DataCompression);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, ExpandBuffer_HeapSmash);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, HugeHeaderBlock);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest, UnclosedStreamDataCompressors);
  FRIEND_TEST_ALL_PREFIXES(SpdyFramerTest,
                           UncompressLargerThanFrameBufferInitialSize);
  friend class net::HttpNetworkLayer;  
  friend class net::HttpNetworkTransactionTest;
  friend class net::HttpProxyClientSocketPoolTest;
  friend class net::SpdyHttpStreamTest;
  friend class net::SpdyNetworkTransactionTest;
  friend class net::SpdyProxyClientSocketTest;
  friend class net::SpdySessionTest;
  friend class net::SpdyStreamTest;
  friend class test::TestSpdyVisitor;
  friend void test::FramerSetEnableCompressionHelper(SpdyFramer* framer,
                                                     bool compress);

  
  
  
  
  
  static size_t kControlFrameBufferInitialSize;

  
  
  static size_t kControlFrameBufferMaxSize;

 private:
  typedef std::map<SpdyStreamId, z_stream*> CompressorMap;

  
  
  size_t ProcessCommonHeader(const char* data, size_t len);
  void ProcessControlFrameHeader();
  size_t ProcessControlFramePayload(const char* data, size_t len);
  size_t ProcessControlFrameBeforeHeaderBlock(const char* data, size_t len);
  
  
  size_t NewProcessControlFrameHeaderBlock(const char* data, size_t len);
  size_t ProcessControlFrameHeaderBlock(const char* data, size_t len);
  size_t ProcessDataFramePayload(const char* data, size_t len);

  
  z_stream* GetHeaderCompressor();
  z_stream* GetHeaderDecompressor();
  z_stream* GetStreamCompressor(SpdyStreamId id);
  z_stream* GetStreamDecompressor(SpdyStreamId id);

  
  SpdyControlFrame* CompressControlFrame(const SpdyControlFrame& frame);
  SpdyDataFrame* CompressDataFrame(const SpdyDataFrame& frame);
  SpdyControlFrame* DecompressControlFrame(const SpdyControlFrame& frame);
  SpdyDataFrame* DecompressDataFrame(const SpdyDataFrame& frame);
  SpdyFrame* CompressFrameWithZStream(const SpdyFrame& frame,
                                      z_stream* compressor);
  SpdyFrame* DecompressFrameWithZStream(const SpdyFrame& frame,
                                        z_stream* decompressor);
  void CleanupCompressorForStream(SpdyStreamId id);
  void CleanupDecompressorForStream(SpdyStreamId id);
  void CleanupStreamCompressorsAndDecompressors();

  
  size_t BytesSafeToRead() const;

  
  
  
  bool IncrementallyDecompressControlFrameHeaderData(
      const SpdyControlFrame* frame);

  
  
  
  bool IncrementallyDecompressControlFrameHeaderData(
      const SpdyControlFrame* frame,
      const char* data,
      size_t len);

  
  
  
  bool IncrementallyDeliverControlFrameHeaderData(const SpdyControlFrame* frame,
                                                  const char* data,
                                                  size_t len);

  
  
  
  
  
  
  size_t UpdateCurrentFrameBuffer(const char** data, size_t* len,
                                  size_t max_bytes);

  
  void set_error(SpdyError error);

  
  void ExpandControlFrameBuffer(size_t size);

  
  
  bool GetFrameBoundaries(const SpdyFrame& frame, int* payload_length,
                          int* header_length, const char** payload) const;

  int num_stream_compressors() const { return stream_compressors_.size(); }
  int num_stream_decompressors() const { return stream_decompressors_.size(); }

  
  
  
  
  
  
  
  
  
  static const size_t kUncompressedControlFrameBufferInitialSize = 18 + 16;

  
  static const size_t kDecompressionBufferSize = 8 * 1024;

  SpdyState state_;
  SpdyError error_code_;
  size_t remaining_data_;

  
  
  size_t remaining_control_payload_;

  
  
  
  size_t remaining_control_header_;

  char* current_frame_buffer_;
  size_t current_frame_len_;  
  size_t current_frame_capacity_;

  bool validate_control_frame_sizes_;
  bool enable_compression_;  
  
  scoped_ptr<z_stream> header_compressor_;
  scoped_ptr<z_stream> header_decompressor_;

  
  CompressorMap stream_compressors_;
  CompressorMap stream_decompressors_;

  SpdyFramerVisitorInterface* visitor_;

  static bool compression_default_;
  static int spdy_version_;
};

}  

#endif  
