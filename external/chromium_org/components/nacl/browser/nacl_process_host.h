// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_PROCESS_HOST_H_
#define COMPONENTS_NACL_BROWSER_NACL_PROCESS_HOST_H_

#include "build/build_config.h"

#include "base/files/file_path.h"
#include "base/files/file_util_proxy.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/process/process.h"
#include "components/nacl/common/nacl_types.h"
#include "content/public/browser/browser_child_process_host_delegate.h"
#include "content/public/browser/browser_child_process_host_iterator.h"
#include "ipc/ipc_channel_handle.h"
#include "net/socket/socket_descriptor.h"
#include "ppapi/shared_impl/ppapi_permissions.h"
#include "url/gurl.h"

class CommandLine;

namespace content {
class BrowserChildProcessHost;
class BrowserPpapiHost;
}

namespace IPC {
class ChannelProxy;
}

namespace nacl {

class NaClHostMessageFilter;
void* AllocateAddressSpaceASLR(base::ProcessHandle process, size_t size);

class NaClProcessHost : public content::BrowserChildProcessHostDelegate {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  NaClProcessHost(const GURL& manifest_url,
                  int render_view_id,
                  uint32 permission_bits,
                  bool uses_irt,
                  bool enable_dyncode_syscalls,
                  bool enable_exception_handling,
                  bool enable_crash_throttling,
                  bool off_the_record,
                  const base::FilePath& profile_directory);
  virtual ~NaClProcessHost();

  virtual void OnProcessCrashed(int exit_status) OVERRIDE;

  
  static void EarlyStartup();

  
  
  void Launch(NaClHostMessageFilter* nacl_host_message_filter,
              IPC::Message* reply_msg,
              const base::FilePath& manifest_path);

  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;

#if defined(OS_WIN)
  void OnProcessLaunchedByBroker(base::ProcessHandle handle);
  void OnDebugExceptionHandlerLaunchedByBroker(bool success);
#endif

  bool Send(IPC::Message* msg);

  content::BrowserChildProcessHost* process() { return process_.get(); }
  content::BrowserPpapiHost* browser_ppapi_host() { return ppapi_host_.get(); }

 private:
  friend class PluginListener;

  
  
  
  
  struct NaClInternal;

  
  
  class PluginListener : public IPC::Listener {
   public:
    explicit PluginListener(NaClProcessHost* host);
    virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
   private:
    
    NaClProcessHost* host_;
  };

  bool LaunchNaClGdb();

#if defined(OS_POSIX)
  
  
  
  net::SocketDescriptor GetDebugStubSocketHandle();
#endif
  bool LaunchSelLdr();

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnProcessLaunched() OVERRIDE;

  void OnResourcesReady();

  
  bool enable_ppapi_proxy() { return render_view_id_ != 0; }

  
  
  bool ReplyToRenderer(const IPC::ChannelHandle& channel_handle);

  
  void SendErrorToRenderer(const std::string& error_message);

  
  
  void SendMessageToRenderer(const NaClLaunchResult& result,
                             const std::string& error_message);

  
  
  bool StartNaClExecution();

  
  
  bool SendStart();

  
  
  
  
  bool StartWithLaunchedProcess();

  
  void OnQueryKnownToValidate(const std::string& signature, bool* result);
  void OnSetKnownToValidate(const std::string& signature);
  void OnResolveFileToken(uint64 file_token_lo, uint64 file_token_hi,
                          IPC::Message* reply_msg);
  void FileResolved(const base::FilePath& file_path,
                    IPC::Message* reply_msg,
                    const base::PlatformFile& file);

#if defined(OS_WIN)
  
  void OnAttachDebugExceptionHandler(const std::string& info,
                                     IPC::Message* reply_msg);
  bool AttachDebugExceptionHandler(const std::string& info,
                                   IPC::Message* reply_msg);
#endif

  
  void OnPpapiChannelCreated(const IPC::ChannelHandle& channel_handle);
  
  
  bool OnUntrustedMessageForwarded(const IPC::Message& msg);

  GURL manifest_url_;
  ppapi::PpapiPermissions permissions_;

#if defined(OS_WIN)
  
  
  bool process_launched_by_broker_;
#endif
  
  
  scoped_refptr<NaClHostMessageFilter> nacl_host_message_filter_;

  
  
  
  IPC::Message* reply_msg_;
#if defined(OS_WIN)
  bool debug_exception_handler_requested_;
  scoped_ptr<IPC::Message> attach_debug_exception_handler_reply_msg_;
#endif

  
  
  base::FilePath manifest_path_;

  
  scoped_ptr<NaClInternal> internal_;

  base::WeakPtrFactory<NaClProcessHost> weak_factory_;

  scoped_ptr<content::BrowserChildProcessHost> process_;

  bool uses_irt_;

  bool enable_debug_stub_;
  bool enable_dyncode_syscalls_;
  bool enable_exception_handling_;
  bool enable_crash_throttling_;

  bool off_the_record_;

  const base::FilePath profile_directory_;

  
  scoped_ptr<IPC::ChannelProxy> ipc_proxy_channel_;
  
  PluginListener ipc_plugin_listener_;
  
  scoped_ptr<content::BrowserPpapiHost> ppapi_host_;

  int render_view_id_;

  DISALLOW_COPY_AND_ASSIGN(NaClProcessHost);
};

}  

#endif  
