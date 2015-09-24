// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_SYSTEM_CORE_CPP_H_
#define MOJO_PUBLIC_SYSTEM_CORE_CPP_H_

#include <assert.h>

#include <limits>

#include "mojo/public/system/core.h"
#include "mojo/public/system/macros.h"
#include "mojo/public/system/system_export.h"

namespace mojo {


template <class HandleType>
class ScopedHandleBase {
  MOJO_MOVE_ONLY_TYPE_FOR_CPP_03(ScopedHandleBase, RValue);

 public:
  ScopedHandleBase() {}
  explicit ScopedHandleBase(HandleType handle) : handle_(handle) {}
  ~ScopedHandleBase() { CloseIfNecessary(); }

  
  ScopedHandleBase(RValue other) : handle_(other.object->release()) {}
  ScopedHandleBase& operator=(RValue other) {
    handle_ = other.object->release();
    return *this;
  }

  const HandleType& get() const { return handle_; }

  void swap(ScopedHandleBase& other) {
    handle_.swap(other.handle_);
  }

  HandleType release() MOJO_WARN_UNUSED_RESULT {
    HandleType rv;
    rv.swap(handle_);
    return rv;
  }

  void reset(HandleType handle = HandleType()) {
    CloseIfNecessary();
    handle_ = handle;
  }

  bool is_valid() const {
    return handle_.is_valid();
  }

 private:
  void CloseIfNecessary() {
    if (!handle_.is_valid())
      return;
    MojoResult result MOJO_ALLOW_UNUSED = MojoClose(handle_.value());
    assert(result == MOJO_RESULT_OK);
  }

  HandleType handle_;
};

template <typename HandleType>
inline ScopedHandleBase<HandleType> MakeScopedHandle(HandleType handle) {
  return ScopedHandleBase<HandleType>(handle);
}


const MojoHandle kInvalidHandleValue = MOJO_HANDLE_INVALID;

class Handle {
 public:
  Handle() : value_(MOJO_HANDLE_INVALID) {}
  explicit Handle(MojoHandle value) : value_(value) {}
  ~Handle() {}

  void swap(Handle& other) {
    MojoHandle temp = value_;
    value_ = other.value_;
    other.value_ = temp;
  }

  bool is_valid() const {
    return value_ != MOJO_HANDLE_INVALID;
  }

  MojoHandle value() const { return value_; }
  MojoHandle* mutable_value() { return &value_; }
  void set_value(MojoHandle value) { value_ = value; }

 private:
  MojoHandle value_;

  
};

MOJO_COMPILE_ASSERT(sizeof(Handle) == sizeof(MojoHandle),
                    bad_size_for_cpp_Handle);

typedef ScopedHandleBase<Handle> ScopedHandle;
MOJO_COMPILE_ASSERT(sizeof(ScopedHandle) == sizeof(Handle),
                    bad_size_for_cpp_ScopedHandle);

inline MojoResult Wait(const Handle& handle,
                       MojoWaitFlags flags,
                       MojoDeadline deadline) {
  return MojoWait(handle.value(), flags, deadline);
}

template <class HandleVectorType, class FlagsVectorType>
inline MojoResult WaitMany(const HandleVectorType& handles,
                           const FlagsVectorType& flags,
                           MojoDeadline deadline) {
  if (flags.size() != handles.size())
    return MOJO_RESULT_INVALID_ARGUMENT;
  if (handles.size() > std::numeric_limits<uint32_t>::max())
    return MOJO_RESULT_OUT_OF_RANGE;

  if (handles.size() == 0)
    return MojoWaitMany(NULL, NULL, 0, deadline);

  const Handle& first_handle = handles[0];
  const MojoWaitFlags& first_flag = flags[0];
  return MojoWaitMany(reinterpret_cast<const MojoHandle*>(&first_handle),
                      reinterpret_cast<const MojoWaitFlags*>(&first_flag),
                      static_cast<uint32_t>(handles.size()),
                      deadline);
}

template <class HandleType>
inline void Close(ScopedHandleBase<HandleType> ) {}

inline MojoResult CloseRaw(Handle handle) {
  return MojoClose(handle.value());
}

inline bool operator<(const Handle& a, const Handle& b) {
  return a.value() < b.value();
}


class MessagePipeHandle : public Handle {
 public:
  MessagePipeHandle() {}
  explicit MessagePipeHandle(MojoHandle value) : Handle(value) {}

  
};

MOJO_COMPILE_ASSERT(sizeof(MessagePipeHandle) == sizeof(Handle),
                    bad_size_for_cpp_MessagePipeHandle);

typedef ScopedHandleBase<MessagePipeHandle> ScopedMessagePipeHandle;
MOJO_COMPILE_ASSERT(sizeof(ScopedMessagePipeHandle) ==
                        sizeof(MessagePipeHandle),
                    bad_size_for_cpp_ScopedMessagePipeHandle);

inline void CreateMessagePipe(ScopedMessagePipeHandle* message_pipe_0,
                              ScopedMessagePipeHandle* message_pipe_1) {
  assert(message_pipe_0);
  assert(message_pipe_1);
  MessagePipeHandle h_0;
  MessagePipeHandle h_1;
  MojoResult result MOJO_ALLOW_UNUSED =
      MojoCreateMessagePipe(h_0.mutable_value(), h_1.mutable_value());
  assert(result == MOJO_RESULT_OK);
  message_pipe_0->reset(h_0);
  message_pipe_1->reset(h_1);
}

inline MojoResult WriteMessageRaw(
    MessagePipeHandle message_pipe,
    const void* bytes, uint32_t num_bytes,
    const MojoHandle* handles, uint32_t num_handles,
    MojoWriteMessageFlags flags) {
  return MojoWriteMessage(message_pipe.value(),
                          bytes, num_bytes,
                          handles, num_handles,
                          flags);
}

inline MojoResult ReadMessageRaw(MessagePipeHandle message_pipe,
                                 void* bytes, uint32_t* num_bytes,
                                 MojoHandle* handles, uint32_t* num_handles,
                                 MojoReadMessageFlags flags) {
  return MojoReadMessage(message_pipe.value(),
                         bytes, num_bytes,
                         handles, num_handles,
                         flags);
}

inline MojoTimeTicks GetTimeTicksNow() {
  return MojoGetTimeTicksNow();
}

}  

#endif  
