// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT

#ifndef IPC_IPC_MESSAGE_MACROS_H_
#define IPC_IPC_MESSAGE_MACROS_H_

#include "base/profiler/scoped_profile.h"
#include "ipc/ipc_message_utils.h"
#include "ipc/param_traits_macros.h"

#if defined(IPC_MESSAGE_IMPL)
#include "ipc/ipc_message_utils_impl.h"
#endif

#define IPC_STRUCT_BEGIN(struct_name) \
  IPC_STRUCT_BEGIN_WITH_PARENT(struct_name, IPC::NoParams)

#define IPC_STRUCT_BEGIN_WITH_PARENT(struct_name, parent) \
  struct struct_name; \
  IPC_STRUCT_TRAITS_BEGIN(struct_name) \
  IPC_STRUCT_TRAITS_END() \
  struct IPC_MESSAGE_EXPORT struct_name : parent { \
    struct_name(); \
    ~struct_name();
#define IPC_STRUCT_MEMBER(type, name, ...) type name;
#define IPC_STRUCT_END() };

#define IPC_MESSAGE_CONTROL0(msg_class) \
  IPC_MESSAGE_DECL(EMPTY, CONTROL, msg_class, 0, 0, (), ())

#define IPC_MESSAGE_CONTROL1(msg_class, type1) \
  IPC_MESSAGE_DECL(ASYNC, CONTROL, msg_class, 1, 0, (type1), ())

#define IPC_MESSAGE_CONTROL2(msg_class, type1, type2) \
  IPC_MESSAGE_DECL(ASYNC, CONTROL, msg_class, 2, 0, (type1, type2), ())

#define IPC_MESSAGE_CONTROL3(msg_class, type1, type2, type3) \
  IPC_MESSAGE_DECL(ASYNC, CONTROL, msg_class, 3, 0, (type1, type2, type3), ())

#define IPC_MESSAGE_CONTROL4(msg_class, type1, type2, type3, type4) \
  IPC_MESSAGE_DECL(ASYNC, CONTROL, msg_class, 4, 0, (type1, type2, type3, type4), ())

#define IPC_MESSAGE_CONTROL5(msg_class, type1, type2, type3, type4, type5) \
  IPC_MESSAGE_DECL(ASYNC, CONTROL, msg_class, 5, 0, (type1, type2, type3, type4, type5), ())

#define IPC_MESSAGE_ROUTED0(msg_class) \
  IPC_MESSAGE_DECL(EMPTY, ROUTED, msg_class, 0, 0, (), ())

#define IPC_MESSAGE_ROUTED1(msg_class, type1) \
  IPC_MESSAGE_DECL(ASYNC, ROUTED, msg_class, 1, 0, (type1), ())

#define IPC_MESSAGE_ROUTED2(msg_class, type1, type2) \
  IPC_MESSAGE_DECL(ASYNC, ROUTED, msg_class, 2, 0, (type1, type2), ())

#define IPC_MESSAGE_ROUTED3(msg_class, type1, type2, type3) \
  IPC_MESSAGE_DECL(ASYNC, ROUTED, msg_class, 3, 0, (type1, type2, type3), ())

#define IPC_MESSAGE_ROUTED4(msg_class, type1, type2, type3, type4) \
  IPC_MESSAGE_DECL(ASYNC, ROUTED, msg_class, 4, 0, (type1, type2, type3, type4), ())

#define IPC_MESSAGE_ROUTED5(msg_class, type1, type2, type3, type4, type5) \
  IPC_MESSAGE_DECL(ASYNC, ROUTED, msg_class, 5, 0, (type1, type2, type3, type4, type5), ())

#define IPC_SYNC_MESSAGE_CONTROL0_0(msg_class) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 0, 0, (), ())

#define IPC_SYNC_MESSAGE_CONTROL0_1(msg_class, type1_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 0, 1, (), (type1_out))

#define IPC_SYNC_MESSAGE_CONTROL0_2(msg_class, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 0, 2, (), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_CONTROL0_3(msg_class, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 0, 3, (), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_CONTROL0_4(msg_class, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 0, 4, (), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_CONTROL1_0(msg_class, type1_in) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 1, 0, (type1_in), ())

#define IPC_SYNC_MESSAGE_CONTROL1_1(msg_class, type1_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 1, 1, (type1_in), (type1_out))

#define IPC_SYNC_MESSAGE_CONTROL1_2(msg_class, type1_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 1, 2, (type1_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_CONTROL1_3(msg_class, type1_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 1, 3, (type1_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_CONTROL1_4(msg_class, type1_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 1, 4, (type1_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_CONTROL2_0(msg_class, type1_in, type2_in) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 2, 0, (type1_in, type2_in), ())

#define IPC_SYNC_MESSAGE_CONTROL2_1(msg_class, type1_in, type2_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 2, 1, (type1_in, type2_in), (type1_out))

#define IPC_SYNC_MESSAGE_CONTROL2_2(msg_class, type1_in, type2_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 2, 2, (type1_in, type2_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_CONTROL2_3(msg_class, type1_in, type2_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 2, 3, (type1_in, type2_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_CONTROL2_4(msg_class, type1_in, type2_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 2, 4, (type1_in, type2_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_CONTROL3_0(msg_class, type1_in, type2_in, type3_in) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 3, 0, (type1_in, type2_in, type3_in), ())

#define IPC_SYNC_MESSAGE_CONTROL3_1(msg_class, type1_in, type2_in, type3_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 3, 1, (type1_in, type2_in, type3_in), (type1_out))

#define IPC_SYNC_MESSAGE_CONTROL3_2(msg_class, type1_in, type2_in, type3_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 3, 2, (type1_in, type2_in, type3_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_CONTROL3_3(msg_class, type1_in, type2_in, type3_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 3, 3, (type1_in, type2_in, type3_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_CONTROL3_4(msg_class, type1_in, type2_in, type3_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 3, 4, (type1_in, type2_in, type3_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_CONTROL4_0(msg_class, type1_in, type2_in, type3_in, type4_in) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 4, 0, (type1_in, type2_in, type3_in, type4_in), ())

#define IPC_SYNC_MESSAGE_CONTROL4_1(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 4, 1, (type1_in, type2_in, type3_in, type4_in), (type1_out))

#define IPC_SYNC_MESSAGE_CONTROL4_2(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 4, 2, (type1_in, type2_in, type3_in, type4_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_CONTROL4_3(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 4, 3, (type1_in, type2_in, type3_in, type4_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_CONTROL4_4(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 4, 4, (type1_in, type2_in, type3_in, type4_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_CONTROL5_0(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 5, 0, (type1_in, type2_in, type3_in, type4_in, type5_in), ())

#define IPC_SYNC_MESSAGE_CONTROL5_1(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 5, 1, (type1_in, type2_in, type3_in, type4_in, type5_in), (type1_out))

#define IPC_SYNC_MESSAGE_CONTROL5_2(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 5, 2, (type1_in, type2_in, type3_in, type4_in, type5_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_CONTROL5_3(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, CONTROL, msg_class, 5, 3, (type1_in, type2_in, type3_in, type4_in, type5_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_ROUTED0_0(msg_class) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 0, 0, (), ())

#define IPC_SYNC_MESSAGE_ROUTED0_1(msg_class, type1_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 0, 1, (), (type1_out))

#define IPC_SYNC_MESSAGE_ROUTED0_2(msg_class, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 0, 2, (), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_ROUTED0_3(msg_class, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 0, 3, (), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_ROUTED0_4(msg_class, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 0, 4, (), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_ROUTED1_0(msg_class, type1_in) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 1, 0, (type1_in), ())

#define IPC_SYNC_MESSAGE_ROUTED1_1(msg_class, type1_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 1, 1, (type1_in), (type1_out))

#define IPC_SYNC_MESSAGE_ROUTED1_2(msg_class, type1_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 1, 2, (type1_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_ROUTED1_3(msg_class, type1_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 1, 3, (type1_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_ROUTED1_4(msg_class, type1_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 1, 4, (type1_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_ROUTED2_0(msg_class, type1_in, type2_in) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 2, 0, (type1_in, type2_in), ())

#define IPC_SYNC_MESSAGE_ROUTED2_1(msg_class, type1_in, type2_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 2, 1, (type1_in, type2_in), (type1_out))

#define IPC_SYNC_MESSAGE_ROUTED2_2(msg_class, type1_in, type2_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 2, 2, (type1_in, type2_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_ROUTED2_3(msg_class, type1_in, type2_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 2, 3, (type1_in, type2_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_ROUTED2_4(msg_class, type1_in, type2_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 2, 4, (type1_in, type2_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_ROUTED3_0(msg_class, type1_in, type2_in, type3_in) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 3, 0, (type1_in, type2_in, type3_in), ())

#define IPC_SYNC_MESSAGE_ROUTED3_1(msg_class, type1_in, type2_in, type3_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 3, 1, (type1_in, type2_in, type3_in), (type1_out))

#define IPC_SYNC_MESSAGE_ROUTED3_2(msg_class, type1_in, type2_in, type3_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 3, 2, (type1_in, type2_in, type3_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_ROUTED3_3(msg_class, type1_in, type2_in, type3_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 3, 3, (type1_in, type2_in, type3_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_ROUTED3_4(msg_class, type1_in, type2_in, type3_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 3, 4, (type1_in, type2_in, type3_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_ROUTED4_0(msg_class, type1_in, type2_in, type3_in, type4_in) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 4, 0, (type1_in, type2_in, type3_in, type4_in), ())

#define IPC_SYNC_MESSAGE_ROUTED4_1(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 4, 1, (type1_in, type2_in, type3_in, type4_in), (type1_out))

#define IPC_SYNC_MESSAGE_ROUTED4_2(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 4, 2, (type1_in, type2_in, type3_in, type4_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_ROUTED4_3(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 4, 3, (type1_in, type2_in, type3_in, type4_in), (type1_out, type2_out, type3_out))

#define IPC_SYNC_MESSAGE_ROUTED4_4(msg_class, type1_in, type2_in, type3_in, type4_in, type1_out, type2_out, type3_out, type4_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 4, 4, (type1_in, type2_in, type3_in, type4_in), (type1_out, type2_out, type3_out, type4_out))

#define IPC_SYNC_MESSAGE_ROUTED5_0(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 5, 0, (type1_in, type2_in, type3_in, type4_in, type5_in), ())

#define IPC_SYNC_MESSAGE_ROUTED5_1(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in, type1_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 5, 1, (type1_in, type2_in, type3_in, type4_in, type5_in), (type1_out))

#define IPC_SYNC_MESSAGE_ROUTED5_2(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in, type1_out, type2_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 5, 2, (type1_in, type2_in, type3_in, type4_in, type5_in), (type1_out, type2_out))

#define IPC_SYNC_MESSAGE_ROUTED5_3(msg_class, type1_in, type2_in, type3_in, type4_in, type5_in, type1_out, type2_out, type3_out) \
  IPC_MESSAGE_DECL(SYNC, ROUTED, msg_class, 5, 3, (type1_in, type2_in, type3_in, type4_in, type5_in), (type1_out, type2_out, type3_out))

#define IPC_ASYNC_MESSAGE_METHODS_GENERIC                                     \
  template<class T, class S, class Method>                                    \
  static bool Dispatch(const Message* msg, T* obj, S* sender, Method func) {  \
    Schema::Param p;                                                          \
    if (Read(msg, &p)) {                                                      \
      DispatchToMethod(obj, func, p);                                         \
      return true;                                                            \
    }                                                                         \
    return false;                                                             \
  }
#define IPC_ASYNC_MESSAGE_METHODS_1                                           \
  IPC_ASYNC_MESSAGE_METHODS_GENERIC                                           \
  template<class T, class S, typename TA>                                     \
  static bool Dispatch(const Message* msg, T* obj, S* sender,                 \
                       void (T::*func)(const Message&, TA)) {                 \
    Schema::Param p;                                                          \
    if (Read(msg, &p)) {                                                      \
      (obj->*func)(*msg, p.a);                                                \
      return true;                                                            \
    }                                                                         \
    return false;                                                             \
  }
#define IPC_ASYNC_MESSAGE_METHODS_2                                           \
  IPC_ASYNC_MESSAGE_METHODS_GENERIC                                           \
  template<class T, class S, typename TA, typename TB>                        \
  static bool Dispatch(const Message* msg, T* obj, S* sender,                 \
                       void (T::*func)(const Message&, TA, TB)) {             \
    Schema::Param p;                                                          \
    if (Read(msg, &p)) {                                                      \
      (obj->*func)(*msg, p.a, p.b);                                           \
      return true;                                                            \
    }                                                                         \
    return false;                                                             \
  }                                                                           \
  template<typename TA, typename TB>                                          \
  static bool Read(const IPC::Message* msg, TA* a, TB* b) {                   \
    Schema::Param p;                                                          \
    if (!Read(msg, &p))                                                       \
      return false;                                                           \
    *a = p.a;                                                                 \
    *b = p.b;                                                                 \
    return true;                                                              \
  }
#define IPC_ASYNC_MESSAGE_METHODS_3                                           \
  IPC_ASYNC_MESSAGE_METHODS_GENERIC                                           \
  template<class T, class S, typename TA, typename TB, typename TC>           \
  static bool Dispatch(const Message* msg, T* obj, S* sender,                 \
                       void (T::*func)(const Message&, TA, TB, TC)) {         \
    Schema::Param p;                                                          \
    if (Read(msg, &p)) {                                                      \
      (obj->*func)(*msg, p.a, p.b, p.c);                                      \
      return true;                                                            \
    }                                                                         \
    return false;                                                             \
  }                                                                           \
  template<typename TA, typename TB, typename TC>                             \
  static bool Read(const IPC::Message* msg, TA* a, TB* b, TC* c) {            \
    Schema::Param p;                                                          \
    if (!Read(msg, &p))                                                       \
      return false;                                                           \
    *a = p.a;                                                                 \
    *b = p.b;                                                                 \
    *c = p.c;                                                                 \
    return true;                                                              \
  }
#define IPC_ASYNC_MESSAGE_METHODS_4                                           \
  IPC_ASYNC_MESSAGE_METHODS_GENERIC                                           \
  template<class T, class S, typename TA, typename TB, typename TC,           \
           typename TD>                                                       \
  static bool Dispatch(const Message* msg, T* obj, S* sender,                 \
                       void (T::*func)(const Message&, TA, TB, TC, TD)) {     \
    Schema::Param p;                                                          \
    if (Read(msg, &p)) {                                                      \
      (obj->*func)(*msg, p.a, p.b, p.c, p.d);                                 \
      return true;                                                            \
    }                                                                         \
    return false;                                                             \
  }                                                                           \
  template<typename TA, typename TB, typename TC, typename TD>                \
  static bool Read(const IPC::Message* msg, TA* a, TB* b, TC* c, TD* d) {     \
    Schema::Param p;                                                          \
    if (!Read(msg, &p))                                                       \
      return false;                                                           \
    *a = p.a;                                                                 \
    *b = p.b;                                                                 \
    *c = p.c;                                                                 \
    *d = p.d;                                                                 \
    return true;                                                              \
  }
#define IPC_ASYNC_MESSAGE_METHODS_5                                           \
  IPC_ASYNC_MESSAGE_METHODS_GENERIC                                           \
  template<class T, class S, typename TA, typename TB, typename TC,           \
           typename TD, typename TE>                                          \
  static bool Dispatch(const Message* msg, T* obj, S* sender,                 \
                       void (T::*func)(const Message&, TA, TB, TC, TD, TE)) { \
    Schema::Param p;                                                          \
    if (Read(msg, &p)) {                                                      \
      (obj->*func)(*msg, p.a, p.b, p.c, p.d, p.e);                            \
      return true;                                                            \
    }                                                                         \
    return false;                                                             \
  }                                                                           \
  template<typename TA, typename TB, typename TC, typename TD, typename TE>   \
  static bool Read(const IPC::Message* msg, TA* a, TB* b, TC* c, TD* d,       \
                   TE* e) {                                                   \
    Schema::Param p;                                                          \
    if (!Read(msg, &p))                                                       \
      return false;                                                           \
    *a = p.a;                                                                 \
    *b = p.b;                                                                 \
    *c = p.c;                                                                 \
    *d = p.d;                                                                 \
    *e = p.e;                                                                 \
    return true;                                                              \
  }

#define IPC_SYNC_MESSAGE_METHODS_GENERIC                                      \
  template<class T, class S, class Method>                                    \
  static bool Dispatch(const Message* msg, T* obj, S* sender, Method func) {  \
    Schema::SendParam send_params;                                            \
    bool ok = ReadSendParam(msg, &send_params);                               \
    return Schema::DispatchWithSendParams(ok, send_params, msg, obj, sender,  \
                                          func);                              \
  }                                                                           \
  template<class T, class Method>                                             \
  static bool DispatchDelayReply(const Message* msg, T* obj, Method func) {   \
    Schema::SendParam send_params;                                            \
    bool ok = ReadSendParam(msg, &send_params);                               \
    return Schema::DispatchDelayReplyWithSendParams(ok, send_params, msg,     \
                                                    obj, func);               \
  }
#define IPC_SYNC_MESSAGE_METHODS_0 \
  IPC_SYNC_MESSAGE_METHODS_GENERIC
#define IPC_SYNC_MESSAGE_METHODS_1 \
  IPC_SYNC_MESSAGE_METHODS_GENERIC \
  template<typename TA> \
  static void WriteReplyParams(Message* reply, TA a) { \
    Schema::WriteReplyParams(reply, a); \
  }
#define IPC_SYNC_MESSAGE_METHODS_2 \
  IPC_SYNC_MESSAGE_METHODS_GENERIC \
  template<typename TA, typename TB> \
  static void WriteReplyParams(Message* reply, TA a, TB b) { \
    Schema::WriteReplyParams(reply, a, b); \
  }
#define IPC_SYNC_MESSAGE_METHODS_3 \
  IPC_SYNC_MESSAGE_METHODS_GENERIC \
  template<typename TA, typename TB, typename TC> \
  static void WriteReplyParams(Message* reply, TA a, TB b, TC c) { \
    Schema::WriteReplyParams(reply, a, b, c); \
  }
#define IPC_SYNC_MESSAGE_METHODS_4 \
  IPC_SYNC_MESSAGE_METHODS_GENERIC \
  template<typename TA, typename TB, typename TC, typename TD> \
  static void WriteReplyParams(Message* reply, TA a, TB b, TC c, TD d) { \
    Schema::WriteReplyParams(reply, a, b, c, d); \
  }
#define IPC_SYNC_MESSAGE_METHODS_5 \
  IPC_SYNC_MESSAGE_METHODS_GENERIC \
  template<typename TA, typename TB, typename TC, typename TD, typename TE> \
  static void WriteReplyParams(Message* reply, TA a, TB b, TC c, TD d, TE e) { \
    Schema::WriteReplyParams(reply, a, b, c, d, e); \
  }

#define IPC_MESSAGE_DECL(sync, kind, msg_class,                               \
                         in_cnt, out_cnt, in_list, out_list)                  \
  IPC_##sync##_##kind##_DECL(msg_class, in_cnt, out_cnt, in_list, out_list)   \
  IPC_MESSAGE_EXTRA(sync, kind, msg_class, in_cnt, out_cnt, in_list, out_list)

