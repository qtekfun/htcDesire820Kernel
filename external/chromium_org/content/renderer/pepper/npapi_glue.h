// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_NPAPI_GLUE_H_
#define CONTENT_RENDERER_PEPPER_NPAPI_GLUE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_var.h"

struct NPObject;
typedef struct _NPVariant NPVariant;
typedef void* NPIdentifier;

namespace content {

class PepperPluginInstanceImpl;
class PluginObject;


bool PPVarToNPVariant(PP_Var var, NPVariant* result);

PP_Var NPVariantToPPVar(PepperPluginInstanceImpl* instance,
                        const NPVariant* variant);

NPIdentifier PPVarToNPIdentifier(PP_Var var);

PP_Var NPIdentifierToPPVar(NPIdentifier id);

CONTENT_EXPORT PP_Var NPObjectToPPVar(PepperPluginInstanceImpl* instance,
                                      NPObject* object);

CONTENT_EXPORT PP_Var NPObjectToPPVarForTest(PepperPluginInstanceImpl* instance,
                                             NPObject* object);


class PPResultAndExceptionToNPResult {
 public:
  
  
  
  
  
  
  
  
  PPResultAndExceptionToNPResult(NPObject* object_var, NPVariant* np_result);

  ~PPResultAndExceptionToNPResult();

  
  bool has_exception() const { return exception_.type != PP_VARTYPE_UNDEFINED; }

  
  
  
  PP_Var* exception() { return &exception_; }

  
  
  bool success() const {
    return success_;
  }

  
  
  
  bool SetResult(PP_Var result);

  
  
  
  
  
  bool CheckExceptionForNoResult();

  
  
  void IgnoreException();

 private:
  
  void ThrowException();

  NPObject* object_var_;  
  NPVariant* np_result_;  
  PP_Var exception_;  
  bool success_;  
  bool checked_exception_;  

  DISALLOW_COPY_AND_ASSIGN(PPResultAndExceptionToNPResult);
};


class PPVarArrayFromNPVariantArray {
 public:
  PPVarArrayFromNPVariantArray(PepperPluginInstanceImpl* instance,
                               size_t size,
                               const NPVariant* variants);
  ~PPVarArrayFromNPVariantArray();

  PP_Var* array() { return array_.get(); }

 private:
  size_t size_;
  scoped_ptr<PP_Var[]> array_;

  DISALLOW_COPY_AND_ASSIGN(PPVarArrayFromNPVariantArray);
};


class PPVarFromNPObject {
 public:
  PPVarFromNPObject(PepperPluginInstanceImpl* instance, NPObject* object);
  ~PPVarFromNPObject();

  PP_Var var() const { return var_; }

 private:
  const PP_Var var_;

  DISALLOW_COPY_AND_ASSIGN(PPVarFromNPObject);
};


class NPObjectAccessorWithIdentifier {
 public:
  NPObjectAccessorWithIdentifier(NPObject* object,
                                 NPIdentifier identifier,
                                 bool allow_integer_identifier);
  ~NPObjectAccessorWithIdentifier();

  
  bool is_valid() const {
    return object_ && identifier_.type != PP_VARTYPE_UNDEFINED;
  }

  PluginObject* object() { return object_; }
  PP_Var identifier() const { return identifier_; }

 private:
  PluginObject* object_;
  PP_Var identifier_;

  DISALLOW_COPY_AND_ASSIGN(NPObjectAccessorWithIdentifier);
};


class TryCatch {
 public:
  
  
  
  TryCatch(PP_Var* exception);
  ~TryCatch();

  
  
  bool has_exception() const { return has_exception_; }

  
  
  void SetException(const char* message);

 private:
  static void Catch(void* self, const char* message);

  
  
  
  
  bool has_exception_;

  
  PP_Var* exception_;
};

}  

#endif  
