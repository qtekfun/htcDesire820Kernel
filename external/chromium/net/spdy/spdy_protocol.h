// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_SPDY_SPDY_PROTOCOL_H_
#define NET_SPDY_SPDY_PROTOCOL_H_
#pragma once

#include <limits>

#include "base/basictypes.h"
#include "base/logging.h"
#include "net/base/sys_byteorder.h"
#include "net/spdy/spdy_bitmasks.h"

namespace spdy {

const int kSpdyProtocolVersion = 2;

const size_t kSpdyStreamInitialWindowSize = 64 * 1024;  

const size_t kSpdyStreamMaximumWindowSize = std::numeric_limits<int32>::max();

const char kMethod[] = "method";
const char kStatus[] = "status";
const char kUrl[] = "url";
const char kVersion[] = "version";
const char kPath[] = "path";


enum SpdyControlType {
  SYN_STREAM = 1,
  SYN_REPLY,
  RST_STREAM,
  SETTINGS,
  NOOP,
  PING,
  GOAWAY,
  HEADERS,
  WINDOW_UPDATE,
  NUM_CONTROL_FRAME_TYPES
};

enum SpdyDataFlags {
  DATA_FLAG_NONE = 0,
  DATA_FLAG_FIN = 1,
  DATA_FLAG_COMPRESSED = 2
};

enum SpdyControlFlags {
  CONTROL_FLAG_NONE = 0,
  CONTROL_FLAG_FIN = 1,
  CONTROL_FLAG_UNIDIRECTIONAL = 2
};

enum SpdySettingsControlFlags {
  SETTINGS_FLAG_CLEAR_PREVIOUSLY_PERSISTED_SETTINGS = 0x1
};

enum SpdySettingsFlags {
  SETTINGS_FLAG_PLEASE_PERSIST = 0x1,
  SETTINGS_FLAG_PERSISTED = 0x2
};

enum SpdySettingsIds {
  SETTINGS_UPLOAD_BANDWIDTH = 0x1,
  SETTINGS_DOWNLOAD_BANDWIDTH = 0x2,
  
  SETTINGS_ROUND_TRIP_TIME = 0x3,
  SETTINGS_MAX_CONCURRENT_STREAMS = 0x4,
  
  SETTINGS_CURRENT_CWND = 0x5,
  
  SETTINGS_DOWNLOAD_RETRANS_RATE = 0x6,
  
  SETTINGS_INITIAL_WINDOW_SIZE = 0x7
};

enum SpdyStatusCodes {
  INVALID = 0,
  PROTOCOL_ERROR = 1,
  INVALID_STREAM = 2,
  REFUSED_STREAM = 3,
  UNSUPPORTED_VERSION = 4,
  CANCEL = 5,
  INTERNAL_ERROR = 6,
  FLOW_CONTROL_ERROR = 7,
  INVALID_ASSOCIATED_STREAM = 8,
  NUM_STATUS_CODES = 9
};

typedef uint32 SpdyStreamId;

typedef uint8 SpdyPriority;

#define SPDY_PRIORITY_LOWEST 3
#define SPDY_PRIORITY_HIGHEST 0


#pragma pack(push)
#pragma pack(1)

union FlagsAndLength {
  uint8 flags_[4];  
  uint32 length_;   
};

struct SpdyFrameBlock {
  union {
    struct {
      uint16 version_;
      uint16 type_;
    } control_;
    struct {
      SpdyStreamId stream_id_;
    } data_;
  };
  FlagsAndLength flags_length_;
};

struct SpdySynStreamControlFrameBlock : SpdyFrameBlock {
  SpdyStreamId stream_id_;
  SpdyStreamId associated_stream_id_;
  SpdyPriority priority_;
  uint8 unused_;
};

struct SpdySynReplyControlFrameBlock : SpdyFrameBlock {
  SpdyStreamId stream_id_;
  uint16 unused_;
};

struct SpdyRstStreamControlFrameBlock : SpdyFrameBlock {
  SpdyStreamId stream_id_;
  uint32 status_;
};

struct SpdySettingsControlFrameBlock : SpdyFrameBlock {
  uint32 num_entries_;
  
};

struct SpdyNoopControlFrameBlock : SpdyFrameBlock {
};

struct SpdyPingControlFrameBlock : SpdyFrameBlock {
  uint32 unique_id_;
};

struct SpdyGoAwayControlFrameBlock : SpdyFrameBlock {
  SpdyStreamId last_accepted_stream_id_;
};

struct SpdyHeadersControlFrameBlock : SpdyFrameBlock {
  SpdyStreamId stream_id_;
  uint16 unused_;
};

struct SpdyWindowUpdateControlFrameBlock : SpdyFrameBlock {
  SpdyStreamId stream_id_;
  uint32 delta_window_size_;
};

union SettingsFlagsAndId {
  
