// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef WEBKIT_RENDERER_CPP_BOUND_CLASS_H_
#define WEBKIT_RENDERER_CPP_BOUND_CLASS_H_

#include <map>
#include <vector>

#include "base/callback.h"
#include "webkit/renderer/cpp_variant.h"
#include "webkit/renderer/webkit_renderer_export.h"

namespace blink {
class WebFrame;
}

namespace webkit_glue {

typedef std::vector<CppVariant> CppArgumentList;

class WEBKIT_RENDERER_EXPORT CppBoundClass {
 public:
  class PropertyCallback {
   public:
    virtual ~PropertyCallback() { }

    
    
    virtual bool GetValue(CppVariant* value) = 0;

    
    virtual bool SetValue(const CppVariant& value) = 0;
  };

  
  
  
  CppBoundClass();
  virtual ~CppBoundClass();

  
  
  CppVariant* GetAsCppVariant();

  
  
  
  
  
  
  
  void BindToJavascript(blink::WebFrame* frame, const std::string& classname);

  
  typedef base::Callback<void(const CppArgumentList&, CppVariant*)> Callback;
  typedef base::Callback<void(CppVariant*)> GetterCallback;

  
  
  bool IsMethodRegistered(const std::string& name) const;

 protected:
  
  void BindCallback(const std::string& name, const Callback& callback);

  
  
  void BindGetterCallback(const std::string& name,
                          const GetterCallback& callback);

  
  void BindProperty(const std::string& name, CppVariant* prop);

  
  
  void BindProperty(const std::string& name, PropertyCallback* callback);

  
  
  
  
  
  
  
  
  
  
  void BindFallbackCallback(const Callback& fallback_callback) {
    fallback_callback_ = fallback_callback;
  }

  
  

  typedef std::map<NPIdentifier, PropertyCallback*> PropertyList;
  typedef std::map<NPIdentifier, Callback> MethodList;
  
  
  PropertyList properties_;
  MethodList methods_;

  
  Callback fallback_callback_;

 private:
  
  friend struct CppNPObject;
  bool HasMethod(NPIdentifier ident) const;
  bool Invoke(NPIdentifier ident, const NPVariant* args, size_t arg_count,
              NPVariant* result);
  bool HasProperty(NPIdentifier ident) const;
  bool GetProperty(NPIdentifier ident, NPVariant* result) const;
  bool SetProperty(NPIdentifier ident, const NPVariant* value);

  
  
  CppVariant self_variant_;

  
  bool bound_to_frame_;

  
  scoped_ptr<NPP_t> npp_;

  DISALLOW_COPY_AND_ASSIGN(CppBoundClass);
};

}  

#endif  
