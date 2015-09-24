// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_VAR_H_
#define PPAPI_SHARED_IMPL_VAR_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/shared_memory.h"
#include "base/platform_file.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/host_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class ArrayBufferVar;
class ArrayVar;
class DictionaryVar;
class NPObjectVar;
class ProxyObjectVar;
class ResourceVar;
class StringVar;
class VarTracker;


class PPAPI_SHARED_EXPORT Var : public base::RefCounted<Var> {
 public:
  
  static std::string PPVarToLogString(PP_Var var);

  virtual StringVar* AsStringVar();
  virtual ArrayBufferVar* AsArrayBufferVar();
  virtual NPObjectVar* AsNPObjectVar();
  virtual ProxyObjectVar* AsProxyObjectVar();
  virtual ArrayVar* AsArrayVar();
  virtual DictionaryVar* AsDictionaryVar();
  virtual ResourceVar* AsResourceVar();

  
  
  PP_Var GetPPVar();

  
  virtual PP_VarType GetType() const = 0;

  
  
  
  
  
  
  int32 GetExistingVarID() const;

 protected:
  friend class base::RefCounted<Var>;
  friend class VarTracker;

  Var();
  virtual ~Var();

  
  
  
  
  
  
  int32 GetOrCreateVarID();

  
  
  void AssignVarID(int32 id);

  
  void ResetVarID() { var_id_ = 0; }

 private:
  
  int32 var_id_;

  DISALLOW_COPY_AND_ASSIGN(Var);
};


class PPAPI_SHARED_EXPORT StringVar : public Var {
 public:
  explicit StringVar(const std::string& str);
  StringVar(const char* str, uint32 len);
  virtual ~StringVar();

  const std::string& value() const { return value_; }
  
  
  
  
  
  const std::string* ptr() const { return &value_; }

  
  virtual StringVar* AsStringVar() OVERRIDE;
  virtual PP_VarType GetType() const OVERRIDE;

  
  
  
  
  
  
  static PP_Var StringToPPVar(const std::string& str);
  static PP_Var StringToPPVar(const char* str, uint32 len);

  
  
  
  static PP_Var SwapValidatedUTF8StringIntoPPVar(std::string* src);

  
  
  static StringVar* FromPPVar(PP_Var var);

 private:
  StringVar();  

  std::string value_;

  DISALLOW_COPY_AND_ASSIGN(StringVar);
};


class PPAPI_SHARED_EXPORT ArrayBufferVar : public Var {
 public:
  ArrayBufferVar();
  virtual ~ArrayBufferVar();

  virtual void* Map() = 0;
  virtual void Unmap() = 0;
  virtual uint32 ByteLength() = 0;

  
  
  
  
  
  
  
  virtual bool CopyToNewShmem(
      PP_Instance instance,
      int *host_shm_handle_id,
      base::SharedMemoryHandle *plugin_shm_handle) = 0;

  
  virtual ArrayBufferVar* AsArrayBufferVar() OVERRIDE;
  virtual PP_VarType GetType() const OVERRIDE;

  
  
  static ArrayBufferVar* FromPPVar(PP_Var var);

 private:
  DISALLOW_COPY_AND_ASSIGN(ArrayBufferVar);
};

}  

#endif  
