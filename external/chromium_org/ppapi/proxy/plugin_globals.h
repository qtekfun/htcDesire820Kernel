// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_GLOBALS_H_
#define PPAPI_PROXY_PLUGIN_GLOBALS_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread_local_storage.h"
#include "ppapi/proxy/connection.h"
#include "ppapi/proxy/plugin_resource_tracker.h"
#include "ppapi/proxy/plugin_var_tracker.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/callback_tracker.h"
#include "ppapi/shared_impl/ppapi_globals.h"

namespace base {
class Thread;
}
namespace IPC {
class Sender;
}

struct PP_BrowserFont_Trusted_Description;

namespace ppapi {

struct Preferences;

namespace proxy {

class MessageLoopResource;
class PluginProxyDelegate;
class ResourceReplyThreadRegistrar;

class PPAPI_PROXY_EXPORT PluginGlobals : public PpapiGlobals {
 public:
  PluginGlobals();
  explicit PluginGlobals(PpapiGlobals::PerThreadForTest);
  virtual ~PluginGlobals();

  
  
  
  inline static PluginGlobals* Get() {
    
    
    CHECK(PpapiGlobals::Get()->IsPluginGlobals());
    return static_cast<PluginGlobals*>(PpapiGlobals::Get());
  }

  
  virtual ResourceTracker* GetResourceTracker() OVERRIDE;
  virtual VarTracker* GetVarTracker() OVERRIDE;
  virtual CallbackTracker* GetCallbackTrackerForInstance(
      PP_Instance instance) OVERRIDE;
  virtual thunk::PPB_Instance_API* GetInstanceAPI(
      PP_Instance instance) OVERRIDE;
  virtual thunk::ResourceCreationAPI* GetResourceCreationAPI(
      PP_Instance instance) OVERRIDE;
  virtual PP_Module GetModuleForInstance(PP_Instance instance) OVERRIDE;
  virtual std::string GetCmdLine() OVERRIDE;
  virtual void PreCacheFontForFlash(const void* logfontw) OVERRIDE;
  virtual void LogWithSource(PP_Instance instance,
                             PP_LogLevel level,
                             const std::string& source,
                             const std::string& value) OVERRIDE;
  virtual void BroadcastLogWithSource(PP_Module module,
                                      PP_LogLevel level,
                                      const std::string& source,
                                      const std::string& value) OVERRIDE;
  virtual MessageLoopShared* GetCurrentMessageLoop() OVERRIDE;
  base::TaskRunner* GetFileTaskRunner() OVERRIDE;

  
  IPC::Sender* GetBrowserSender();

  
  std::string GetUILanguage();

  
  void SetActiveURL(const std::string& url);

  PP_Resource CreateBrowserFont(
      Connection connection,
      PP_Instance instance,
      const PP_BrowserFont_Trusted_Description& desc,
      const Preferences& prefs);

  
  PluginResourceTracker* plugin_resource_tracker() {
    return &plugin_resource_tracker_;
  }
  PluginVarTracker* plugin_var_tracker() {
    return &plugin_var_tracker_;
  }

  
  void set_plugin_proxy_delegate(PluginProxyDelegate* d) {
    plugin_proxy_delegate_ = d;
  }

  
  
  
  
  base::ThreadLocalStorage::Slot* msg_loop_slot() {
    return msg_loop_slot_.get();
  }

  
  
  void set_msg_loop_slot(base::ThreadLocalStorage::Slot* slot) {
    msg_loop_slot_.reset(slot);
  }

  
  
  
  MessageLoopResource* loop_for_main_thread();

  
  
  void set_plugin_name(const std::string& name) { plugin_name_ = name; }

  
  void set_command_line(const std::string& c) { command_line_ = c; }

  ResourceReplyThreadRegistrar* resource_reply_thread_registrar() {
    return resource_reply_thread_registrar_.get();
  }

 private:
  class BrowserSender;

  
  virtual bool IsPluginGlobals() const OVERRIDE;

  static PluginGlobals* plugin_globals_;

  PluginProxyDelegate* plugin_proxy_delegate_;
  PluginResourceTracker plugin_resource_tracker_;
  PluginVarTracker plugin_var_tracker_;
  scoped_refptr<CallbackTracker> callback_tracker_;

  scoped_ptr<base::ThreadLocalStorage::Slot> msg_loop_slot_;
  
  
  scoped_refptr<MessageLoopResource> loop_for_main_thread_;

  
  
  std::string plugin_name_;

  
  
  std::string command_line_;

  scoped_ptr<BrowserSender> browser_sender_;

  
  
  scoped_ptr<base::Thread> file_thread_;

  scoped_refptr<ResourceReplyThreadRegistrar> resource_reply_thread_registrar_;

  DISALLOW_COPY_AND_ASSIGN(PluginGlobals);
};

}  
}  

#endif   
