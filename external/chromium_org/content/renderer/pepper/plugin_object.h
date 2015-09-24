// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PLUGIN_OBJECT_H_
#define CONTENT_RENDERER_PEPPER_PLUGIN_OBJECT_H_

#include <string>

#include "base/basictypes.h"

struct PP_Var;
struct PPP_Class_Deprecated;
typedef struct NPObject NPObject;
typedef struct _NPVariant NPVariant;

namespace content {

class PepperPluginInstanceImpl;

class PluginObject {
 public:
  virtual ~PluginObject();

  
  
  static PP_Var Create(PepperPluginInstanceImpl* instance,
                       const PPP_Class_Deprecated* ppp_class,
                       void* ppp_class_data);

  PepperPluginInstanceImpl* instance() const { return instance_; }

  const PPP_Class_Deprecated* ppp_class() { return ppp_class_; }
  void* ppp_class_data() { return ppp_class_data_; };

  NPObject* GetNPObject() const;

  
  
  
  
  static bool IsInstanceOf(NPObject* np_object,
                           const PPP_Class_Deprecated* ppp_class,
                           void** ppp_class_data);

  
  
  
  
  
  
  static PluginObject* FromNPObject(NPObject* object);

  
  
  static NPObject* AllocateObjectWrapper();

 private:
  struct NPObjectWrapper;

  
  
  
  
  
  PluginObject(PepperPluginInstanceImpl* instance,
               NPObjectWrapper* object_wrapper,
               const PPP_Class_Deprecated* ppp_class,
               void* ppp_class_data);

  PepperPluginInstanceImpl* instance_;

  
  
  
  
  
  
  NPObjectWrapper* object_wrapper_;

  const PPP_Class_Deprecated* ppp_class_;
  void* ppp_class_data_;

  DISALLOW_COPY_AND_ASSIGN(PluginObject);
};

}  

#endif  
