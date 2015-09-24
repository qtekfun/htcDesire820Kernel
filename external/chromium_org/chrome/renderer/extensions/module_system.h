// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_MODULE_SYSTEM_H_
#define CHROME_RENDERER_EXTENSIONS_MODULE_SYSTEM_H_

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/renderer/extensions/native_handler.h"
#include "chrome/renderer/extensions/object_backed_native_handler.h"
#include "v8/include/v8.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace extensions {

class ChromeV8Context;

class ModuleSystem : public ObjectBackedNativeHandler {
 public:
  class SourceMap {
   public:
    virtual ~SourceMap() {}
    virtual v8::Handle<v8::Value> GetSource(v8::Isolate* isolate,
                                            const std::string& name) = 0;
    virtual bool Contains(const std::string& name) = 0;
  };

  class ExceptionHandler {
   public:
    virtual ~ExceptionHandler() {}
    virtual void HandleUncaughtException(const v8::TryCatch& try_catch) = 0;

   protected:
    
    std::string CreateExceptionString(const v8::TryCatch& try_catch);
  };

  
  class NativesEnabledScope {
   public:
    explicit NativesEnabledScope(ModuleSystem* module_system);
    ~NativesEnabledScope();

   private:
    ModuleSystem* module_system_;
    DISALLOW_COPY_AND_ASSIGN(NativesEnabledScope);
  };

  
  ModuleSystem(ChromeV8Context* context, SourceMap* source_map);
  virtual ~ModuleSystem();

  
  
  v8::Handle<v8::Value> Require(const std::string& module_name);
  void Require(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  
  v8::Local<v8::Value> CallModuleMethod(const std::string& module_name,
                                        const std::string& method_name);
  v8::Local<v8::Value> CallModuleMethod(
      const std::string& module_name,
      const std::string& method_name,
      std::vector<v8::Handle<v8::Value> >* args);
  v8::Local<v8::Value> CallModuleMethod(
      const std::string& module_name,
      const std::string& method_name,
      int argc,
      v8::Handle<v8::Value> argv[]);

  
  
  
  void RegisterNativeHandler(const std::string& name,
                             scoped_ptr<NativeHandler> native_handler);

  
  
  
  void OverrideNativeHandlerForTest(const std::string& name);

  
  void RunString(const std::string& code, const std::string& name);

  
  
  
  
  
  
  void SetLazyField(v8::Handle<v8::Object> object,
                    const std::string& field,
                    const std::string& module_name,
                    const std::string& module_field);

  void SetLazyField(v8::Handle<v8::Object> object,
                    const std::string& field,
                    const std::string& module_name,
                    const std::string& module_field,
                    v8::AccessorGetterCallback getter);

  
  
  
  void SetNativeLazyField(v8::Handle<v8::Object> object,
                          const std::string& field,
                          const std::string& module_name,
                          const std::string& module_field);

  
  void SetExceptionHandlerForTest(scoped_ptr<ExceptionHandler> handler) {
    exception_handler_ = handler.Pass();
  }

  v8::Isolate* GetIsolate() const;

 protected:
  friend class ChromeV8Context;
  virtual void Invalidate() OVERRIDE;

 private:
  typedef std::map<std::string, linked_ptr<NativeHandler> > NativeHandlerMap;

  
  static void LazyFieldGetter(v8::Local<v8::String> property,
                              const v8::PropertyCallbackInfo<v8::Value>& info);
  
  
  static void NativeLazyFieldGetter(
      v8::Local<v8::String> property,
      const v8::PropertyCallbackInfo<v8::Value>& info);

  
  void HandleException(const v8::TryCatch& try_catch);

  
  void EnsureRequireLoaded();

  
  
  v8::Handle<v8::Value> RunString(v8::Handle<v8::String> code,
                                  v8::Handle<v8::String> name);

  void RequireForJs(const v8::FunctionCallbackInfo<v8::Value>& args);
  v8::Local<v8::Value> RequireForJsInner(v8::Handle<v8::String> module_name);

  typedef v8::Handle<v8::Value> (ModuleSystem::*RequireFunction)(
      const std::string&);
  
  
  static void LazyFieldGetterInner(
      v8::Local<v8::String> property,
      const v8::PropertyCallbackInfo<v8::Value>& info,
      RequireFunction require_function);

  
  
  v8::Handle<v8::Value> GetSource(const std::string& module_name);

  
  
  
  v8::Handle<v8::Value> RequireNativeFromString(const std::string& native_name);
  void RequireNative(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  v8::Handle<v8::String> WrapSource(v8::Handle<v8::String> source);

  
  void Private(const v8::FunctionCallbackInfo<v8::Value>& args);

  ChromeV8Context* context_;

  
  
  SourceMap* source_map_;

  
  NativeHandlerMap native_handler_map_;

  
  
  int natives_enabled_;

  
  
  scoped_ptr<ExceptionHandler> exception_handler_;

  std::set<std::string> overridden_native_handlers_;

  DISALLOW_COPY_AND_ASSIGN(ModuleSystem);
};

}  

#endif  
