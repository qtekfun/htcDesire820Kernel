// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_STUBS_ONCE_H__
#define GOOGLE_PROTOBUF_STUBS_ONCE_H__

#include <google/protobuf/stubs/atomicops.h>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {

#ifdef GOOGLE_PROTOBUF_NO_THREAD_SAFETY

typedef bool ProtobufOnceType;

#define GOOGLE_PROTOBUF_ONCE_INIT false

inline void GoogleOnceInit(ProtobufOnceType* once, void (*init_func)()) {
  if (!*once) {
    *once = true;
    init_func();
  }
}

template <typename Arg>
inline void GoogleOnceInit(ProtobufOnceType* once, void (*init_func)(Arg),
    Arg arg) {
  if (!*once) {
    *once = true;
    init_func(arg);
  }
}

#else

enum {
  ONCE_STATE_UNINITIALIZED = 0,
  ONCE_STATE_EXECUTING_CLOSURE = 1,
  ONCE_STATE_DONE = 2
};

typedef internal::AtomicWord ProtobufOnceType;

#define GOOGLE_PROTOBUF_ONCE_INIT ::google::protobuf::ONCE_STATE_UNINITIALIZED

LIBPROTOBUF_EXPORT
void GoogleOnceInitImpl(ProtobufOnceType* once, Closure* closure);

inline void GoogleOnceInit(ProtobufOnceType* once, void (*init_func)()) {
  if (internal::Acquire_Load(once) != ONCE_STATE_DONE) {
    internal::FunctionClosure0 func(init_func, false);
    GoogleOnceInitImpl(once, &func);
  }
}

template <typename Arg>
inline void GoogleOnceInit(ProtobufOnceType* once, void (*init_func)(Arg*),
    Arg* arg) {
  if (internal::Acquire_Load(once) != ONCE_STATE_DONE) {
    internal::FunctionClosure1<Arg*> func(init_func, false, arg);
    GoogleOnceInitImpl(once, &func);
  }
}

#endif  

#define GOOGLE_PROTOBUF_DECLARE_ONCE(NAME) \
  ::google::protobuf::ProtobufOnceType NAME = GOOGLE_PROTOBUF_ONCE_INIT

}  
}  

#endif  