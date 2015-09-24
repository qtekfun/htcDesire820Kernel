// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PLUGIN_PLUGIN_CHANNEL_H_
#define CONTENT_PLUGIN_PLUGIN_CHANNEL_H_

#include <vector>
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_handle.h"
#include "base/process/process.h"
#include "build/build_config.h"
#include "content/child/npapi/np_channel_base.h"
#include "content/child/scoped_child_process_reference.h"
#include "content/plugin/webplugin_delegate_stub.h"

namespace base {
class WaitableEvent;
}

namespace content {

class PluginChannel : public NPChannelBase {
 public:
  
  
  
  static PluginChannel* GetPluginChannel(
      int renderer_id, base::MessageLoopProxy* ipc_message_loop);

  
  static void NotifyRenderersOfPendingShutdown();

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  int renderer_id() { return renderer_id_; }

  virtual int GenerateRouteID() OVERRIDE;

  
  
  virtual base::WaitableEvent* GetModalDialogEvent(int render_view_id) OVERRIDE;

  bool in_send() { return in_send_ != 0; }

  bool incognito() { return incognito_; }
  void set_incognito(bool value) { incognito_ = value; }

#if defined(OS_POSIX)
  int TakeRendererFileDescriptor() {
    return channel_->TakeClientFileDescriptor();
  }
#endif

 protected:
  virtual ~PluginChannel();

  
  virtual void CleanUp() OVERRIDE;
  virtual bool Init(base::MessageLoopProxy* ipc_message_loop,
                    bool create_pipe_now,
                    base::WaitableEvent* shutdown_event) OVERRIDE;

 private:
  class MessageFilter;

  
  PluginChannel();

  virtual bool OnControlMessageReceived(const IPC::Message& msg) OVERRIDE;

  static NPChannelBase* ClassFactory() { return new PluginChannel(); }

  void OnCreateInstance(const std::string& mime_type, int* instance_id);
  void OnDestroyInstance(int instance_id, IPC::Message* reply_msg);
  void OnGenerateRouteID(int* route_id);
  void OnClearSiteData(const std::string& site,
                       uint64 flags,
                       uint64 max_age);
  void OnDidAbortLoading(int render_view_id);

  std::vector<scoped_refptr<WebPluginDelegateStub> > plugin_stubs_;

  ScopedChildProcessReference process_ref_;

  
  int renderer_id_;

  int in_send_;  
  bool log_messages_;  
  bool incognito_; 
  scoped_refptr<MessageFilter> filter_;  

  
  
  scoped_ptr<struct _NPP> npp_;

  DISALLOW_COPY_AND_ASSIGN(PluginChannel);
};

}  

#endif  
