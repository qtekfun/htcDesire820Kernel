// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/simple_thread.h"
#include "base/threading/thread.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/proxy/host_dispatcher.h"
#include "ppapi/proxy/plugin_dispatcher.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/plugin_proxy_delegate.h"
#include "ppapi/proxy/plugin_resource_tracker.h"
#include "ppapi/proxy/plugin_var_tracker.h"
#include "ppapi/proxy/resource_message_test_sink.h"
#include "ppapi/shared_impl/test_globals.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
class MessageLoopProxy;
class RunLoop;
}

namespace ppapi {
namespace proxy {

class MessageLoopResource;

class ProxyTestHarnessBase {
 public:
  enum GlobalsConfiguration {
    PER_THREAD_GLOBALS,
    SINGLETON_GLOBALS
  };

  ProxyTestHarnessBase();
  virtual ~ProxyTestHarnessBase();

  PP_Module pp_module() const { return pp_module_; }
  PP_Instance pp_instance() const { return pp_instance_; }
  ResourceMessageTestSink& sink() { return sink_; }

  virtual PpapiGlobals* GetGlobals() = 0;
  
  virtual Dispatcher* GetDispatcher() = 0;

  
  virtual void SetUpHarness() = 0;

  
  virtual void SetUpHarnessWithChannel(const IPC::ChannelHandle& channel_handle,
                                       base::MessageLoopProxy* ipc_message_loop,
                                       base::WaitableEvent* shutdown_event,
                                       bool is_client) = 0;

  virtual void TearDownHarness() = 0;

  
  
  
  const void* GetInterface(const char* name);

  
  
  
  void RegisterTestInterface(const char* name, const void* test_interface);

  
  
  bool SupportsInterface(const char* name);

 private:
  
  ResourceMessageTestSink sink_;

  
  PP_Module pp_module_;
  PP_Instance pp_instance_;

  
  std::map<std::string, const void*> registered_interfaces_;
};

class PluginProxyTestHarness : public ProxyTestHarnessBase {
 public:
  explicit PluginProxyTestHarness(GlobalsConfiguration globals_config);
  virtual ~PluginProxyTestHarness();

  PluginDispatcher* plugin_dispatcher() { return plugin_dispatcher_.get(); }
  PluginResourceTracker& resource_tracker() {
    return *plugin_globals_->plugin_resource_tracker();
  }
  PluginVarTracker& var_tracker() {
    return *plugin_globals_->plugin_var_tracker();
  }

  
  virtual PpapiGlobals* GetGlobals();
  virtual Dispatcher* GetDispatcher();
  virtual void SetUpHarness();
  virtual void SetUpHarnessWithChannel(const IPC::ChannelHandle& channel_handle,
                                       base::MessageLoopProxy* ipc_message_loop,
                                       base::WaitableEvent* shutdown_event,
                                       bool is_client);
  virtual void TearDownHarness();

  class PluginDelegateMock : public PluginDispatcher::PluginDelegate,
                             public PluginProxyDelegate {
   public:
    PluginDelegateMock() : ipc_message_loop_(NULL), shutdown_event_() {}
    virtual ~PluginDelegateMock() {}

    void Init(base::MessageLoopProxy* ipc_message_loop,
              base::WaitableEvent* shutdown_event) {
      ipc_message_loop_ = ipc_message_loop;
      shutdown_event_ = shutdown_event;
    }

    void set_browser_sender(IPC::Sender* browser_sender) {
      browser_sender_ = browser_sender;
    }

    
    virtual base::MessageLoopProxy* GetIPCMessageLoop() OVERRIDE;
    virtual base::WaitableEvent* GetShutdownEvent() OVERRIDE;
    virtual IPC::PlatformFileForTransit ShareHandleWithRemote(
        base::PlatformFile handle,
        base::ProcessId remote_pid,
        bool should_close_source) OVERRIDE;

    
    virtual std::set<PP_Instance>* GetGloballySeenInstanceIDSet() OVERRIDE;
    virtual uint32 Register(PluginDispatcher* plugin_dispatcher) OVERRIDE;
    virtual void Unregister(uint32 plugin_dispatcher_id) OVERRIDE;

    
    virtual IPC::Sender* GetBrowserSender() OVERRIDE;
    virtual std::string GetUILanguage() OVERRIDE;
    virtual void PreCacheFont(const void* logfontw) OVERRIDE;
    virtual void SetActiveURL(const std::string& url) OVERRIDE;
    virtual PP_Resource CreateBrowserFont(
        Connection connection,
        PP_Instance instance,
        const PP_BrowserFont_Trusted_Description& desc,
        const Preferences& prefs) OVERRIDE;

   private:
    base::MessageLoopProxy* ipc_message_loop_;  
    base::WaitableEvent* shutdown_event_;  
    std::set<PP_Instance> instance_id_set_;
    IPC::Sender* browser_sender_;

    DISALLOW_COPY_AND_ASSIGN(PluginDelegateMock);
  };

 private:
  void CreatePluginGlobals();

  GlobalsConfiguration globals_config_;
  scoped_ptr<PluginGlobals> plugin_globals_;

