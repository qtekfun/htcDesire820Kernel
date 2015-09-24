// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_CHROME_V8_CONTEXT_H_
#define CHROME_RENDERER_EXTENSIONS_CHROME_V8_CONTEXT_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/renderer/extensions/module_system.h"
#include "chrome/renderer/extensions/pepper_request_proxy.h"
#include "chrome/renderer/extensions/request_sender.h"
#include "chrome/renderer/extensions/safe_builtins.h"
#include "chrome/renderer/extensions/scoped_persistent.h"
#include "extensions/common/features/feature.h"
#include "v8/include/v8.h"

namespace blink {
class WebFrame;
}

namespace content {
class RenderView;
}

namespace extensions {
class Extension;

class ChromeV8Context : public RequestSender::Source {
 public:
  ChromeV8Context(v8::Handle<v8::Context> context,
                  blink::WebFrame* frame,
                  const Extension* extension,
                  Feature::Context context_type);
  virtual ~ChromeV8Context();

  
  
  void Invalidate();

  
  
  bool is_valid() const {
    return !v8_context_.IsEmpty();
  }

  v8::Handle<v8::Context> v8_context() const {
    return v8_context_.NewHandle(v8::Isolate::GetCurrent());
  }

  const Extension* extension() const {
    return extension_.get();
  }

  blink::WebFrame* web_frame() const {
    return web_frame_;
  }

  Feature::Context context_type() const {
    return context_type_;
  }

  void set_module_system(scoped_ptr<ModuleSystem> module_system) {
    module_system_ = module_system.Pass();
  }

  ModuleSystem* module_system() { return module_system_.get(); }

  SafeBuiltins* safe_builtins() {
    return &safe_builtins_;
  }
  const SafeBuiltins* safe_builtins() const {
    return &safe_builtins_;
  }

  PepperRequestProxy* pepper_request_proxy() {
    return &pepper_request_proxy_;
  }

  
  
  std::string GetExtensionID() const;

  
  
  content::RenderView* GetRenderView() const;

  
  GURL GetURL() const;

  
  
  
  
  v8::Local<v8::Value> CallFunction(v8::Handle<v8::Function> function,
                                    int argc,
                                    v8::Handle<v8::Value> argv[]) const;

  
  void DispatchOnUnloadEvent();

  
  Feature::Availability GetAvailability(const std::string& api_name);

  
  
  
  bool IsAnyFeatureAvailableToContext(const std::string& api_name);

  
  std::string GetContextTypeDescription();

  
  virtual ChromeV8Context* GetContext() OVERRIDE;
  virtual void OnResponseReceived(const std::string& name,
                                  int request_id,
                                  bool success,
                                  const base::ListValue& response,
                                  const std::string& error) OVERRIDE;

  v8::Isolate* isolate() const {
    return isolate_;
  }

 private:
  
  ScopedPersistent<v8::Context> v8_context_;

  
  
  blink::WebFrame* web_frame_;

  
  
  scoped_refptr<const Extension> extension_;

  
  Feature::Context context_type_;

  
  scoped_ptr<ModuleSystem> module_system_;

  
  SafeBuiltins safe_builtins_;

  
  PepperRequestProxy pepper_request_proxy_;

  v8::Isolate* isolate_;

  DISALLOW_COPY_AND_ASSIGN(ChromeV8Context);
};

}  

#endif  
