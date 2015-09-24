// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_RESOURCE_CONVERTER_H
#define CONTENT_RENDERER_PEPPER_RESOURCE_CONVERTER_H

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/renderer/pepper/host_resource_var.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "v8/include/v8.h"

namespace IPC {
class Message;
}

namespace ppapi {
class ScopedPPVar;
}

namespace content {

class RendererPpapiHost;

class CONTENT_EXPORT ResourceConverter {
 public:
  virtual ~ResourceConverter();

  
  
  virtual void Flush(const base::Callback<void(bool)>& callback) = 0;

  
  
  
  
  virtual bool FromV8Value(v8::Handle<v8::Object> val,
                           v8::Handle<v8::Context> context,
                           PP_Var* result,
                           bool* was_resource) = 0;
};

class ResourceConverterImpl : public ResourceConverter {
 public:
  ResourceConverterImpl(PP_Instance instance, RendererPpapiHost* host);
  virtual ~ResourceConverterImpl();

  
  virtual void Flush(const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual bool FromV8Value(v8::Handle<v8::Object> val,
                           v8::Handle<v8::Context> context,
                           PP_Var* result,
                           bool* was_resource) OVERRIDE;

 private:
  
  
  scoped_refptr<HostResourceVar> CreateResourceVar(
      int pending_renderer_id,
      const IPC::Message& create_message);
  
  
  
  
  scoped_refptr<HostResourceVar> CreateResourceVarWithBrowserHost(
      int pending_renderer_id,
      const IPC::Message& create_message,
      const IPC::Message& browser_host_create_message);

  
  PP_Instance instance_;
  
  RendererPpapiHost* host_;

  
  
  
  std::vector<IPC::Message> browser_host_create_messages_;
  
  std::vector<scoped_refptr<HostResourceVar> > browser_vars;

  DISALLOW_COPY_AND_ASSIGN(ResourceConverterImpl);
};

}  
#endif  
