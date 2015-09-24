// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PPAPI_PLUGIN_PPAPI_THREAD_H_
#define CONTENT_PPAPI_PLUGIN_PPAPI_THREAD_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "base/scoped_native_library.h"
#include "build/build_config.h"
#include "content/child/child_thread.h"
#include "content/public/common/pepper_plugin_info.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/trusted/ppp_broker.h"
#include "ppapi/proxy/connection.h"
#include "ppapi/proxy/plugin_dispatcher.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/plugin_proxy_delegate.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif

class CommandLine;

namespace base {
class FilePath;
}

namespace IPC {
struct ChannelHandle;
}

namespace content {

class PpapiWebKitPlatformSupportImpl;

class PpapiThread : public ChildThread,
                    public ppapi::proxy::PluginDispatcher::PluginDelegate,
                    public ppapi::proxy::PluginProxyDelegate {
 public:
  PpapiThread(const CommandLine& command_line, bool is_broker);
  virtual ~PpapiThread();
  virtual void Shutdown() OVERRIDE;

 private:
  
  
  enum LoadResult {
    LOAD_SUCCESS,
    LOAD_FAILED,
    ENTRY_POINT_MISSING,
    INIT_FAILED,
    
    LOAD_RESULT_MAX  
  };

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;
  virtual bool OnControlMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;

  
  virtual std::set<PP_Instance>* GetGloballySeenInstanceIDSet() OVERRIDE;
  virtual base::MessageLoopProxy* GetIPCMessageLoop() OVERRIDE;
  virtual base::WaitableEvent* GetShutdownEvent() OVERRIDE;
  virtual IPC::PlatformFileForTransit ShareHandleWithRemote(
      base::PlatformFile handle,
      base::ProcessId peer_pid,
      bool should_close_source) OVERRIDE;
  virtual uint32 Register(
      ppapi::proxy::PluginDispatcher* plugin_dispatcher) OVERRIDE;
  virtual void Unregister(uint32 plugin_dispatcher_id) OVERRIDE;

  
  
  
  virtual IPC::Sender* GetBrowserSender() OVERRIDE;
  virtual std::string GetUILanguage() OVERRIDE;
  virtual void PreCacheFont(const void* logfontw) OVERRIDE;
  virtual void SetActiveURL(const std::string& url) OVERRIDE;
  virtual PP_Resource CreateBrowserFont(
      ppapi::proxy::Connection connection,
      PP_Instance instance,
      const PP_BrowserFont_Trusted_Description& desc,
      const ppapi::Preferences& prefs) OVERRIDE;

  
  void OnLoadPlugin(const base::FilePath& path,
                    const ppapi::PpapiPermissions& permissions,
                    bool supports_dev_channel);
  void OnCreateChannel(base::ProcessId renderer_pid,
                       int renderer_child_id,
                       bool incognito);
  void OnSetNetworkState(bool online);
  void OnCrash();
  void OnHang();

  
  
  bool SetupRendererChannel(base::ProcessId renderer_pid,
                            int renderer_child_id,
                            bool incognito,
                            IPC::ChannelHandle* handle);

  
  void SavePluginName(const base::FilePath& path);

  void ReportLoadResult(const base::FilePath& path, LoadResult result);

  
  bool is_broker_;

  base::ScopedNativeLibrary library_;

  ppapi::PpapiPermissions permissions_;

  
  ppapi::proxy::PluginGlobals plugin_globals_;

  
  PepperPluginInfo::EntryPoints plugin_entry_points_;

  
  
  PP_ConnectInstance_Func connect_instance_func_;

  
  
  
  
  
  
  PP_Module local_pp_module_;

  
  std::set<PP_Instance> globally_seen_instance_ids_;

  
  std::map<uint32, ppapi::proxy::PluginDispatcher*> plugin_dispatchers_;
  uint32 next_plugin_dispatcher_id_;

  
  scoped_ptr<PpapiWebKitPlatformSupportImpl> webkit_platform_support_;

#if defined(OS_WIN)
  
  base::win::ScopedHandle peer_handle_;
#endif

  DISALLOW_IMPLICIT_CONSTRUCTORS(PpapiThread);
};

}  

#endif  
