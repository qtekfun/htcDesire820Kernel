// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_JAVA_JAVA_BRIDGE_DISPATCHER_H_
#define CONTENT_RENDERER_JAVA_JAVA_BRIDGE_DISPATCHER_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "content/public/renderer/render_view_observer.h"
#include "ipc/ipc_channel_handle.h"
#include "third_party/npapi/bindings/npruntime.h"

namespace content {
class JavaBridgeChannel;
struct NPVariant_Param;

class JavaBridgeDispatcher : public RenderViewObserver {
 public:
  JavaBridgeDispatcher(RenderView* render_view);
  virtual ~JavaBridgeDispatcher();

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidClearWindowObject(blink::WebFrame* frame) OVERRIDE;

  
  void OnAddNamedObject(const base::string16& name,
                        const NPVariant_Param& variant_param);
  void OnRemoveNamedObject(const base::string16& name);

  void EnsureChannelIsSetUp();

  
  
  typedef std::map<base::string16, NPVariant> ObjectMap;
  ObjectMap objects_;
  scoped_refptr<JavaBridgeChannel> channel_;
};

}  

#endif  
