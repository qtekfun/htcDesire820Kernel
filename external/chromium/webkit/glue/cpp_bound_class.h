// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef WEBKIT_GLUE_CPP_BOUNDCLASS_H__
#define WEBKIT_GLUE_CPP_BOUNDCLASS_H__

#include <map>
#include <vector>

#include "webkit/glue/cpp_variant.h"

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"

namespace WebKit {
class WebFrame;
}

typedef std::vector<CppVariant> CppArgumentList;

class CppBoundClass {
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

  
  
  
  
  
  
  
  void BindToJavascript(WebKit::WebFrame* frame, const std::string& classname);

  
  typedef Callback2<const CppArgumentList&, CppVariant*>::Type Callback;
  typedef Callback1<CppVariant*>::Type GetterCallback;

  
  
  bool IsMethodRegistered(const std::string& name) const;

 protected:
  
  void BindCallback(const std::string& name, Callback* callback);

  
  
  
  template<typename T>
  void BindMethod(const std::string& name,
      void (T::*method)(const CppArgumentList&, CppVariant*)) {
    Callback* callback =
        NewCallback<T, const CppArgumentList&, CppVariant*>(
            static_cast<T*>(this), method);
    BindCallback(name, callback);
  }

  
  
  void BindGetterCallback(const std::string& name, GetterCallback* callback);

  
  
  
  template<typename T>
  void BindProperty(const std::string& name, void (T::*method)(CppVariant*)) {
    GetterCallback* callback =
        NewCallback<T, CppVariant*>(static_cast<T*>(this), method);
    BindGetterCallback(name, callback);
  }

  
  void BindProperty(const std::string& name, CppVariant* prop);

  
  
  void BindProperty(const std::string& name, PropertyCallback* callback);

  
  
  
  
  
  
  
  
  
  
  void BindFallbackCallback(Callback* fallback_callback) {
    fallback_callback_.reset(fallback_callback);
  }

  
  
  
  
  template<typename T>
  void BindFallbackMethod(
      void (T::*method)(const CppArgumentList&, CppVariant*)) {
    if (method) {
      Callback* callback =
          NewCallback<T, const CppArgumentList&, CppVariant*>(
              static_cast<T*>(this), method);
      BindFallbackCallback(callback);
    } else {
      BindFallbackCallback(NULL);
    }
  }

  
  

  typedef std::map<NPIdentifier, PropertyCallback*> PropertyList;
  typedef std::map<NPIdentifier, Callback*> MethodList;
  
  
  PropertyList properties_;
  MethodList methods_;

  
  scoped_ptr<Callback> fallback_callback_;

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

  DISALLOW_COPY_AND_ASSIGN(CppBoundClass);
};

#endif  
