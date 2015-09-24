// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_PLUGIN_HOST_PLUGIN_UTILS_H_
#define REMOTING_HOST_PLUGIN_HOST_PLUGIN_UTILS_H_

#include <string>

#include "third_party/npapi/bindings/npapi.h"
#include "third_party/npapi/bindings/npfunctions.h"
#include "third_party/npapi/bindings/npruntime.h"

namespace remoting {

extern NPNetscapeFuncs* g_npnetscape_funcs;

std::string StringFromNPIdentifier(NPIdentifier identifier);

std::string StringFromNPVariant(const NPVariant& variant);

NPVariant NPVariantFromString(const std::string& val);

NPObject* ObjectFromNPVariant(const NPVariant& variant);

class ScopedRefNPObject {
 public:
  ScopedRefNPObject();
  explicit ScopedRefNPObject(NPObject* object);
  explicit ScopedRefNPObject(const ScopedRefNPObject& object);
  ~ScopedRefNPObject();

  
  
  ScopedRefNPObject& operator=(NPObject* object);
  ScopedRefNPObject& operator=(const ScopedRefNPObject& object);

  NPObject* get() const { return object_; }

 private:
  NPObject* object_;
};

}  

#endif  
