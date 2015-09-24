// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_SERIALIZED_VAR_H_
#define PPAPI_PROXY_SERIALIZED_VAR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/proxy/raw_var_data.h"
#include "ppapi/proxy/serialized_handle.h"
#include "ppapi/proxy/serialized_structs.h"
#include "ppapi/proxy/var_serialization_rules.h"

class PickleIterator;

namespace IPC {
class Message;
}

namespace ppapi {
namespace proxy {

class Dispatcher;
class VarSerializationRules;

class PPAPI_PROXY_EXPORT SerializedVar {
 public:
  SerializedVar();
  ~SerializedVar();

  
  void WriteToMessage(IPC::Message* m) const {
    inner_->WriteToMessage(m);
  }
  
  
  
  void WriteDataToMessage(IPC::Message* m,
                          const HandleWriter& handle_writer) const {
    inner_->WriteDataToMessage(m, handle_writer);
  }
  bool ReadFromMessage(const IPC::Message* m, PickleIterator* iter) {
    return inner_->ReadFromMessage(m, iter);
  }

  bool is_valid_var() const {
    return inner_->is_valid_var();
  }

  
  std::vector<SerializedHandle*> GetHandles() const {
    return inner_->GetHandles();
  }

 protected:
  friend class SerializedVarReceiveInput;
  friend class SerializedVarReturnValue;
  friend class SerializedVarOutParam;
  friend class SerializedVarSendInput;
  friend class SerializedVarSendInputShmem;
  friend class SerializedVarTestConstructor;
  friend class SerializedVarVectorReceiveInput;

  class PPAPI_PROXY_EXPORT Inner : public base::RefCounted<Inner> {
   public:
    Inner();
    Inner(VarSerializationRules* serialization_rules);
    ~Inner();

    VarSerializationRules* serialization_rules() {
      return serialization_rules_.get();
    }
    void set_serialization_rules(VarSerializationRules* serialization_rules) {
      serialization_rules_ = serialization_rules;
    }

    bool is_valid_var() const {
      return is_valid_var_;
    }

    std::vector<SerializedHandle*> GetHandles() {
      return (raw_var_data_ ? raw_var_data_->GetHandles() :
          std::vector<SerializedHandle*>());
    }

    
    PP_Var GetVar();
    void SetVar(PP_Var var);
    void SetInstance(PP_Instance instance);

    
    
    void ForceSetVarValueForTest(PP_Var value);

    void WriteToMessage(IPC::Message* m) const;
    void WriteDataToMessage(IPC::Message* m,
                            const HandleWriter& handle_writer) const;
    bool ReadFromMessage(const IPC::Message* m, PickleIterator* iter);

    
    void SetCleanupModeToEndSendPassRef();
    void SetCleanupModeToEndReceiveCallerOwned();

   private:
    enum CleanupMode {
      
      
      CLEANUP_NONE,

      
      END_SEND_PASS_REF,

      
      END_RECEIVE_CALLER_OWNED
    };

    
    
    
    scoped_refptr<VarSerializationRules> serialization_rules_;

    
    
    
    
    
    
    
    
    PP_Var var_;

    PP_Instance instance_;

    CleanupMode cleanup_mode_;

    
    bool is_valid_var_;

#ifndef NDEBUG
    
    
    mutable bool has_been_serialized_;
    mutable bool has_been_deserialized_;
#endif

    
    
    
    
    
    scoped_ptr<RawVarDataGraph> raw_var_data_;

    DISALLOW_COPY_AND_ASSIGN(Inner);
  };

  SerializedVar(VarSerializationRules* serialization_rules);

  mutable scoped_refptr<Inner> inner_;
};


class PPAPI_PROXY_EXPORT SerializedVarSendInput : public SerializedVar {
 public:
  SerializedVarSendInput(Dispatcher* dispatcher, const PP_Var& var);

  
  static void ConvertVector(Dispatcher* dispatcher,
                            const PP_Var* input,
                            size_t input_count,
                            std::vector<SerializedVar>* output);

 private:
  
  
  SerializedVarSendInput();
};

class PPAPI_PROXY_EXPORT SerializedVarSendInputShmem : public SerializedVar {
 public:
  SerializedVarSendInputShmem(Dispatcher* dispatcher, const PP_Var& var,
                              const PP_Instance& instance);