  SettingsFlagsAndId(uint32 val) : id_(val) {}
  uint8 flags() const { return flags_[0]; }
  void set_flags(uint8 flags) { flags_[0] = flags; }
  uint32 id() const { return (ntohl(id_) & kSettingsIdMask); }
  void set_id(uint32 id) {
    DCHECK_EQ(0u, (id & ~kSettingsIdMask));
    id = htonl(id & kSettingsIdMask);
    id_ = flags() | id;
  }

  uint8 flags_[4];  
  uint32 id_;       
};

#pragma pack(pop)


class SpdyFrame {
 public:
  
  explicit SpdyFrame(size_t size) : frame_(NULL), owns_buffer_(true) {
    DCHECK_GE(size, sizeof(struct SpdyFrameBlock));
    char* buffer = new char[size];
    memset(buffer, 0, size);
    frame_ = reinterpret_cast<struct SpdyFrameBlock*>(buffer);
  }

  
  
  
  
  
  
  
  SpdyFrame(char* data, bool owns_buffer)
      : frame_(reinterpret_cast<struct SpdyFrameBlock*>(data)),
        owns_buffer_(owns_buffer) {
    DCHECK(frame_);
  }

  ~SpdyFrame() {
    if (owns_buffer_) {
      char* buffer = reinterpret_cast<char*>(frame_);
      delete [] buffer;
    }
    frame_ = NULL;
  }

  
  
  char* data() const { return reinterpret_cast<char*>(frame_); }

  uint8 flags() const { return frame_->flags_length_.flags_[0]; }
  void set_flags(uint8 flags) { frame_->flags_length_.flags_[0] = flags; }

  uint32 length() const {
    return ntohl(frame_->flags_length_.length_) & kLengthMask;
  }

  void set_length(uint32 length) {
    DCHECK_EQ(0u, (length & ~kLengthMask));
    length = htonl(length & kLengthMask);
    frame_->flags_length_.length_ = flags() | length;
  }

  bool is_control_frame() const {
    return (ntohs(frame_->control_.version_) & kControlFlagMask) ==
        kControlFlagMask;
  }

  
  
  
  
  static size_t size() { return sizeof(struct SpdyFrameBlock); }

 protected:
  SpdyFrameBlock* frame_;

 private:
  bool owns_buffer_;
  DISALLOW_COPY_AND_ASSIGN(SpdyFrame);
};

class SpdyDataFrame : public SpdyFrame {
 public:
  SpdyDataFrame() : SpdyFrame(size()) {}
  SpdyDataFrame(char* data, bool owns_buffer)
      : SpdyFrame(data, owns_buffer) {}

  SpdyStreamId stream_id() const {
    return ntohl(frame_->data_.stream_id_) & kStreamIdMask;
  }

  
  
  
  void set_stream_id(SpdyStreamId id) {
    DCHECK_EQ(0u, (id & ~kStreamIdMask));
    frame_->data_.stream_id_ = htonl(id & kStreamIdMask);
  }

  
  
