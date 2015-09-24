// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_AUTOMATION_PROXY_LAUNCHER_H_
#define CHROME_TEST_AUTOMATION_PROXY_LAUNCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "base/time/time.h"

class AutomationProxy;

class ProxyLauncher {
 public:
  
  static const char kDefaultInterfaceId[];

  
  enum ShutdownType {
    WINDOW_CLOSE,
    USER_QUIT,
    SESSION_ENDING,
  };

  
  struct LaunchState {
    
    bool clear_profile;

    
    
    base::FilePath template_user_data;

    
    
    base::Closure setup_profile_callback;

    
    CommandLine command;

    
    bool include_testing_id;

    
    bool show_window;
  };

  ProxyLauncher();

  virtual ~ProxyLauncher();

  
  
  virtual bool InitializeConnection(
      const LaunchState& state,
      bool wait_for_initial_loads) WARN_UNUSED_RESULT = 0;

  
  
  virtual void TerminateConnection() = 0;

  
  
  bool LaunchBrowserAndServer(const LaunchState& state,
                              bool wait_for_initial_loads) WARN_UNUSED_RESULT;

  
  
  
  bool ConnectToRunningBrowser(bool wait_for_initial_loads) WARN_UNUSED_RESULT;

  
  
  void CloseBrowserAndServer();

  
  
  bool LaunchBrowser(const LaunchState& state) WARN_UNUSED_RESULT;

  
  void QuitBrowser();

  
  void TerminateBrowser();

  
  
  void AssertAppNotRunning(const std::string& error_message);

  
  
  
  bool WaitForBrowserProcessToQuit(base::TimeDelta timeout, int* exit_code);

  AutomationProxy* automation() const;

  
  base::FilePath user_data_dir() const;

  
  
  
  base::ProcessHandle process() const;

  
  base::ProcessId process_id() const;

  
  base::TimeTicks browser_launch_time() const;

  
  base::TimeDelta browser_quit_time() const;

  
  void set_shutdown_type(ShutdownType value) {
    shutdown_type_ = value;
  }

 protected:
  
  virtual AutomationProxy* CreateAutomationProxy(
      base::TimeDelta execution_timeout) = 0;

  
  
  virtual std::string PrefixedChannelID() const = 0;

  
  
  void DisconnectFromRunningBrowser();

 private:
  bool WaitForBrowserLaunch(bool wait_for_initial_loads) WARN_UNUSED_RESULT;

  
  void PrepareTestCommandline(CommandLine* command_line,
                              bool include_testing_id);

  bool LaunchBrowserHelper(const LaunchState& state,
                           bool main_launch,
                           bool wait,
                           base::ProcessHandle* process) WARN_UNUSED_RESULT;

  scoped_ptr<AutomationProxy> automation_proxy_;

  
  
  base::ScopedTempDir temp_profile_dir_;

  
  base::ProcessHandle process_;

  
  base::ProcessId process_id_;

  
  base::TimeTicks browser_launch_time_;

  
  base::TimeDelta browser_quit_time_;

  
  ShutdownType shutdown_type_;

  
  bool no_sandbox_;

  
  bool full_memory_dump_;

  
  bool show_error_dialogs_;

  
  bool enable_dcheck_;

  
  bool silent_dump_on_dcheck_;

  
  bool disable_breakpad_;

  
  std::string js_flags_;

  
  std::string log_level_;

  DISALLOW_COPY_AND_ASSIGN(ProxyLauncher);
};

class NamedProxyLauncher : public ProxyLauncher {
 public:
  
  
  NamedProxyLauncher(const std::string& channel_id,
                     bool launch_browser, bool disconnect_on_failure);

  virtual AutomationProxy* CreateAutomationProxy(
      base::TimeDelta execution_timeout);
  virtual bool InitializeConnection(
      const LaunchState& state,
      bool wait_for_initial_loads) OVERRIDE WARN_UNUSED_RESULT;
  virtual void TerminateConnection();
  virtual std::string PrefixedChannelID() const;

 protected:
  std::string channel_id_;      
  bool launch_browser_;         
  bool disconnect_on_failure_;  

 private:
  DISALLOW_COPY_AND_ASSIGN(NamedProxyLauncher);
};

class AnonymousProxyLauncher : public ProxyLauncher {
 public:
  explicit AnonymousProxyLauncher(bool disconnect_on_failure);
  virtual AutomationProxy* CreateAutomationProxy(
      base::TimeDelta execution_timeout);
  virtual bool InitializeConnection(
      const LaunchState& state,
      bool wait_for_initial_loads) OVERRIDE WARN_UNUSED_RESULT;
  virtual void TerminateConnection();
  virtual std::string PrefixedChannelID() const;

 protected:
  std::string channel_id_;      
  bool disconnect_on_failure_;  

 private:
  DISALLOW_COPY_AND_ASSIGN(AnonymousProxyLauncher);
};

#endif  