 private:
  
  
  SerializedVarSendInputShmem();
};


class PPAPI_PROXY_EXPORT ReceiveSerializedVarReturnValue
    : public SerializedVar {
 public:
  
  
  
  
  ReceiveSerializedVarReturnValue();

  
  
  
  
  explicit ReceiveSerializedVarReturnValue(const SerializedVar& serialized);

  PP_Var Return(Dispatcher* dispatcher);

 private:
  DISALLOW_COPY_AND_ASSIGN(ReceiveSerializedVarReturnValue);
};

class PPAPI_PROXY_EXPORT ReceiveSerializedException : public SerializedVar {
 public:
  ReceiveSerializedException(Dispatcher* dispatcher, PP_Var* exception);
  ~ReceiveSerializedException();

  
  
  bool IsThrown() const;

 private:
  
  PP_Var* exception_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ReceiveSerializedException);
};

class PPAPI_PROXY_EXPORT ReceiveSerializedVarVectorOutParam {
 public:
  ReceiveSerializedVarVectorOutParam(Dispatcher* dispatcher,
                                     uint32_t* output_count,
                                     PP_Var** output);
  ~ReceiveSerializedVarVectorOutParam();

  std::vector<SerializedVar>* OutParam();

 private:
  Dispatcher* dispatcher_;
  uint32_t* output_count_;
  PP_Var** output_;

  std::vector<SerializedVar> vector_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ReceiveSerializedVarVectorOutParam);
};


class PPAPI_PROXY_EXPORT SerializedVarReceiveInput {
 public:
  
  
  
  SerializedVarReceiveInput(const SerializedVar& serialized);
  ~SerializedVarReceiveInput();

  PP_Var Get(Dispatcher* dispatcher);
  PP_Var GetForInstance(Dispatcher* dispatcher, PP_Instance instance);
  bool is_valid_var() { return serialized_.is_valid_var(); }

 private:
  const SerializedVar& serialized_;
};

class PPAPI_PROXY_EXPORT SerializedVarVectorReceiveInput {
 public:
  SerializedVarVectorReceiveInput(const std::vector<SerializedVar>& serialized);
  ~SerializedVarVectorReceiveInput();

  
  
  
  PP_Var* Get(Dispatcher* dispatcher, uint32_t* array_size);

 private:
  const std::vector<SerializedVar>& serialized_;

  
  std::vector<PP_Var> deserialized_;
};

class PPAPI_PROXY_EXPORT SerializedVarReturnValue {
 public:
  
  
  
  SerializedVarReturnValue(SerializedVar* serialized);

  void Return(Dispatcher* dispatcher, const PP_Var& var);

  
  
  
  static SerializedVar Convert(Dispatcher* dispatcher, const PP_Var& var);

 private:
  SerializedVar* serialized_;
};

class PPAPI_PROXY_EXPORT SerializedVarOutParam {
 public:
  
  
  
  SerializedVarOutParam(SerializedVar* serialized);
  ~SerializedVarOutParam();

  
  
  
  PP_Var* OutParam(Dispatcher* dispatcher);

 private:
  SerializedVar* serialized_;

  // This is the value actually written by the code and returned by OutParam.
  
  PP_Var writable_var_;

  Dispatcher* dispatcher_;
};

class PPAPI_PROXY_EXPORT SerializedVarVectorOutParam {
 public:
  SerializedVarVectorOutParam(std::vector<SerializedVar>* serialized);
  ~SerializedVarVectorOutParam();

  uint32_t* CountOutParam() { return &count_; }
  PP_Var** ArrayOutParam(Dispatcher* dispatcher);

 private:
  Dispatcher* dispatcher_;
  std::vector<SerializedVar>* serialized_;

  uint32_t count_;
  PP_Var* array_;
};

class PPAPI_PROXY_EXPORT SerializedVarTestConstructor : public SerializedVar {
 public:
  
  explicit SerializedVarTestConstructor(const PP_Var& pod_var);

  
  explicit SerializedVarTestConstructor(const std::string& str);
};

class PPAPI_PROXY_EXPORT SerializedVarTestReader : public SerializedVar {
 public:
  explicit SerializedVarTestReader(const SerializedVar& var);

  PP_Var GetVar() const { return inner_->GetVar(); }
};

}  
}  

#endif  