  scoped_ptr<PluginDispatcher> plugin_dispatcher_;
  PluginDelegateMock plugin_delegate_mock_;
};

class PluginProxyTest : public PluginProxyTestHarness, public testing::Test {
 public:
  PluginProxyTest();
  virtual ~PluginProxyTest();

  
  virtual void SetUp();
  virtual void TearDown();
 private:
  base::MessageLoop message_loop_;
};

class PluginProxyMultiThreadTest
    : public PluginProxyTest,
      public base::DelegateSimpleThread::Delegate {
 public:
  PluginProxyMultiThreadTest();
  virtual ~PluginProxyMultiThreadTest();

  
  
  
  virtual void SetUpTestOnMainThread() = 0;

  virtual void SetUpTestOnSecondaryThread() = 0;

  
  void RunTest();

  enum ThreadType {
    MAIN_THREAD,
    SECONDARY_THREAD
  };
  void CheckOnThread(ThreadType thread_type);

  
  void PostQuitForMainThread();
  void PostQuitForSecondaryThread();

 protected:
  scoped_refptr<MessageLoopResource> secondary_thread_message_loop_;
  scoped_refptr<base::MessageLoopProxy> main_thread_message_loop_proxy_;

 private:
  
  virtual void Run() OVERRIDE;

  void QuitNestedLoop();

  static void InternalSetUpTestOnSecondaryThread(void* user_data,
                                                 int32_t result);

  scoped_ptr<base::DelegateSimpleThread> secondary_thread_;
  scoped_ptr<base::RunLoop> nested_main_thread_message_loop_;
};

class HostProxyTestHarness : public ProxyTestHarnessBase {
 public:
  explicit HostProxyTestHarness(GlobalsConfiguration globals_config);
  virtual ~HostProxyTestHarness();

  HostDispatcher* host_dispatcher() { return host_dispatcher_.get(); }
  ResourceTracker& resource_tracker() {
    return *host_globals_->GetResourceTracker();
  }
  VarTracker& var_tracker() {
    return *host_globals_->GetVarTracker();
  }

  
  virtual PpapiGlobals* GetGlobals();
  virtual Dispatcher* GetDispatcher();
  virtual void SetUpHarness();
  virtual void SetUpHarnessWithChannel(const IPC::ChannelHandle& channel_handle,
                                       base::MessageLoopProxy* ipc_message_loop,
                                       base::WaitableEvent* shutdown_event,
                                       bool is_client);
  virtual void TearDownHarness();

  class DelegateMock : public ProxyChannel::Delegate {
   public:
    DelegateMock() : ipc_message_loop_(NULL), shutdown_event_(NULL) {
    }
    virtual ~DelegateMock() {}

    void Init(base::MessageLoopProxy* ipc_message_loop,
              base::WaitableEvent* shutdown_event) {
      ipc_message_loop_ = ipc_message_loop;
      shutdown_event_ = shutdown_event;
    }

    
    virtual base::MessageLoopProxy* GetIPCMessageLoop();
    virtual base::WaitableEvent* GetShutdownEvent();
    virtual IPC::PlatformFileForTransit ShareHandleWithRemote(
        base::PlatformFile handle,
        base::ProcessId remote_pid,
        bool should_close_source) OVERRIDE;

   private:
    base::MessageLoopProxy* ipc_message_loop_;  
    base::WaitableEvent* shutdown_event_;  

    DISALLOW_COPY_AND_ASSIGN(DelegateMock);
  };

 private:
  class MockSyncMessageStatusReceiver;

  void CreateHostGlobals();

  GlobalsConfiguration globals_config_;
  scoped_ptr<ppapi::TestGlobals> host_globals_;
  scoped_ptr<HostDispatcher> host_dispatcher_;
  DelegateMock delegate_mock_;

  scoped_ptr<MockSyncMessageStatusReceiver> status_receiver_;
};

class HostProxyTest : public HostProxyTestHarness, public testing::Test {
 public:
  HostProxyTest();
  virtual ~HostProxyTest();

  
  virtual void SetUp();
  virtual void TearDown();
 private:
  base::MessageLoop message_loop_;
};

class TwoWayTest : public testing::Test {
 public:
  enum TwoWayTestMode {
    TEST_PPP_INTERFACE,
    TEST_PPB_INTERFACE
  };
  TwoWayTest(TwoWayTestMode test_mode);
  virtual ~TwoWayTest();

  HostProxyTestHarness& host() { return host_; }
  PluginProxyTestHarness& plugin() { return plugin_; }
  PP_Module pp_module() const { return host_.pp_module(); }
  PP_Instance pp_instance() const { return host_.pp_instance(); }
  TwoWayTestMode test_mode() { return test_mode_; }

  
  virtual void SetUp();
  virtual void TearDown();

 protected:
  
  
  
  void PostTaskOnRemoteHarness(const base::Closure& task);

 private:
  TwoWayTestMode test_mode_;
  HostProxyTestHarness host_;
  PluginProxyTestHarness plugin_;
  
  base::Thread io_thread_;
  
  base::Thread plugin_thread_;
  
  base::MessageLoop message_loop_;

  
  
  
  
  ProxyTestHarnessBase* remote_harness_;
  ProxyTestHarnessBase* local_harness_;

  base::WaitableEvent channel_created_;
  base::WaitableEvent shutdown_event_;
};

#define EXPECT_VAR_IS_STRING(str, var) { \
  StringVar* sv = StringVar::FromPPVar(var); \
  EXPECT_TRUE(sv); \
  if (sv) \
    EXPECT_EQ(str, sv->value()); \
}

}  
}  
