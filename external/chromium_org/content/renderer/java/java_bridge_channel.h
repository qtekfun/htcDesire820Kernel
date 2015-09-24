// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_JAVA_JAVA_BRIDGE_CHANNEL_H_
#define CONTENT_RENDERER_JAVA_JAVA_BRIDGE_CHANNEL_H_

#include "base/memory/scoped_ptr.h"
#include "content/child/npapi/np_channel_base.h"
#include "ipc/ipc_channel_handle.h"

namespace content {

class JavaBridgeChannel : public content::NPChannelBase {
 public:
  
  static JavaBridgeChannel* GetJavaBridgeChannel(
      const IPC::ChannelHandle& channel_handle,
      base::MessageLoopProxy* ipc_message_loop);

  
  virtual int GenerateRouteID() OVERRIDE;

  
  virtual bool OnControlMessageReceived(const IPC::Message& msg) OVERRIDE;

 private:
  JavaBridgeChannel();
  
  virtual ~JavaBridgeChannel();

  static NPChannelBase* ClassFactory() { return new JavaBridgeChannel(); }

  
  
  scoped_ptr<struct _NPP> peer_owner_id_;

  DISALLOW_COPY_AND_ASSIGN(JavaBridgeChannel);
};

}  

#endif  