  static size_t size() { return SpdyFrame::size(); }

  const char* payload() const {
    return reinterpret_cast<const char*>(frame_) + size();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SpdyDataFrame);
};

class SpdyControlFrame : public SpdyFrame {
 public:
  explicit SpdyControlFrame(size_t size) : SpdyFrame(size) {}
  SpdyControlFrame(char* data, bool owns_buffer)
      : SpdyFrame(data, owns_buffer) {}

  
  
  bool AppearsToBeAValidControlFrame() const {
    
    uint16 type = ntohs(block()->control_.type_);
    return (type >= SYN_STREAM && type < NUM_CONTROL_FRAME_TYPES);
  }

  uint16 version() const {
    const int kVersionMask = 0x7fff;
    return ntohs(block()->control_.version_) & kVersionMask;
  }

  void set_version(uint16 version) {
    DCHECK_EQ(0u, version & kControlFlagMask);
    mutable_block()->control_.version_ = htons(kControlFlagMask | version);
  }

  SpdyControlType type() const {
    uint16 type = ntohs(block()->control_.type_);
    DCHECK(type >= SYN_STREAM && type < NUM_CONTROL_FRAME_TYPES);
    return static_cast<SpdyControlType>(type);
  }

  void set_type(SpdyControlType type) {
    DCHECK(type >= SYN_STREAM && type < NUM_CONTROL_FRAME_TYPES);
    mutable_block()->control_.type_ = htons(type);
  }

  
  
  bool has_header_block() const {
    return type() == SYN_STREAM || type() == SYN_REPLY || type() == HEADERS;
  }

  
  
  static size_t size() { return sizeof(SpdyFrameBlock); }

  
  static const size_t kNumNameValuePairsSize = 2;

  
  static const size_t kLengthOfNameSize = 2;

  
  static const size_t kLengthOfValueSize = 2;

 private:
  const struct SpdyFrameBlock* block() const {
    return frame_;
  }
  struct SpdyFrameBlock* mutable_block() {
    return frame_;
  }
  DISALLOW_COPY_AND_ASSIGN(SpdyControlFrame);
};

class SpdySynStreamControlFrame : public SpdyControlFrame {
 public:
  SpdySynStreamControlFrame() : SpdyControlFrame(size()) {}
  SpdySynStreamControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  SpdyStreamId stream_id() const {
    return ntohl(block()->stream_id_) & kStreamIdMask;
  }

  void set_stream_id(SpdyStreamId id) {
    mutable_block()->stream_id_ = htonl(id & kStreamIdMask);
  }

  SpdyStreamId associated_stream_id() const {
    return ntohl(block()->associated_stream_id_) & kStreamIdMask;
  }

  void set_associated_stream_id(SpdyStreamId id) {
    mutable_block()->associated_stream_id_ = htonl(id & kStreamIdMask);
  }

  SpdyPriority priority() const {
    return (block()->priority_ & kPriorityMask) >> 6;
  }

  
  int header_block_len() const {
    return length() - (size() - SpdyFrame::size());
  }

  const char* header_block() const {
    return reinterpret_cast<const char*>(block()) + size();
  }

  
  
  static size_t size() { return sizeof(SpdySynStreamControlFrameBlock); }

 private:
  const struct SpdySynStreamControlFrameBlock* block() const {
    return static_cast<SpdySynStreamControlFrameBlock*>(frame_);
  }
  struct SpdySynStreamControlFrameBlock* mutable_block() {
    return static_cast<SpdySynStreamControlFrameBlock*>(frame_);
  }
  DISALLOW_COPY_AND_ASSIGN(SpdySynStreamControlFrame);
};

class SpdySynReplyControlFrame : public SpdyControlFrame {
 public:
  SpdySynReplyControlFrame() : SpdyControlFrame(size()) {}
  SpdySynReplyControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  SpdyStreamId stream_id() const {
    return ntohl(block()->stream_id_) & kStreamIdMask;
  }