#define IPC_EMPTY_CONTROL_DECL(msg_class, in_cnt, out_cnt, in_list, out_list) \
  class IPC_MESSAGE_EXPORT msg_class : public IPC::Message {                  \
   public:                                                                    \
    typedef IPC::Message Schema;                                              \
    enum { ID = IPC_MESSAGE_ID() };                                           \
    msg_class() : IPC::Message(MSG_ROUTING_CONTROL, ID, PRIORITY_NORMAL) {}   \
    static void Log(std::string* name, const Message* msg, std::string* l);   \
  };

#define IPC_EMPTY_ROUTED_DECL(msg_class, in_cnt, out_cnt, in_list, out_list)  \
  class IPC_MESSAGE_EXPORT msg_class : public IPC::Message {                  \
   public:                                                                    \
    typedef IPC::Message Schema;                                              \
    enum { ID = IPC_MESSAGE_ID() };                                           \
    msg_class(int32 routing_id)                                               \
        : IPC::Message(routing_id, ID, PRIORITY_NORMAL) {}                    \
    static void Log(std::string* name, const Message* msg, std::string* l);   \
  };

#define IPC_ASYNC_CONTROL_DECL(msg_class, in_cnt, out_cnt, in_list, out_list) \
  class IPC_MESSAGE_EXPORT msg_class : public IPC::Message {                  \
   public:                                                                    \
    typedef IPC::MessageSchema<IPC_TUPLE_IN_##in_cnt in_list> Schema;         \
    typedef Schema::Param Param;                                              \
    enum { ID = IPC_MESSAGE_ID() };                                           \
    msg_class(IPC_TYPE_IN_##in_cnt in_list);                                  \
    virtual ~msg_class();                                                     \
    static bool Read(const Message* msg, Schema::Param* p);                   \
    static void Log(std::string* name, const Message* msg, std::string* l);   \
    IPC_ASYNC_MESSAGE_METHODS_##in_cnt                                        \
  };

#define IPC_ASYNC_ROUTED_DECL(msg_class, in_cnt, out_cnt, in_list, out_list)  \
  class IPC_MESSAGE_EXPORT msg_class : public IPC::Message {                  \
   public:                                                                    \
    typedef IPC::MessageSchema<IPC_TUPLE_IN_##in_cnt in_list> Schema;         \
    typedef Schema::Param Param;                                              \
    enum { ID = IPC_MESSAGE_ID() };                                           \
    msg_class(int32 routing_id IPC_COMMA_##in_cnt                             \
              IPC_TYPE_IN_##in_cnt in_list);                                  \
    virtual ~msg_class();                                                     \
    static bool Read(const Message* msg, Schema::Param* p);                   \
    static void Log(std::string* name, const Message* msg, std::string* l);   \
    IPC_ASYNC_MESSAGE_METHODS_##in_cnt                                        \
  };

#define IPC_SYNC_CONTROL_DECL(msg_class, in_cnt, out_cnt, in_list, out_list)  \
  class IPC_MESSAGE_EXPORT msg_class : public IPC::SyncMessage {              \
   public:                                                                    \
    typedef IPC::SyncMessageSchema<IPC_TUPLE_IN_##in_cnt in_list,             \
                                   IPC_TUPLE_OUT_##out_cnt out_list> Schema;  \
    typedef Schema::ReplyParam ReplyParam;                                    \
    typedef Schema::SendParam SendParam;                                      \
    enum { ID = IPC_MESSAGE_ID() };                                           \
    msg_class(IPC_TYPE_IN_##in_cnt in_list                                    \
              IPC_COMMA_AND_##in_cnt(IPC_COMMA_##out_cnt)                     \
              IPC_TYPE_OUT_##out_cnt out_list);                               \
    virtual ~msg_class();                                                     \
    static bool ReadSendParam(const Message* msg, Schema::SendParam* p);      \
    static bool ReadReplyParam(                                               \
        const Message* msg,                                                   \
        TupleTypes<ReplyParam>::ValueTuple* p);                               \
    static void Log(std::string* name, const Message* msg, std::string* l);   \
    IPC_SYNC_MESSAGE_METHODS_##out_cnt                                        \
  };

#define IPC_SYNC_ROUTED_DECL(msg_class, in_cnt, out_cnt, in_list, out_list)   \
  class IPC_MESSAGE_EXPORT msg_class : public IPC::SyncMessage {              \
   public:                                                                    \
    typedef IPC::SyncMessageSchema<IPC_TUPLE_IN_##in_cnt in_list,             \
                                   IPC_TUPLE_OUT_##out_cnt out_list> Schema;  \
    typedef Schema::ReplyParam ReplyParam;                                    \
    typedef Schema::SendParam SendParam;                                      \
    enum { ID = IPC_MESSAGE_ID() };                                           \
    msg_class(int32 routing_id                                                \
              IPC_COMMA_OR_##in_cnt(IPC_COMMA_##out_cnt)                      \
              IPC_TYPE_IN_##in_cnt in_list                                    \
              IPC_COMMA_AND_##in_cnt(IPC_COMMA_##out_cnt)                     \
              IPC_TYPE_OUT_##out_cnt out_list);                               \
    virtual ~msg_class();                                                     \
    static bool ReadSendParam(const Message* msg, Schema::SendParam* p);      \
    static bool ReadReplyParam(                                               \
        const Message* msg,                                                   \
        TupleTypes<ReplyParam>::ValueTuple* p);                               \
    static void Log(std::string* name, const Message* msg, std::string* l);   \
    IPC_SYNC_MESSAGE_METHODS_##out_cnt                                        \
  };

#if defined(IPC_MESSAGE_IMPL)

#define IPC_MESSAGE_EXTRA(sync, kind, msg_class,                              \
                          in_cnt, out_cnt, in_list, out_list)                 \
  IPC_##sync##_##kind##_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list)   \
  IPC_##sync##_MESSAGE_LOG(msg_class)

#define IPC_EMPTY_CONTROL_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list)
#define IPC_EMPTY_ROUTED_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list)

#define IPC_ASYNC_CONTROL_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list) \
  msg_class::msg_class(IPC_TYPE_IN_##in_cnt in_list) :                        \
      IPC::Message(MSG_ROUTING_CONTROL, ID, PRIORITY_NORMAL) {                \
        Schema::Write(this, IPC_NAME_IN_##in_cnt in_list);                    \
      }                                                                       \
  msg_class::~msg_class() {}                                                  \
  bool msg_class::Read(const Message* msg, Schema::Param* p) {                \
    return Schema::Read(msg, p);                                              \
  }

#define IPC_ASYNC_ROUTED_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list)  \
  msg_class::msg_class(int32 routing_id IPC_COMMA_##in_cnt                    \
                       IPC_TYPE_IN_##in_cnt in_list) :                        \
      IPC::Message(routing_id, ID, PRIORITY_NORMAL) {                         \
        Schema::Write(this, IPC_NAME_IN_##in_cnt in_list);                    \
      }                                                                       \
  msg_class::~msg_class() {}                                                  \
  bool msg_class::Read(const Message* msg, Schema::Param* p) {                \
    return Schema::Read(msg, p);                                              \
  }

#define IPC_SYNC_CONTROL_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list)  \
  msg_class::msg_class(IPC_TYPE_IN_##in_cnt in_list                           \
                       IPC_COMMA_AND_##in_cnt(IPC_COMMA_##out_cnt)            \
                       IPC_TYPE_OUT_##out_cnt out_list) :                     \
      IPC::SyncMessage(MSG_ROUTING_CONTROL, ID, PRIORITY_NORMAL,              \
                       new IPC::ParamDeserializer<Schema::ReplyParam>(        \
                           IPC_NAME_OUT_##out_cnt out_list)) {                \
        Schema::Write(this, IPC_NAME_IN_##in_cnt in_list);                    \
      }                                                                       \
  msg_class::~msg_class() {}                                                  \
  bool msg_class::ReadSendParam(const Message* msg, Schema::SendParam* p) {   \
    return Schema::ReadSendParam(msg, p);                                     \
  }                                                                           \
  bool msg_class::ReadReplyParam(const Message* msg,                          \
                                 TupleTypes<ReplyParam>::ValueTuple* p) {     \
    return Schema::ReadReplyParam(msg, p);                                    \
  }

#define IPC_SYNC_ROUTED_IMPL(msg_class, in_cnt, out_cnt, in_list, out_list)   \
  msg_class::msg_class(int32 routing_id                                       \
                       IPC_COMMA_OR_##in_cnt(IPC_COMMA_##out_cnt)             \
                       IPC_TYPE_IN_##in_cnt in_list                           \
                       IPC_COMMA_AND_##in_cnt(IPC_COMMA_##out_cnt)            \
                       IPC_TYPE_OUT_##out_cnt out_list) :                     \
      IPC::SyncMessage(routing_id, ID, PRIORITY_NORMAL,                       \
                       new IPC::ParamDeserializer<Schema::ReplyParam>(        \
                           IPC_NAME_OUT_##out_cnt out_list)) {                \
        Schema::Write(this, IPC_NAME_IN_##in_cnt in_list);                    \
      }                                                                       \
  msg_class::~msg_class() {}                                                  \
  bool msg_class::ReadSendParam(const Message* msg, Schema::SendParam* p) {   \
    return Schema::ReadSendParam(msg, p);                                     \
  }                                                                           \
  bool msg_class::ReadReplyParam(const Message* msg,                          \
                                 TupleTypes<ReplyParam>::ValueTuple* p) {     \
    return Schema::ReadReplyParam(msg, p);                                    \
  }

#define IPC_EMPTY_MESSAGE_LOG(msg_class)                                \
  void msg_class::Log(std::string* name,                                \
                      const Message* msg,                               \
                      std::string* l) {                                 \
    if (name)                                                           \
      *name = #msg_class;                                               \
  }

#define IPC_ASYNC_MESSAGE_LOG(msg_class)                                \
  void msg_class::Log(std::string* name,                                \
                      const Message* msg,                               \
                      std::string* l) {                                 \
    if (name)                                                           \
      *name = #msg_class;                                               \
    if (!msg || !l)                                                     \
      return;                                                           \
    Schema::Param p;                                                    \
    if (Schema::Read(msg, &p))                                          \
      IPC::LogParam(p, l);                                              \
  }

#define IPC_SYNC_MESSAGE_LOG(msg_class)                                 \
  void msg_class::Log(std::string* name,                                \
                      const Message* msg,                               \
                      std::string* l) {                                 \
    if (name)                                                           \
      *name = #msg_class;                                               \
    if (!msg || !l)                                                     \
      return;                                                           \
    if (msg->is_sync()) {                                               \
      TupleTypes<Schema::SendParam>::ValueTuple p;                      \
      if (Schema::ReadSendParam(msg, &p))                               \
        IPC::LogParam(p, l);                                            \
      AddOutputParamsToLog(msg, l);                                     \
    } else {                                                            \
      TupleTypes<Schema::ReplyParam>::ValueTuple p;                     \
      if (Schema::ReadReplyParam(msg, &p))                              \
        IPC::LogParam(p, l);                                            \
    }                                                                   \
  }

#elif defined(IPC_MESSAGE_MACROS_LOG_ENABLED)

#ifndef IPC_LOG_TABLE_ADD_ENTRY
#error You need to define IPC_LOG_TABLE_ADD_ENTRY(msg_id, logger)
#endif

#define IPC_MESSAGE_EXTRA(sync, kind, msg_class,                        \
                          in_cnt, out_cnt, in_list, out_list)           \
  class LoggerRegisterHelper##msg_class {                               \
 public:                                                                \
    LoggerRegisterHelper##msg_class() {                                 \
      const uint32 msg_id = static_cast<uint32>(msg_class::ID);         \
      IPC_LOG_TABLE_ADD_ENTRY(msg_id, msg_class::Log);                  \
    }                                                                   \
  };                                                                    \
  LoggerRegisterHelper##msg_class g_LoggerRegisterHelper##msg_class;

#else

#define IPC_MESSAGE_EXTRA(sync, kind, msg_class,                \
                          in_cnt, out_cnt, in_list, out_list)

#endif 

#define IPC_TYPE_IN_0()
#define IPC_TYPE_IN_1(t1)                   const t1& arg1
#define IPC_TYPE_IN_2(t1, t2)               const t1& arg1, const t2& arg2
#define IPC_TYPE_IN_3(t1, t2, t3)           const t1& arg1, const t2& arg2, const t3& arg3
#define IPC_TYPE_IN_4(t1, t2, t3, t4)       const t1& arg1, const t2& arg2, const t3& arg3, const t4& arg4
#define IPC_TYPE_IN_5(t1, t2, t3, t4, t5)   const t1& arg1, const t2& arg2, const t3& arg3, const t4& arg4, const t5& arg5

#define IPC_TYPE_OUT_0()
#define IPC_TYPE_OUT_1(t1)                  t1* arg6
#define IPC_TYPE_OUT_2(t1, t2)              t1* arg6, t2* arg7
#define IPC_TYPE_OUT_3(t1, t2, t3)          t1* arg6, t2* arg7, t3* arg8
#define IPC_TYPE_OUT_4(t1, t2, t3, t4)      t1* arg6, t2* arg7, t3* arg8, t4* arg9

#define IPC_TUPLE_IN_0()                    Tuple0
#define IPC_TUPLE_IN_1(t1)                  Tuple1<t1>
#define IPC_TUPLE_IN_2(t1, t2)              Tuple2<t1, t2>
#define IPC_TUPLE_IN_3(t1, t2, t3)          Tuple3<t1, t2, t3>
#define IPC_TUPLE_IN_4(t1, t2, t3, t4)      Tuple4<t1, t2, t3, t4>
#define IPC_TUPLE_IN_5(t1, t2, t3, t4, t5)  Tuple5<t1, t2, t3, t4, t5>

#define IPC_TUPLE_OUT_0()                   Tuple0
#define IPC_TUPLE_OUT_1(t1)                 Tuple1<t1&>
#define IPC_TUPLE_OUT_2(t1, t2)             Tuple2<t1&, t2&>
#define IPC_TUPLE_OUT_3(t1, t2, t3)         Tuple3<t1&, t2&, t3&>
#define IPC_TUPLE_OUT_4(t1, t2, t3, t4)     Tuple4<t1&, t2&, t3&, t4&>

#define IPC_NAME_IN_0()                     MakeTuple()
#define IPC_NAME_IN_1(t1)                   MakeRefTuple(arg1)
#define IPC_NAME_IN_2(t1, t2)               MakeRefTuple(arg1, arg2)
#define IPC_NAME_IN_3(t1, t2, t3)           MakeRefTuple(arg1, arg2, arg3)
#define IPC_NAME_IN_4(t1, t2, t3, t4)       MakeRefTuple(arg1, arg2, arg3, arg4)
#define IPC_NAME_IN_5(t1, t2, t3, t4, t5)   MakeRefTuple(arg1, arg2, arg3, arg4, arg5)

#define IPC_NAME_OUT_0()                    MakeTuple()
#define IPC_NAME_OUT_1(t1)                  MakeRefTuple(*arg6)
#define IPC_NAME_OUT_2(t1, t2)              MakeRefTuple(*arg6, *arg7)
#define IPC_NAME_OUT_3(t1, t2, t3)          MakeRefTuple(*arg6, *arg7, *arg8)
#define IPC_NAME_OUT_4(t1, t2, t3, t4)      MakeRefTuple(*arg6, *arg7, *arg8, *arg9)

#define IPC_COMMA_0
#define IPC_COMMA_1 ,
#define IPC_COMMA_2 ,
#define IPC_COMMA_3 ,
#define IPC_COMMA_4 ,
#define IPC_COMMA_5 ,

#define IPC_COMMA_AND_0(x)
#define IPC_COMMA_AND_1(x) x
#define IPC_COMMA_AND_2(x) x
#define IPC_COMMA_AND_3(x) x
#define IPC_COMMA_AND_4(x) x
#define IPC_COMMA_AND_5(x) x

#define IPC_COMMA_OR_0(x) x
#define IPC_COMMA_OR_1(x) ,
#define IPC_COMMA_OR_2(x) ,
#define IPC_COMMA_OR_3(x) ,
#define IPC_COMMA_OR_4(x) ,
#define IPC_COMMA_OR_5(x) ,

#define IPC_MESSAGE_ID() ((IPC_MESSAGE_START << 16) + __LINE__)
#define IPC_MESSAGE_ID_CLASS(id) ((id) >> 16)
#define IPC_MESSAGE_ID_LINE(id) ((id) & 0xffff)



#define IPC_BEGIN_MESSAGE_MAP_EX(class_name, msg, msg_is_ok) \
  { \
    typedef class_name _IpcMessageHandlerClass; \
    const IPC::Message& ipc_message__ = msg; \
    bool& msg_is_ok__ = msg_is_ok; \
    switch (ipc_message__.type()) { \

#define IPC_BEGIN_MESSAGE_MAP(class_name, msg) \
  { \
    typedef class_name _IpcMessageHandlerClass; \
    const IPC::Message& ipc_message__ = msg; \
    bool msg_is_ok__ = true; \
    switch (ipc_message__.type()) { \

#define IPC_MESSAGE_FORWARD(msg_class, obj, member_func)                       \
    case msg_class::ID: {                                                      \
        TRACK_RUN_IN_IPC_HANDLER(member_func);                                 \
        msg_is_ok__ = msg_class::Dispatch(&ipc_message__, obj, this,           \
                                      &member_func);                           \
      }                                                                        \
      break;

#define IPC_MESSAGE_HANDLER(msg_class, member_func) \
  IPC_MESSAGE_FORWARD(msg_class, this, _IpcMessageHandlerClass::member_func)

#define IPC_MESSAGE_FORWARD_DELAY_REPLY(msg_class, obj, member_func)           \
    case msg_class::ID: {                                                      \
        TRACK_RUN_IN_IPC_HANDLER(member_func);                                 \
        msg_is_ok__ = msg_class::DispatchDelayReply(&ipc_message__, obj,       \
                                                  &member_func);               \
      }                                                                        \
      break;

#define IPC_MESSAGE_HANDLER_DELAY_REPLY(msg_class, member_func)                \
    IPC_MESSAGE_FORWARD_DELAY_REPLY(msg_class, this,                           \
                                    _IpcMessageHandlerClass::member_func)

#define IPC_MESSAGE_HANDLER_GENERIC(msg_class, code)                           \
    case msg_class::ID: {                                                      \
                               \
        code;                                                                  \
      }                                                                        \
      break;

#define IPC_REPLY_HANDLER(func)                                                \
    case IPC_REPLY_ID: {                                                       \
        TRACK_RUN_IN_IPC_HANDLER(func);                                        \
        func(ipc_message__);                                                   \
      }                                                                        \
      break;


#define IPC_MESSAGE_UNHANDLED(code)                                            \
    default: {                                                                 \
        code;                                                                  \
      }                                                                        \
      break;

#define IPC_MESSAGE_UNHANDLED_ERROR() \
  IPC_MESSAGE_UNHANDLED(NOTREACHED() << \
                              "Invalid message with type = " << \
                              ipc_message__.type())

#define IPC_END_MESSAGE_MAP() \
  } \
  DCHECK(msg_is_ok__); \
}

#define IPC_END_MESSAGE_MAP_EX() \
  } \
}

#define IPC_MESSAGE_CLASS(message) \
  IPC_MESSAGE_ID_CLASS(message.type())

#endif  

#ifdef IPC_MESSAGE_START
#undef IPC_MESSAGE_START
#endif
