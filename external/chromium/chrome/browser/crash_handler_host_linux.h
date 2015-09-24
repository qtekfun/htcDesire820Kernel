// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CRASH_HANDLER_HOST_LINUX_H_
#define CHROME_BROWSER_CRASH_HANDLER_HOST_LINUX_H_
#pragma once

#include "base/message_loop.h"

#if defined(USE_LINUX_BREAKPAD)
#include <sys/types.h>

#include <string>

#include "base/memory/scoped_ptr.h"

class BreakpadInfo;

namespace base {
class Thread;
}
#endif  

template <typename T> struct DefaultSingletonTraits;

class CrashHandlerHostLinux : public MessageLoopForIO::Watcher,
                              public MessageLoop::DestructionObserver {
 public:
  
  
  int GetDeathSignalSocket() const {
    return process_socket_;
  }

  
  virtual void OnFileCanWriteWithoutBlocking(int fd);
  virtual void OnFileCanReadWithoutBlocking(int fd);

  
  virtual void WillDestroyCurrentMessageLoop();

#if defined(USE_LINUX_BREAKPAD)
  
  bool IsShuttingDown() const;
#endif

 protected:
  CrashHandlerHostLinux();
  virtual ~CrashHandlerHostLinux();

#if defined(USE_LINUX_BREAKPAD)
  
  void InitCrashUploaderThread();

  std::string process_type_;
#endif

 private:
  void Init();

#if defined(USE_LINUX_BREAKPAD)
  
  virtual void SetProcessType() = 0;

  
  void WriteDumpFile(BreakpadInfo* info,
                     pid_t crashing_pid,
                     char* crash_context,
                     int signal_fd);

  
  void QueueCrashDumpTask(BreakpadInfo* info, int signal_fd);
#endif

  int process_socket_;
  int browser_socket_;

#if defined(USE_LINUX_BREAKPAD)
  MessageLoopForIO::FileDescriptorWatcher file_descriptor_watcher_;
  scoped_ptr<base::Thread> uploader_thread_;
  bool shutting_down_;
#endif

  DISALLOW_COPY_AND_ASSIGN(CrashHandlerHostLinux);
};

class GpuCrashHandlerHostLinux : public CrashHandlerHostLinux {
 public:
  
  static GpuCrashHandlerHostLinux* GetInstance();

 private:
  friend struct DefaultSingletonTraits<GpuCrashHandlerHostLinux>;
  GpuCrashHandlerHostLinux();
  virtual ~GpuCrashHandlerHostLinux();

#if defined(USE_LINUX_BREAKPAD)
  virtual void SetProcessType();
#endif

  DISALLOW_COPY_AND_ASSIGN(GpuCrashHandlerHostLinux);
};

class PluginCrashHandlerHostLinux : public CrashHandlerHostLinux {
 public:
  
  static PluginCrashHandlerHostLinux* GetInstance();

 private:
  friend struct DefaultSingletonTraits<PluginCrashHandlerHostLinux>;
  PluginCrashHandlerHostLinux();
  virtual ~PluginCrashHandlerHostLinux();

#if defined(USE_LINUX_BREAKPAD)
  virtual void SetProcessType();
#endif

  DISALLOW_COPY_AND_ASSIGN(PluginCrashHandlerHostLinux);
};

class RendererCrashHandlerHostLinux : public CrashHandlerHostLinux {
 public:
  
  static RendererCrashHandlerHostLinux* GetInstance();

 private:
  friend struct DefaultSingletonTraits<RendererCrashHandlerHostLinux>;
  RendererCrashHandlerHostLinux();
  virtual ~RendererCrashHandlerHostLinux();

#if defined(USE_LINUX_BREAKPAD)
  virtual void SetProcessType();
#endif

  DISALLOW_COPY_AND_ASSIGN(RendererCrashHandlerHostLinux);
};

class PpapiCrashHandlerHostLinux : public CrashHandlerHostLinux {
 public:
  
  static PpapiCrashHandlerHostLinux* GetInstance();

 private:
  friend struct DefaultSingletonTraits<PpapiCrashHandlerHostLinux>;
  PpapiCrashHandlerHostLinux();
  virtual ~PpapiCrashHandlerHostLinux();

#if defined(USE_LINUX_BREAKPAD)
  virtual void SetProcessType();
#endif

  DISALLOW_COPY_AND_ASSIGN(PpapiCrashHandlerHostLinux);
};

#endif  