  void set_stream_id(SpdyStreamId id) {
    mutable_block()->stream_id_ = htonl(id & kStreamIdMask);
  }

  int header_block_len() const {
    return length() - (size() - SpdyFrame::size());
  }

  const char* header_block() const {
    return reinterpret_cast<const char*>(block()) + size();
  }

  
  
  static size_t size() { return sizeof(SpdySynReplyControlFrameBlock); }

 private:
  const struct SpdySynReplyControlFrameBlock* block() const {
    return static_cast<SpdySynReplyControlFrameBlock*>(frame_);
  }
  struct SpdySynReplyControlFrameBlock* mutable_block() {
    return static_cast<SpdySynReplyControlFrameBlock*>(frame_);
  }
  DISALLOW_COPY_AND_ASSIGN(SpdySynReplyControlFrame);
};

class SpdyRstStreamControlFrame : public SpdyControlFrame {
 public:
  SpdyRstStreamControlFrame() : SpdyControlFrame(size()) {}
  SpdyRstStreamControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  SpdyStreamId stream_id() const {
    return ntohl(block()->stream_id_) & kStreamIdMask;
  }

  void set_stream_id(SpdyStreamId id) {
    mutable_block()->stream_id_ = htonl(id & kStreamIdMask);
  }

  SpdyStatusCodes status() const {
    SpdyStatusCodes status =
        static_cast<SpdyStatusCodes>(ntohl(block()->status_));
    if (status < INVALID || status >= NUM_STATUS_CODES) {
      status = INVALID;
    }
    return status;
  }
  void set_status(SpdyStatusCodes status) {
    mutable_block()->status_ = htonl(static_cast<uint32>(status));
  }

  
  
  static size_t size() { return sizeof(SpdyRstStreamControlFrameBlock); }

 private:
  const struct SpdyRstStreamControlFrameBlock* block() const {
    return static_cast<SpdyRstStreamControlFrameBlock*>(frame_);
  }
  struct SpdyRstStreamControlFrameBlock* mutable_block() {
    return static_cast<SpdyRstStreamControlFrameBlock*>(frame_);
  }
  DISALLOW_COPY_AND_ASSIGN(SpdyRstStreamControlFrame);
};

class SpdySettingsControlFrame : public SpdyControlFrame {
 public:
  SpdySettingsControlFrame() : SpdyControlFrame(size()) {}
  SpdySettingsControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  uint32 num_entries() const {
    return ntohl(block()->num_entries_);
  }

  void set_num_entries(int val) {
    mutable_block()->num_entries_ = htonl(val);
  }

  int header_block_len() const {
    return length() - (size() - SpdyFrame::size());
  }

  const char* header_block() const {
    return reinterpret_cast<const char*>(block()) + size();
  }

  
  
  static size_t size() { return sizeof(SpdySettingsControlFrameBlock); }

 private:
  const struct SpdySettingsControlFrameBlock* block() const {
    return static_cast<SpdySettingsControlFrameBlock*>(frame_);
  }
  struct SpdySettingsControlFrameBlock* mutable_block() {
    return static_cast<SpdySettingsControlFrameBlock*>(frame_);
  }
  DISALLOW_COPY_AND_ASSIGN(SpdySettingsControlFrame);
};

class SpdyNoOpControlFrame : public SpdyControlFrame {
 public:
  SpdyNoOpControlFrame() : SpdyControlFrame(size()) {}
  SpdyNoOpControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  static size_t size() { return sizeof(SpdyNoopControlFrameBlock); }
};

class SpdyPingControlFrame : public SpdyControlFrame {
 public:
  SpdyPingControlFrame() : SpdyControlFrame(size()) {}
  SpdyPingControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  uint32 unique_id() const {
    return ntohl(block()->unique_id_);
  }

  void set_unique_id(uint32 unique_id) {
    mutable_block()->unique_id_ = htonl(unique_id);
  }

