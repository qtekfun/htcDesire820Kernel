// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_MESSAGE_UTILS_H_
#define IPC_IPC_MESSAGE_UTILS_H_

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/format_macros.h"
#include "base/memory/scoped_vector.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/tuple.h"
#include "ipc/ipc_message_start.h"
#include "ipc/ipc_param_traits.h"
#include "ipc/ipc_sync_message.h"

#if defined(COMPILER_GCC)
#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) >= 40500
#define IPC_MSG_NOINLINE  __attribute__((noinline, noclone));
#else
#define IPC_MSG_NOINLINE  __attribute__((noinline));
#endif
#elif defined(COMPILER_MSVC)
#define IPC_MSG_NOINLINE
#else
#error "Please add the noinline property for your new compiler here."
#endif

namespace base {
class DictionaryValue;
class FilePath;
class ListValue;
class NullableString16;
class Time;
class TimeDelta;
class TimeTicks;
struct FileDescriptor;
}

namespace IPC {

struct ChannelHandle;

struct IPC_EXPORT LogData {
  LogData();
  ~LogData();

  std::string channel;
  int32 routing_id;
  uint32 type;  
  std::string flags;
  int64 sent;  
  int64 receive;  
                  
  int64 dispatch;  
                   
  std::string message_name;
  std::string params;
};


struct NoParams {
};

template <class P>
static inline void WriteParam(Message* m, const P& p) {
  typedef typename SimilarTypeTraits<P>::Type Type;
  ParamTraits<Type>::Write(m, static_cast<const Type& >(p));
}

template <class P>
static inline bool WARN_UNUSED_RESULT ReadParam(const Message* m,
                                                PickleIterator* iter,
                                                P* p) {
  typedef typename SimilarTypeTraits<P>::Type Type;
  return ParamTraits<Type>::Read(m, iter, reinterpret_cast<Type* >(p));
}

template <class P>
static inline void LogParam(const P& p, std::string* l) {
  typedef typename SimilarTypeTraits<P>::Type Type;
  ParamTraits<Type>::Log(static_cast<const Type& >(p), l);
}


template <>
struct ParamTraits<bool> {
  typedef bool param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteBool(p);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return m->ReadBool(iter, r);
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<unsigned char> {
  typedef unsigned char param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<unsigned short> {
  typedef unsigned short param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<int> {
  typedef int param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteInt(p);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return m->ReadInt(iter, r);
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<unsigned int> {
  typedef unsigned int param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteInt(p);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return m->ReadInt(iter, reinterpret_cast<int*>(r));
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<long> {
  typedef long param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteLongUsingDangerousNonPortableLessPersistableForm(p);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return m->ReadLong(iter, r);
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<unsigned long> {
  typedef unsigned long param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteLongUsingDangerousNonPortableLessPersistableForm(p);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return m->ReadLong(iter, reinterpret_cast<long*>(r));
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<long long> {
  typedef long long param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteInt64(static_cast<int64>(p));
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    return m->ReadInt64(iter, reinterpret_cast<int64*>(r));
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<unsigned long long> {
  typedef unsigned long long param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteInt64(p);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    return m->ReadInt64(iter, reinterpret_cast<int64*>(r));
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<float> {
  typedef float param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteFloat(p);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return m->ReadFloat(iter, r);
  }
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<double> {
  typedef double param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};


template <>
struct ParamTraits<std::string> {
  typedef std::string param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteString(p);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    return m->ReadString(iter, r);
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<std::wstring> {
  typedef std::wstring param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteWString(p);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    return m->ReadWString(iter, r);
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};

#if !defined(WCHAR_T_IS_UTF16)
template <>
struct ParamTraits<base::string16> {
  typedef base::string16 param_type;
  static void Write(Message* m, const param_type& p) {
    m->WriteString16(p);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    return m->ReadString16(iter, r);
  }
  IPC_EXPORT static void Log(const param_type& p, std::string* l);
};
#endif

template <>
struct IPC_EXPORT ParamTraits<std::vector<char> > {
  typedef std::vector<char> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message*, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<std::vector<unsigned char> > {
  typedef std::vector<unsigned char> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<std::vector<bool> > {
  typedef std::vector<bool> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <class P>
struct ParamTraits<std::vector<P> > {
  typedef std::vector<P> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, static_cast<int>(p.size()));
    for (size_t i = 0; i < p.size(); i++)
      WriteParam(m, p[i]);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    int size;
    
    if (!m->ReadLength(iter, &size))
      return false;
    
    if (INT_MAX / sizeof(P) <= static_cast<size_t>(size))
      return false;
    r->resize(size);
    for (int i = 0; i < size; i++) {
      if (!ReadParam(m, iter, &(*r)[i]))
        return false;
    }
    return true;
  }
  static void Log(const param_type& p, std::string* l) {
    for (size_t i = 0; i < p.size(); ++i) {
      if (i != 0)
        l->append(" ");
      LogParam((p[i]), l);
    }
  }
};

template <class P>
struct ParamTraits<std::set<P> > {
  typedef std::set<P> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, static_cast<int>(p.size()));
    typename param_type::const_iterator iter;
    for (iter = p.begin(); iter != p.end(); ++iter)
      WriteParam(m, *iter);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    int size;
    if (!m->ReadLength(iter, &size))
      return false;
    for (int i = 0; i < size; ++i) {
      P item;
      if (!ReadParam(m, iter, &item))
        return false;
      r->insert(item);
    }
    return true;
  }
  static void Log(const param_type& p, std::string* l) {
    l->append("<std::set>");
  }
};

template <class K, class V>
struct ParamTraits<std::map<K, V> > {
  typedef std::map<K, V> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, static_cast<int>(p.size()));
    typename param_type::const_iterator iter;
    for (iter = p.begin(); iter != p.end(); ++iter) {
      WriteParam(m, iter->first);
      WriteParam(m, iter->second);
    }
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    int size;
    if (!ReadParam(m, iter, &size) || size < 0)
      return false;
    for (int i = 0; i < size; ++i) {
      K k;
      if (!ReadParam(m, iter, &k))
        return false;
      V& value = (*r)[k];
      if (!ReadParam(m, iter, &value))
        return false;
    }
    return true;
  }
  static void Log(const param_type& p, std::string* l) {
    l->append("<std::map>");
  }
};

template <class A, class B>
struct ParamTraits<std::pair<A, B> > {
  typedef std::pair<A, B> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.first);
    WriteParam(m, p.second);
  }
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r) {
    return ReadParam(m, iter, &r->first) && ReadParam(m, iter, &r->second);
  }
  static void Log(const param_type& p, std::string* l) {
    l->append("(");
    LogParam(p.first, l);
    l->append(", ");
    LogParam(p.second, l);
    l->append(")");
  }
};


template <>
struct IPC_EXPORT ParamTraits<base::DictionaryValue> {
  typedef base::DictionaryValue param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

#if defined(OS_POSIX)
template<>
struct IPC_EXPORT ParamTraits<base::FileDescriptor> {
  typedef base::FileDescriptor param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};
#endif  

template <>
struct IPC_EXPORT ParamTraits<base::FilePath> {
  typedef base::FilePath param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<base::ListValue> {
  typedef base::ListValue param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<base::NullableString16> {
  typedef base::NullableString16 param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter,
                   param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<base::PlatformFileInfo> {
  typedef base::PlatformFileInfo param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct SimilarTypeTraits<base::PlatformFileError> {
  typedef int Type;
};

#if defined(OS_WIN)
template <>
struct SimilarTypeTraits<HWND> {
  typedef HANDLE Type;
};
#endif  

template <>
struct IPC_EXPORT ParamTraits<base::Time> {
  typedef base::Time param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<base::TimeDelta> {
  typedef base::TimeDelta param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<base::TimeTicks> {
  typedef base::TimeTicks param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<Tuple0> {
  typedef Tuple0 param_type;
  static void Write(Message* m, const param_type& p) {
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return true;
  }
  static void Log(const param_type& p, std::string* l) {
  }
};

template <class A>
struct ParamTraits< Tuple1<A> > {
  typedef Tuple1<A> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.a);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return ReadParam(m, iter, &r->a);
  }
  static void Log(const param_type& p, std::string* l) {
    LogParam(p.a, l);
  }
};

template <class A, class B>
struct ParamTraits< Tuple2<A, B> > {
  typedef Tuple2<A, B> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.a);
    WriteParam(m, p.b);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return (ReadParam(m, iter, &r->a) &&
            ReadParam(m, iter, &r->b));
  }
  static void Log(const param_type& p, std::string* l) {
    LogParam(p.a, l);
    l->append(", ");
    LogParam(p.b, l);
  }
};

template <class A, class B, class C>
struct ParamTraits< Tuple3<A, B, C> > {
  typedef Tuple3<A, B, C> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.a);
    WriteParam(m, p.b);
    WriteParam(m, p.c);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return (ReadParam(m, iter, &r->a) &&
            ReadParam(m, iter, &r->b) &&
            ReadParam(m, iter, &r->c));
  }
  static void Log(const param_type& p, std::string* l) {
    LogParam(p.a, l);
    l->append(", ");
    LogParam(p.b, l);
    l->append(", ");
    LogParam(p.c, l);
  }
};

template <class A, class B, class C, class D>
struct ParamTraits< Tuple4<A, B, C, D> > {
  typedef Tuple4<A, B, C, D> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.a);
    WriteParam(m, p.b);
    WriteParam(m, p.c);
    WriteParam(m, p.d);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return (ReadParam(m, iter, &r->a) &&
            ReadParam(m, iter, &r->b) &&
            ReadParam(m, iter, &r->c) &&
            ReadParam(m, iter, &r->d));
  }
  static void Log(const param_type& p, std::string* l) {
    LogParam(p.a, l);
    l->append(", ");
    LogParam(p.b, l);
    l->append(", ");
    LogParam(p.c, l);
    l->append(", ");
    LogParam(p.d, l);
  }
};

template <class A, class B, class C, class D, class E>
struct ParamTraits< Tuple5<A, B, C, D, E> > {
  typedef Tuple5<A, B, C, D, E> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.a);
    WriteParam(m, p.b);
    WriteParam(m, p.c);
    WriteParam(m, p.d);
    WriteParam(m, p.e);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return (ReadParam(m, iter, &r->a) &&
            ReadParam(m, iter, &r->b) &&
            ReadParam(m, iter, &r->c) &&
            ReadParam(m, iter, &r->d) &&
            ReadParam(m, iter, &r->e));
  }
  static void Log(const param_type& p, std::string* l) {
    LogParam(p.a, l);
    l->append(", ");
    LogParam(p.b, l);
    l->append(", ");
    LogParam(p.c, l);
    l->append(", ");
    LogParam(p.d, l);
    l->append(", ");
    LogParam(p.e, l);
  }
};

template<class P>
struct ParamTraits<ScopedVector<P> > {
  typedef ScopedVector<P> param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, static_cast<int>(p.size()));
    for (size_t i = 0; i < p.size(); i++)
      WriteParam(m, *p[i]);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    int size = 0;
    if (!m->ReadLength(iter, &size))
      return false;
    if (INT_MAX/sizeof(P) <= static_cast<size_t>(size))
      return false;
    r->resize(size);
    for (int i = 0; i < size; i++) {
      (*r)[i] = new P();
      if (!ReadParam(m, iter, (*r)[i]))
        return false;
    }
    return true;
  }
  static void Log(const param_type& p, std::string* l) {
    for (size_t i = 0; i < p.size(); ++i) {
      if (i != 0)
        l->append(" ");
      LogParam(*p[i], l);
    }
  }
};


template<>
struct IPC_EXPORT ParamTraits<IPC::ChannelHandle> {
  typedef ChannelHandle param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<LogData> {
  typedef LogData param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<Message> {
  static void Write(Message* m, const Message& p);
  static bool Read(const Message* m, PickleIterator* iter, Message* r);
  static void Log(const Message& p, std::string* l);
};


#if defined(OS_WIN)
template <>
struct IPC_EXPORT ParamTraits<HANDLE> {
  typedef HANDLE param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<LOGFONT> {
  typedef LOGFONT param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct IPC_EXPORT ParamTraits<MSG> {
  typedef MSG param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};
#endif  


template <class ParamType>
class MessageSchema {
 public:
  typedef ParamType Param;
  typedef typename TupleTypes<ParamType>::ParamTuple RefParam;

  static void Write(Message* msg, const RefParam& p) IPC_MSG_NOINLINE;
  static bool Read(const Message* msg, Param* p) IPC_MSG_NOINLINE;
};

IPC_EXPORT void GenerateLogData(const std::string& channel,
                                const Message& message,
                                LogData* data, bool get_params);


#if defined(IPC_MESSAGE_LOG_ENABLED)
inline void AddOutputParamsToLog(const Message* msg, std::string* l) {
  const std::string& output_params = msg->output_params();
  if (!l->empty() && !output_params.empty())
    l->append(", ");

  l->append(output_params);
}

template <class ReplyParamType>
inline void LogReplyParamsToMessage(const ReplyParamType& reply_params,
                                    const Message* msg) {
  if (msg->received_time() != 0) {
    std::string output_params;
    LogParam(reply_params, &output_params);
    msg->set_output_params(output_params);
  }
}

inline void ConnectMessageAndReply(const Message* msg, Message* reply) {
  if (msg->sent_time()) {
    
    
    
    LogData* data = new LogData;
    GenerateLogData("", *msg, data, true);
    msg->set_dont_log();
    reply->set_sync_log_data(data);
  }
}
#else
inline void AddOutputParamsToLog(const Message* msg, std::string* l) {}

template <class ReplyParamType>
inline void LogReplyParamsToMessage(const ReplyParamType& reply_params,
                                    const Message* msg) {}

inline void ConnectMessageAndReply(const Message* msg, Message* reply) {}
#endif

template <class RefTuple>
class ParamDeserializer : public MessageReplyDeserializer {
 public:
  explicit ParamDeserializer(const RefTuple& out) : out_(out) { }

  bool SerializeOutputParameters(const IPC::Message& msg, PickleIterator iter) {
    return ReadParam(&msg, &iter, &out_);
  }

  RefTuple out_;
};

template <class SendParamType, class ReplyParamType>
class SyncMessageSchema {
 public:
  typedef SendParamType SendParam;
  typedef typename TupleTypes<SendParam>::ParamTuple RefSendParam;
  typedef ReplyParamType ReplyParam;

  static void Write(Message* msg, const RefSendParam& send) IPC_MSG_NOINLINE;
  static bool ReadSendParam(const Message* msg, SendParam* p) IPC_MSG_NOINLINE;
  static bool ReadReplyParam(
      const Message* msg,
      typename TupleTypes<ReplyParam>::ValueTuple* p) IPC_MSG_NOINLINE;

  template<class T, class S, class Method>
  static bool DispatchWithSendParams(bool ok, const SendParam& send_params,
                                     const Message* msg, T* obj, S* sender,
                                     Method func) {
    Message* reply = SyncMessage::GenerateReply(msg);
    if (ok) {
      typename TupleTypes<ReplyParam>::ValueTuple reply_params;
      DispatchToMethod(obj, func, send_params, &reply_params);
      WriteParam(reply, reply_params);
      LogReplyParamsToMessage(reply_params, msg);
    } else {
      NOTREACHED() << "Error deserializing message " << msg->type();
      reply->set_reply_error();
    }
    sender->Send(reply);
    return ok;
  }

  template<class T, class Method>
  static bool DispatchDelayReplyWithSendParams(bool ok,
                                               const SendParam& send_params,
                                               const Message* msg, T* obj,
                                               Method func) {
    Message* reply = SyncMessage::GenerateReply(msg);
    if (ok) {
      Tuple1<Message&> t = MakeRefTuple(*reply);
      ConnectMessageAndReply(msg, reply);
      DispatchToMethod(obj, func, send_params, &t);
    } else {
      NOTREACHED() << "Error deserializing message " << msg->type();
      reply->set_reply_error();
      obj->Send(reply);
    }
    return ok;
  }

  template<typename TA>
  static void WriteReplyParams(Message* reply, TA a) {
    ReplyParam p(a);
    WriteParam(reply, p);
  }

  template<typename TA, typename TB>
  static void WriteReplyParams(Message* reply, TA a, TB b) {
    ReplyParam p(a, b);
    WriteParam(reply, p);
  }

  template<typename TA, typename TB, typename TC>
  static void WriteReplyParams(Message* reply, TA a, TB b, TC c) {
    ReplyParam p(a, b, c);
    WriteParam(reply, p);
  }

  template<typename TA, typename TB, typename TC, typename TD>
  static void WriteReplyParams(Message* reply, TA a, TB b, TC c, TD d) {
    ReplyParam p(a, b, c, d);
    WriteParam(reply, p);
  }

  template<typename TA, typename TB, typename TC, typename TD, typename TE>
  static void WriteReplyParams(Message* reply, TA a, TB b, TC c, TD d, TE e) {
    ReplyParam p(a, b, c, d, e);
    WriteParam(reply, p);
  }
};

}  

#endif  
