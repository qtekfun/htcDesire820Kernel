// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_RESOURCE_MESSAGE_PARAMS_H_
#define PPAPI_PROXY_RESOURCE_MESSAGE_PARAMS_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "ipc/ipc_message_utils.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/proxy/serialized_handle.h"

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT ResourceMessageParams {
 public:
  virtual ~ResourceMessageParams();

  PP_Resource pp_resource() const { return pp_resource_; }
  int32_t sequence() const { return sequence_; }

  
  const std::vector<SerializedHandle>& handles() const {
    return handles_->data();
  }

  
  
  
  void ConsumeHandles() const;

  
  
  
  
  
  
  SerializedHandle TakeHandleOfTypeAtIndex(size_t index,
                                           SerializedHandle::Type type) const;

  
  
  
  
  
  
  
  
  
  
  
  
  bool TakeSharedMemoryHandleAtIndex(size_t index,
                                     base::SharedMemoryHandle* handle) const;
  bool TakeSocketHandleAtIndex(size_t index,
                               IPC::PlatformFileForTransit* handle) const;
  bool TakeFileHandleAtIndex(size_t index,
                             IPC::PlatformFileForTransit* handle) const;
  void TakeAllSharedMemoryHandles(
      std::vector<base::SharedMemoryHandle>* handles) const;

  
  
  void AppendHandle(const SerializedHandle& handle) const;

 protected:
  ResourceMessageParams();
  ResourceMessageParams(PP_Resource resource, int32_t sequence);

  virtual void Serialize(IPC::Message* msg) const;
  virtual bool Deserialize(const IPC::Message* msg, PickleIterator* iter);

  
  void WriteHeader(IPC::Message* msg) const;
  
  void WriteHandles(IPC::Message* msg) const;
  
  bool ReadHeader(const IPC::Message* msg, PickleIterator* iter);
  bool ReadHandles(const IPC::Message* msg, PickleIterator* iter);

 private:
  class SerializedHandles
      : public base::RefCountedThreadSafe<SerializedHandles> {
   public:
    SerializedHandles();
    ~SerializedHandles();

    void set_should_close(bool value) { should_close_ = value; }
    std::vector<SerializedHandle>& data() { return data_; }

   private:
    friend class base::RefCountedThreadSafe<SerializedHandles>;

    
    
    
    
    
    
    
    
    bool should_close_;
    std::vector<SerializedHandle> data_;
  };

  PP_Resource pp_resource_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t sequence_;

  
  
  
  
  
  mutable scoped_refptr<SerializedHandles> handles_;
};

class PPAPI_PROXY_EXPORT ResourceMessageCallParams
    : public ResourceMessageParams {
 public:
  ResourceMessageCallParams();
  ResourceMessageCallParams(PP_Resource resource, int32_t sequence);
  virtual ~ResourceMessageCallParams();

  void set_has_callback() { has_callback_ = true; }
  bool has_callback() const { return has_callback_; }

  virtual void Serialize(IPC::Message* msg) const OVERRIDE;
  virtual bool Deserialize(const IPC::Message* msg,
                           PickleIterator* iter) OVERRIDE;

 private:
  bool has_callback_;
};

class PPAPI_PROXY_EXPORT ResourceMessageReplyParams
    : public ResourceMessageParams {
 public:
  ResourceMessageReplyParams();
  ResourceMessageReplyParams(PP_Resource resource, int32_t sequence);
  virtual ~ResourceMessageReplyParams();

  void set_result(int32_t r) { result_ = r; }
  int32_t result() const { return result_; }

  virtual void Serialize(IPC::Message* msg) const OVERRIDE;
  virtual bool Deserialize(const IPC::Message* msg,
                           PickleIterator* iter) OVERRIDE;

  
  void WriteReplyHeader(IPC::Message* msg) const;

 private:
  
  int32_t result_;
};

}  
}  

namespace IPC {

template <> struct PPAPI_PROXY_EXPORT
ParamTraits<ppapi::proxy::ResourceMessageCallParams> {
  typedef ppapi::proxy::ResourceMessageCallParams param_type;
  static void Write(Message* m, const param_type& p) {
    p.Serialize(m);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return r->Deserialize(m, iter);
  }
  static void Log(const param_type& p, std::string* l) {
  }
};

template <> struct PPAPI_PROXY_EXPORT
ParamTraits<ppapi::proxy::ResourceMessageReplyParams> {
  typedef ppapi::proxy::ResourceMessageReplyParams param_type;
  static void Write(Message* m, const param_type& p) {
    p.Serialize(m);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r) {
    return r->Deserialize(m, iter);
  }
  static void Log(const param_type& p, std::string* l) {
  }
};

}  

#endif  
