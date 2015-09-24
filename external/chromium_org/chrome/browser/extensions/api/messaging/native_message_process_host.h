// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MESSAGING_NATIVE_MESSAGE_PROCESS_HOST_H_
#define CHROME_BROWSER_EXTENSIONS_API_MESSAGING_NATIVE_MESSAGE_PROCESS_HOST_H_

#include <queue>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/process/process.h"
#include "chrome/browser/extensions/api/messaging/native_process_launcher.h"
#include "content/public/browser/browser_thread.h"
#include "ui/gfx/native_widget_types.h"

namespace net {

class DrainableIOBuffer;
class FileStream;
class IOBuffer;
class IOBufferWithSize;

}  

namespace extensions {

class NativeMessageProcessHost
#if defined(OS_POSIX)
    : public base::MessageLoopForIO::Watcher
#endif  
{
 public:
  
  class Client {
   public:
    virtual ~Client() {}
    
    virtual void PostMessageFromNativeProcess(int port_id,
                                              const std::string& message) = 0;
    virtual void CloseChannel(int port_id,
                              const std::string& error_message) = 0;
  };

  virtual ~NativeMessageProcessHost();

  static scoped_ptr<NativeMessageProcessHost> Create(
      gfx::NativeView native_view,
      base::WeakPtr<Client> weak_client_ui,
      const std::string& source_extension_id,
      const std::string& native_host_name,
      int destination_port);

  
  static scoped_ptr<NativeMessageProcessHost> CreateWithLauncher(
      base::WeakPtr<Client> weak_client_ui,
      const std::string& source_extension_id,
      const std::string& native_host_name,
      int destination_port,
      scoped_ptr<NativeProcessLauncher> launcher);

  
  void Send(const std::string& json);

#if defined(OS_POSIX)
  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;
#endif  

  
  
  void ReadNowForTesting();

 private:
  NativeMessageProcessHost(base::WeakPtr<Client> weak_client_ui,
                           const std::string& source_extension_id,
                           const std::string& native_host_name,
                           int destination_port,
                           scoped_ptr<NativeProcessLauncher> launcher);

  
  void LaunchHostProcess();

  
  void OnHostProcessLaunched(NativeProcessLauncher::LaunchResult result,
                             base::ProcessHandle process_handle,
                             base::PlatformFile read_file,
                             base::PlatformFile write_file);

  
  void WaitRead();
  void DoRead();
  void OnRead(int result);
  void HandleReadResult(int result);
  void ProcessIncomingData(const char* data, int data_size);

  
  void DoWrite();
  void HandleWriteResult(int result);
  void OnWritten(int result);

  
  void Close(const std::string& error_message);

  
  
  base::WeakPtr<Client> weak_client_ui_;

  
  std::string source_extension_id_;

  
  std::string native_host_name_;

  
  
  int destination_port_;

  
  scoped_ptr<NativeProcessLauncher> launcher_;

  
  
  bool closed_;

  base::ProcessHandle process_handle_;

  
  base::PlatformFile read_file_;
  scoped_ptr<net::FileStream> read_stream_;

#if defined(OS_POSIX)
  base::MessageLoopForIO::FileDescriptorWatcher read_watcher_;
#endif  

  
  scoped_ptr<net::FileStream> write_stream_;

  
  scoped_refptr<net::IOBuffer> read_buffer_;

  
  bool read_pending_;

  
  std::string incoming_data_;

  
  std::queue<scoped_refptr<net::IOBufferWithSize> > write_queue_;

  
  scoped_refptr<net::DrainableIOBuffer> current_write_buffer_;

  
  bool write_pending_;

  DISALLOW_COPY_AND_ASSIGN(NativeMessageProcessHost);
};

}  

#endif  
