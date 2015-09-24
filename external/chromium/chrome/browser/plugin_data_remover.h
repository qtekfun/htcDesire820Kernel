// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGIN_DATA_REMOVER_H_
#define CHROME_BROWSER_PLUGIN_DATA_REMOVER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "content/browser/plugin_process_host.h"

class Task;

namespace base {
class MessageLoopProxy;
class WaitableEvent;
}

class PluginDataRemover : public base::RefCountedThreadSafe<PluginDataRemover>,
                          public PluginProcessHost::Client,
                          public IPC::Channel::Listener {
 public:
  PluginDataRemover();

  
  
  
  void set_mime_type(const std::string& mime_type) { mime_type_ = mime_type; }

  
  base::WaitableEvent* StartRemoving(base::Time begin_time);

  
  
  
  static bool IsSupported();

  
  bool is_removing() const { return is_removing_; }

  
  
  
  void Wait();

  
  virtual int ID();
  virtual bool OffTheRecord();
  virtual void SetPluginInfo(const webkit::npapi::WebPluginInfo& info);
  virtual void OnChannelOpened(const IPC::ChannelHandle& handle);
  virtual void OnError();

  
  virtual bool OnMessageReceived(const IPC::Message& message);
  virtual void OnChannelError();

 private:
  friend class base::RefCountedThreadSafe<PluginDataRemover>;
  friend class PluginDataRemoverTest;
  ~PluginDataRemover();

  
  
  void SignalDone();
  
  void ConnectToChannel(const IPC::ChannelHandle& handle);
  
  void OnClearSiteDataResult(bool success);
  
  
  void OnTimeout();

  std::string mime_type_;
  bool is_removing_;
  
  base::Time remove_start_time_;
  
  base::Time begin_time_;
  scoped_ptr<base::WaitableEvent> event_;
  
  
  
  IPC::Channel* channel_;

  DISALLOW_COPY_AND_ASSIGN(PluginDataRemover);
};

#endif  
