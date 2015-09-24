// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NACL_HOST_NACL_PROCESS_HOST_H_
#define CHROME_BROWSER_NACL_HOST_NACL_PROCESS_HOST_H_
#pragma once

#include "build/build_config.h"

#include "base/memory/ref_counted.h"
#include "chrome/common/nacl_types.h"
#include "content/browser/browser_child_process_host.h"

class ChromeRenderMessageFilter;

class NaClProcessHost : public BrowserChildProcessHost {
 public:
  explicit NaClProcessHost(const std::wstring& url);
  ~NaClProcessHost();

  
  bool Launch(ChromeRenderMessageFilter* chrome_render_message_filter,
              int socket_count,
              IPC::Message* reply_msg);

  virtual bool OnMessageReceived(const IPC::Message& msg);

  void OnProcessLaunchedByBroker(base::ProcessHandle handle);

 protected:
  virtual base::TerminationStatus GetChildTerminationStatus(int* exit_code);
  virtual void OnChildDied();

 private:
  
  
  
  
  struct NaClInternal;

  bool LaunchSelLdr();

  void SendStartMessage();

  virtual void OnProcessLaunched();

  virtual bool CanShutdown();

 private:
  
  
  scoped_refptr<ChromeRenderMessageFilter> chrome_render_message_filter_;

  
  IPC::Message* reply_msg_;

  
  scoped_ptr<NaClInternal> internal_;

  
  bool running_on_wow64_;

  DISALLOW_COPY_AND_ASSIGN(NaClProcessHost);
};

#endif  
