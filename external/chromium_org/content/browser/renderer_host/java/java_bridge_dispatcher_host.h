// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_BRIDGE_DISPATCHER_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_BRIDGE_DISPATCHER_HOST_H_

#include <vector>
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "content/child/npapi/npobject_stub.h"

class RouteIDGenerator;
struct NPObject;

namespace IPC {
class Message;
}

namespace content {
class NPChannelBase;
class RenderViewHost;
struct NPVariant_Param;

class JavaBridgeDispatcherHost
    : public base::RefCountedThreadSafe<JavaBridgeDispatcherHost> {
 public:
  
  explicit JavaBridgeDispatcherHost(RenderViewHost* render_view_host);

  
  
  
  
  
  
  
  
  
  void AddNamedObject(const base::string16& name, NPObject* object);
  void RemoveNamedObject(const base::string16& name);

  
  void RenderViewDeleted();

  void OnGetChannelHandle(IPC::Message* reply_msg);

 private:
  friend class base::RefCountedThreadSafe<JavaBridgeDispatcherHost>;
  virtual ~JavaBridgeDispatcherHost();

  void Send(IPC::Message* msg);

  void GetChannelHandle(IPC::Message* reply_msg);
  void CreateNPVariantParam(NPObject* object, NPVariant_Param* param);
  void CreateObjectStub(NPObject* object, int render_process_id, int route_id);

  scoped_refptr<NPChannelBase> channel_;
  RenderViewHost* render_view_host_;
  std::vector<base::WeakPtr<NPObjectStub> > stubs_;

  DISALLOW_COPY_AND_ASSIGN(JavaBridgeDispatcherHost);
};

}  

#endif  
