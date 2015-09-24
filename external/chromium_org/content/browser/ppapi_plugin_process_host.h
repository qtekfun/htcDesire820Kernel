// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_PPAPI_PLUGIN_PROCESS_HOST_H_
#define CONTENT_BROWSER_PPAPI_PLUGIN_PROCESS_HOST_H_

#include <queue>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "content/browser/renderer_host/pepper/browser_ppapi_host_impl.h"
#include "content/browser/renderer_host/pepper/pepper_message_filter.h"
#include "content/public/browser/browser_child_process_host_delegate.h"
#include "content/public/browser/browser_child_process_host_iterator.h"
#include "ipc/ipc_sender.h"
#include "ppapi/shared_impl/ppapi_permissions.h"

namespace content {
class BrowserChildProcessHostImpl;
class ResourceContext;
struct PepperPluginInfo;

class PpapiPluginProcessHost : public BrowserChildProcessHostDelegate,
                               public IPC::Sender {
 public:
  class Client {
   public:
    
    virtual void GetPpapiChannelInfo(base::ProcessHandle* renderer_handle,
                                     int* renderer_id) = 0;

    
    
    
    
    
    virtual void OnPpapiChannelOpened(
        const IPC::ChannelHandle& channel_handle,
        base::ProcessId plugin_pid,
        int plugin_child_id) = 0;

    
    virtual bool OffTheRecord() = 0;

   protected:
    virtual ~Client() {}
  };

  class PluginClient : public Client {
   public:
    
    virtual ResourceContext* GetResourceContext() = 0;

   protected:
    virtual ~PluginClient() {}
  };

  class BrokerClient : public Client {
   protected:
    virtual ~BrokerClient() {}
  };

  virtual ~PpapiPluginProcessHost();

  static PpapiPluginProcessHost* CreatePluginHost(
      const PepperPluginInfo& info,
      const base::FilePath& profile_data_directory);
  static PpapiPluginProcessHost* CreateBrokerHost(
      const PepperPluginInfo& info);

  
  
  
  
  
  static void DidCreateOutOfProcessInstance(
      int plugin_process_id,
      int32 pp_instance,
      const PepperRendererInstanceData& instance_data);

  
  static void DidDeleteOutOfProcessInstance(int plugin_process_id,
                                            int32 pp_instance);

  
  
  static void FindByName(const base::string16& name,
                         std::vector<PpapiPluginProcessHost*>* hosts);

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  void OpenChannelToPlugin(Client* client);

  BrowserPpapiHostImpl* host_impl() { return host_impl_.get(); }
  const BrowserChildProcessHostImpl* process() { return process_.get(); }
  const base::FilePath& plugin_path() const { return plugin_path_; }
  const base::FilePath& profile_data_directory() const {
    return profile_data_directory_;
  }

  

 private:
  class PluginNetworkObserver;

  
  
  PpapiPluginProcessHost(const PepperPluginInfo& info,
                         const base::FilePath& profile_data_directory);
  PpapiPluginProcessHost();

  
  
  bool Init(const PepperPluginInfo& info);

  void RequestPluginChannel(Client* client);

  virtual void OnProcessLaunched() OVERRIDE;

  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  void CancelRequests();

  
  void OnRendererPluginChannelCreated(const IPC::ChannelHandle& handle);

  
  scoped_refptr<PepperMessageFilter> filter_;

  ppapi::PpapiPermissions permissions_;
  scoped_ptr<BrowserPpapiHostImpl> host_impl_;

  
  scoped_ptr<PluginNetworkObserver> network_observer_;

  
  
  std::vector<Client*> pending_requests_;

  
  
  std::queue<Client*> sent_requests_;

  
  base::FilePath plugin_path_;

  
  base::FilePath profile_data_directory_;

  const bool is_broker_;

  scoped_ptr<BrowserChildProcessHostImpl> process_;

  DISALLOW_COPY_AND_ASSIGN(PpapiPluginProcessHost);
};

class PpapiPluginProcessHostIterator
    : public BrowserChildProcessHostTypeIterator<
          PpapiPluginProcessHost> {
 public:
  PpapiPluginProcessHostIterator()
      : BrowserChildProcessHostTypeIterator<
          PpapiPluginProcessHost>(PROCESS_TYPE_PPAPI_PLUGIN) {}
};

class PpapiBrokerProcessHostIterator
    : public BrowserChildProcessHostTypeIterator<
          PpapiPluginProcessHost> {
 public:
  PpapiBrokerProcessHostIterator()
      : BrowserChildProcessHostTypeIterator<
          PpapiPluginProcessHost>(PROCESS_TYPE_PPAPI_BROKER) {}
};

}  

#endif  