  static size_t size() { return sizeof(SpdyPingControlFrameBlock); }

 private:
  const struct SpdyPingControlFrameBlock* block() const {
    return static_cast<SpdyPingControlFrameBlock*>(frame_);
  }
  struct SpdyPingControlFrameBlock* mutable_block() {
    return static_cast<SpdyPingControlFrameBlock*>(frame_);
  }
};

class SpdyGoAwayControlFrame : public SpdyControlFrame {
 public:
  SpdyGoAwayControlFrame() : SpdyControlFrame(size()) {}
  SpdyGoAwayControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  SpdyStreamId last_accepted_stream_id() const {
    return ntohl(block()->last_accepted_stream_id_) & kStreamIdMask;
  }

  void set_last_accepted_stream_id(SpdyStreamId id) {
    mutable_block()->last_accepted_stream_id_ = htonl(id & kStreamIdMask);
  }

  static size_t size() { return sizeof(SpdyGoAwayControlFrameBlock); }

 private:
  const struct SpdyGoAwayControlFrameBlock* block() const {
    return static_cast<SpdyGoAwayControlFrameBlock*>(frame_);
  }
  struct SpdyGoAwayControlFrameBlock* mutable_block() {
    return static_cast<SpdyGoAwayControlFrameBlock*>(frame_);
  }
  DISALLOW_COPY_AND_ASSIGN(SpdyGoAwayControlFrame);
};

class SpdyHeadersControlFrame : public SpdyControlFrame {
 public:
  SpdyHeadersControlFrame() : SpdyControlFrame(size()) {}
  SpdyHeadersControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  SpdyStreamId stream_id() const {
    return ntohl(block()->stream_id_) & kStreamIdMask;
  }

  void set_stream_id(SpdyStreamId id) {
    mutable_block()->stream_id_ = htonl(id & kStreamIdMask);
  }

  
  int header_block_len() const {
    return length() - (size() - SpdyFrame::size());
  }

  const char* header_block() const {
    return reinterpret_cast<const char*>(block()) + size();
  }

  
  
  static size_t size() { return sizeof(SpdyHeadersControlFrameBlock); }

 private:
  const struct SpdyHeadersControlFrameBlock* block() const {
    return static_cast<SpdyHeadersControlFrameBlock*>(frame_);
  }
  struct SpdyHeadersControlFrameBlock* mutable_block() {
    return static_cast<SpdyHeadersControlFrameBlock*>(frame_);
  }
  DISALLOW_COPY_AND_ASSIGN(SpdyHeadersControlFrame);
};

class SpdyWindowUpdateControlFrame : public SpdyControlFrame {
 public:
  SpdyWindowUpdateControlFrame() : SpdyControlFrame(size()) {}
  SpdyWindowUpdateControlFrame(char* data, bool owns_buffer)
      : SpdyControlFrame(data, owns_buffer) {}

  SpdyStreamId stream_id() const {
    return ntohl(block()->stream_id_) & kStreamIdMask;
  }

  void set_stream_id(SpdyStreamId id) {
    mutable_block()->stream_id_ = htonl(id & kStreamIdMask);
  }

  uint32 delta_window_size() const {
    return ntohl(block()->delta_window_size_);
  }

  void set_delta_window_size(uint32 delta_window_size) {
    mutable_block()->delta_window_size_ = htonl(delta_window_size);
  }

  
  
  static size_t size() { return sizeof(SpdyWindowUpdateControlFrameBlock); }

 private:
  const struct SpdyWindowUpdateControlFrameBlock* block() const {
    return static_cast<SpdyWindowUpdateControlFrameBlock*>(frame_);
  }
  struct SpdyWindowUpdateControlFrameBlock* mutable_block() {
    return static_cast<SpdyWindowUpdateControlFrameBlock*>(frame_);
  }

  DISALLOW_COPY_AND_ASSIGN(SpdyWindowUpdateControlFrame);
};

}  

#endif  
