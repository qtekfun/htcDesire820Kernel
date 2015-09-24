// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NACL_HOST_NACL_BROKER_HOST_WIN_H_
#define CHROME_BROWSER_NACL_HOST_NACL_BROKER_HOST_WIN_H_
#pragma once

#include "base/basictypes.h"
#include "content/browser/browser_child_process_host.h"

class NaClBrokerHost : public BrowserChildProcessHost {
 public:
  NaClBrokerHost();
  ~NaClBrokerHost();

  
  
  bool Init();

  
  
  bool LaunchLoader(const std::wstring& loader_channel_id);

  
  void StopBroker();

 private:
  virtual bool CanShutdown() { return true; }

  
  void OnLoaderLaunched(const std::wstring& loader_channel_id,
                        base::ProcessHandle handle);

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

  bool stopping_;

  DISALLOW_COPY_AND_ASSIGN(NaClBrokerHost);
};

#endif  
