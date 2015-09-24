// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_MESSAGE_H_
#define IPC_IPC_MESSAGE_H_

#include <string>

#include "base/basictypes.h"
#include "base/debug/trace_event.h"
#include "base/pickle.h"
#include "ipc/ipc_export.h"

#if !defined(NDEBUG)
#define IPC_MESSAGE_LOG_ENABLED
#endif

#if defined(OS_POSIX)
#include "base/memory/ref_counted.h"
#endif

namespace base {
struct FileDescriptor;
}

class FileDescriptorSet;

namespace IPC {


struct LogData;

class IPC_EXPORT Message : public Pickle {
 public:
  enum PriorityValue {
    PRIORITY_LOW = 1,
    PRIORITY_NORMAL,
    PRIORITY_HIGH
  };

  
  
  
  enum {
    PRIORITY_MASK     = 0x03,  
    SYNC_BIT          = 0x04,
    REPLY_BIT         = 0x08,
    REPLY_ERROR_BIT   = 0x10,
    UNBLOCK_BIT       = 0x20,
    PUMPING_MSGS_BIT  = 0x40,
    HAS_SENT_TIME_BIT = 0x80,
  };

  virtual ~Message();

  Message();

  
  
  Message(int32 routing_id, uint32 type, PriorityValue priority);

  
  
  
  Message(const char* data, int data_len);

  Message(const Message& other);
  Message& operator=(const Message& other);

  PriorityValue priority() const {
    return static_cast<PriorityValue>(header()->flags & PRIORITY_MASK);
  }

  
  void set_sync() {
    header()->flags |= SYNC_BIT;
  }
  bool is_sync() const {
    return (header()->flags & SYNC_BIT) != 0;
  }

  
  void set_reply() {
    header()->flags |= REPLY_BIT;
  }

  bool is_reply() const {
    return (header()->flags & REPLY_BIT) != 0;
  }

  
  
  void set_reply_error() {
    header()->flags |= REPLY_ERROR_BIT;
  }

  bool is_reply_error() const {
    return (header()->flags & REPLY_ERROR_BIT) != 0;
  }

  
  
  
  void set_unblock(bool unblock) {
    if (unblock) {
      header()->flags |= UNBLOCK_BIT;
    } else {
      header()->flags &= ~UNBLOCK_BIT;
    }
  }

  bool should_unblock() const {
    return (header()->flags & UNBLOCK_BIT) != 0;
  }

  
  
  bool is_caller_pumping_messages() const {
    return (header()->flags & PUMPING_MSGS_BIT) != 0;
  }

  uint32 type() const {
    return header()->type;
  }

  int32 routing_id() const {
    return header()->routing;
  }

  void set_routing_id(int32 new_id) {
    header()->routing = new_id;
  }

  uint32 flags() const {
    return header()->flags;
  }

  
  
  void SetHeaderValues(int32 routing, uint32 type, uint32 flags);

  template<class T, class S>
  static bool Dispatch(const Message* msg, T* obj, S* sender,
                       void (T::*func)()) {
    (obj->*func)();
    return true;
  }

  template<class T, class S>
  static bool Dispatch(const Message* msg, T* obj, S* sender,
                       void (T::*func)() const) {
    (obj->*func)();
    return true;
  }

  template<class T, class S>
  static bool Dispatch(const Message* msg, T* obj, S* sender,
                       void (T::*func)(const Message&)) {
    (obj->*func)(*msg);
    return true;
  }

  template<class T, class S>
  static bool Dispatch(const Message* msg, T* obj, S* sender,
                       void (T::*func)(const Message&) const) {
    (obj->*func)(*msg);
    return true;
  }

  
  static void Log(std::string* name, const Message* msg, std::string* l) {
  }

  
  
  static const char* FindNext(const char* range_start, const char* range_end) {
    return Pickle::FindNext(sizeof(Header), range_start, range_end);
  }

#if defined(OS_POSIX)
  
  

  
  bool WriteFileDescriptor(const base::FileDescriptor& descriptor);

  
  
  bool ReadFileDescriptor(PickleIterator* iter,
                          base::FileDescriptor* descriptor) const;

  
  bool HasFileDescriptors() const;
#endif

#ifdef IPC_MESSAGE_LOG_ENABLED
  
  
  void set_sent_time(int64 time);
  int64 sent_time() const;

  void set_received_time(int64 time) const;
  int64 received_time() const { return received_time_; }
  void set_output_params(const std::string& op) const { output_params_ = op; }
  const std::string& output_params() const { return output_params_; }
  
  
  
  
  void set_sync_log_data(LogData* data) const { log_data_ = data; }
  LogData* sync_log_data() const { return log_data_; }
  void set_dont_log() const { dont_log_ = true; }
  bool dont_log() const { return dont_log_; }
#endif

  
  void TraceMessageBegin() {
    TRACE_EVENT_FLOW_BEGIN0("ipc", "IPC", header()->flags);
  }
  
  void TraceMessageEnd() {
    TRACE_EVENT_FLOW_END0("ipc", "IPC", header()->flags);
  }

 protected:
  friend class Channel;
  friend class MessageReplyDeserializer;
  friend class SyncMessage;

#pragma pack(push, 4)
  struct Header : Pickle::Header {
    int32 routing;  
    uint32 type;    
    uint32 flags;   
#if defined(OS_POSIX)
    uint16 num_fds; 
    uint16 pad;     
#endif
  };
#pragma pack(pop)

  Header* header() {
    return headerT<Header>();
  }
  const Header* header() const {
    return headerT<Header>();
  }

  void InitLoggingVariables();

#if defined(OS_POSIX)
  
  scoped_refptr<FileDescriptorSet> file_descriptor_set_;

  
  void EnsureFileDescriptorSet();

  FileDescriptorSet* file_descriptor_set() {
    EnsureFileDescriptorSet();
    return file_descriptor_set_.get();
  }
  const FileDescriptorSet* file_descriptor_set() const {
    return file_descriptor_set_.get();
  }
#endif

#ifdef IPC_MESSAGE_LOG_ENABLED
  
  mutable int64 received_time_;
  mutable std::string output_params_;
  mutable LogData* log_data_;
  mutable bool dont_log_;
#endif
};


}  

enum SpecialRoutingIDs {
  
  MSG_ROUTING_NONE = -2,

  
  MSG_ROUTING_CONTROL = kint32max,
};

#define IPC_REPLY_ID 0xFFFFFFF0  
#define IPC_LOGGING_ID 0xFFFFFFF1  

#endif  
