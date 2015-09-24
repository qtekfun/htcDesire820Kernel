// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PLUGIN_PLUGIN_THREAD_H_
#define CONTENT_PLUGIN_PLUGIN_THREAD_H_

#include "base/files/file_path.h"
#include "base/native_library.h"
#include "build/build_config.h"
#include "content/child/child_thread.h"
#include "content/child/npapi/plugin_lib.h"
#include "content/plugin/plugin_channel.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#endif

namespace content {

class PluginThread : public ChildThread {
 public:
  PluginThread();
  virtual ~PluginThread();
  virtual void Shutdown() OVERRIDE;

  
  static PluginThread* current();

  
  
  void SetForcefullyTerminatePluginProcess();

 private:
  virtual bool OnControlMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  void OnCreateChannel(int renderer_id, bool incognito);
  void OnPluginMessage(const std::vector<uint8> &data);
  void OnNotifyRenderersOfPendingShutdown();
#if defined(OS_MACOSX)
  void OnAppActivated();
  void OnPluginFocusNotify(uint32 instance_id);
#endif

  
  base::NativeLibrary preloaded_plugin_module_;

  bool forcefully_terminate_plugin_process_;

  DISALLOW_COPY_AND_ASSIGN(PluginThread);
};

}  

#endif  
