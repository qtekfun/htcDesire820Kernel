// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_NPOBJECT_PROXY_H_
#define CONTENT_CHILD_NPAPI_NPOBJECT_PROXY_H_

#include "base/memory/ref_counted.h"
#include "content/child/npapi/npobject_base.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "third_party/npapi/bindings/npruntime.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

struct NPObject;

namespace content {
class NPChannelBase;

class NPObjectProxy : public IPC::Listener,
                      public IPC::Sender,
                      public NPObjectBase {
 public:
  virtual ~NPObjectProxy();

  static NPObject* Create(NPChannelBase* channel,
                          int route_id,
                          int render_view_id,
                          const GURL& page_url,
                          NPP owner);

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;
  int route_id() { return route_id_; }
  NPChannelBase* channel() { return channel_.get(); }

  
  static bool NPHasMethod(NPObject *obj,
                          NPIdentifier name);
  static bool NPInvoke(NPObject *obj,
                       NPIdentifier name,
                       const NPVariant *args,
                       uint32_t arg_count,
                       NPVariant *result);
  static bool NPInvokeDefault(NPObject *npobj,
                              const NPVariant *args,
                              uint32_t arg_count,
                              NPVariant *result);
  static bool NPHasProperty(NPObject *obj,
                            NPIdentifier name);
  static bool NPGetProperty(NPObject *obj,
                            NPIdentifier name,
                            NPVariant *result);
  static bool NPSetProperty(NPObject *obj,
                            NPIdentifier name,
                            const NPVariant *value);
  static bool NPRemoveProperty(NPObject *obj,
                               NPIdentifier name);
  static bool NPNEnumerate(NPObject *obj,
                           NPIdentifier **value,
                           uint32_t *count);
  static bool NPNConstruct(NPObject *npobj,
                           const NPVariant *args,
                           uint32_t arg_count,
                           NPVariant *result);

  
  static bool NPNEvaluate(NPP npp,
                          NPObject *obj,
                          NPString *script,
                          NPVariant *result);

  static bool NPInvokePrivate(NPP npp,
                              NPObject *obj,
                              bool is_default,
                              NPIdentifier name,
                              const NPVariant *args,
                              uint32_t arg_count,
                              NPVariant *result);

  static NPObjectProxy* GetProxy(NPObject* object);
  static const NPClass* npclass() { return &npclass_proxy_; }

  
  virtual NPObject* GetUnderlyingNPObject() OVERRIDE;

  virtual IPC::Listener* GetChannelListener() OVERRIDE;

 private:
  NPObjectProxy(NPChannelBase* channel,
                int route_id,
                int render_view_id,
                const GURL& page_url);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  static NPObject* NPAllocate(NPP, NPClass*);
  static void NPDeallocate(NPObject* npObj);

  
  static void NPPInvalidate(NPObject *obj);
  static NPClass npclass_proxy_;

  scoped_refptr<NPChannelBase> channel_;
  int route_id_;
  int render_view_id_;

  
  GURL page_url_;
};

}  

#endif  
