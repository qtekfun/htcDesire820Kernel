// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_RAW_VAR_DATA_H_
#define PPAPI_PROXY_RAW_VAR_DATA_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/proxy/ppapi_param_traits.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/proxy/serialized_handle.h"

class PickleIterator;

namespace IPC {
class Message;
}

namespace ppapi {
namespace proxy {

class RawVarData;

typedef base::Callback<void(IPC::Message*, const SerializedHandle&)>
    HandleWriter;

class PPAPI_PROXY_EXPORT RawVarDataGraph {
 public:
  
  
  static scoped_ptr<RawVarDataGraph> Create(const PP_Var& var,
                                            PP_Instance instance);

  
  RawVarDataGraph();
  ~RawVarDataGraph();

  
  
  
  
  PP_Var CreatePPVar(PP_Instance instance);

  
  void Write(IPC::Message* m, const HandleWriter& handle_writer);

  
  static scoped_ptr<RawVarDataGraph> Read(const IPC::Message* m,
                                          PickleIterator* iter);

  
  
  std::vector<SerializedHandle*> GetHandles();

  
  
  
  
  static void SetMinimumArrayBufferSizeForShmemForTest(uint32 threshold);

  
  ScopedVector<RawVarData> data_;
};

class RawVarData {
 public:
  
  static RawVarData* Create(PP_VarType type);
  RawVarData();
  virtual ~RawVarData();

  
  virtual PP_VarType Type() = 0;

  
  virtual bool Init(const PP_Var& var, PP_Instance instance) = 0;

  
  virtual PP_Var CreatePPVar(PP_Instance instance) = 0;
  
  
  
  
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) = 0;

  
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) = 0;
  
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) = 0;

  
  
  virtual SerializedHandle* GetHandle();

  bool initialized() { return initialized_; }

 protected:
  bool initialized_;
};

class BasicRawVarData : public RawVarData {
 public:
  BasicRawVarData();
  virtual ~BasicRawVarData();

  
  virtual PP_VarType Type() OVERRIDE;
  virtual bool Init(const PP_Var& var, PP_Instance instance) OVERRIDE;
  virtual PP_Var CreatePPVar(PP_Instance instance) OVERRIDE;
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) OVERRIDE;
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) OVERRIDE;
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) OVERRIDE;

 private:
  PP_Var var_;
};

class StringRawVarData : public RawVarData {
 public:
  StringRawVarData();
  virtual ~StringRawVarData();

  
  virtual PP_VarType Type() OVERRIDE;
  virtual bool Init(const PP_Var& var, PP_Instance instance) OVERRIDE;
  virtual PP_Var CreatePPVar(PP_Instance instance) OVERRIDE;
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) OVERRIDE;
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) OVERRIDE;
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) OVERRIDE;

 private:
  
  std::string data_;
};

class ArrayBufferRawVarData : public RawVarData {
 public:
  
  enum ShmemType {
    ARRAY_BUFFER_NO_SHMEM,
    ARRAY_BUFFER_SHMEM_HOST,
    ARRAY_BUFFER_SHMEM_PLUGIN,
  };

  ArrayBufferRawVarData();
  virtual ~ArrayBufferRawVarData();

  
  virtual PP_VarType Type() OVERRIDE;
  virtual bool Init(const PP_Var& var, PP_Instance instance) OVERRIDE;
  virtual PP_Var CreatePPVar(PP_Instance instance) OVERRIDE;
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) OVERRIDE;
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) OVERRIDE;
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) OVERRIDE;
  virtual SerializedHandle* GetHandle() OVERRIDE;

 private:
  
  ShmemType type_;
  
  std::string data_;
  
  int host_shm_handle_id_;
  
  SerializedHandle plugin_shm_handle_;
};

class ArrayRawVarData : public RawVarData {
 public:
  ArrayRawVarData();
  virtual ~ArrayRawVarData();

  void AddChild(size_t element);

  
  virtual PP_VarType Type() OVERRIDE;
  virtual bool Init(const PP_Var& var, PP_Instance instance) OVERRIDE;
  virtual PP_Var CreatePPVar(PP_Instance instance) OVERRIDE;
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) OVERRIDE;
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) OVERRIDE;
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) OVERRIDE;

 private:
  std::vector<size_t> children_;
};

class DictionaryRawVarData : public RawVarData {
 public:
  DictionaryRawVarData();
  virtual ~DictionaryRawVarData();

  void AddChild(const std::string& key, size_t value);

  
  virtual PP_VarType Type() OVERRIDE;
  virtual bool Init(const PP_Var& var, PP_Instance instance) OVERRIDE;
  virtual PP_Var CreatePPVar(PP_Instance instance) OVERRIDE;
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) OVERRIDE;
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) OVERRIDE;
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) OVERRIDE;

 private:
  std::vector<std::pair<std::string, size_t> > children_;
};

class ResourceRawVarData : public RawVarData {
 public:
  ResourceRawVarData();
  virtual ~ResourceRawVarData();

  
  virtual PP_VarType Type() OVERRIDE;
  virtual bool Init(const PP_Var& var, PP_Instance instance) OVERRIDE;
  virtual PP_Var CreatePPVar(PP_Instance instance) OVERRIDE;
  virtual void PopulatePPVar(const PP_Var& var,
                             const std::vector<PP_Var>& graph) OVERRIDE;
  virtual void Write(IPC::Message* m,
                     const HandleWriter& handle_writer) OVERRIDE;
  virtual bool Read(PP_VarType type,
                    const IPC::Message* m,
                    PickleIterator* iter) OVERRIDE;

 private:
  
  
  PP_Resource pp_resource_;

  
  int pending_renderer_host_id_;

  
  int pending_browser_host_id_;

  
  
  
  
  scoped_ptr<IPC::Message> creation_message_;
};

}  
}  

#endif  
