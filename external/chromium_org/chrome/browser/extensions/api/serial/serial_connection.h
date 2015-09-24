// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SERIAL_SERIAL_CONNECTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_SERIAL_SERIAL_CONNECTION_H_

#include <set>
#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/platform_file.h"
#include "base/time/time.h"
#include "chrome/browser/extensions/api/api_resource.h"
#include "chrome/browser/extensions/api/api_resource_manager.h"
#include "chrome/browser/extensions/api/serial/serial_io_handler.h"
#include "chrome/common/extensions/api/serial.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/file_stream.h"

using content::BrowserThread;

namespace extensions {

class SerialConnection : public ApiResource,
                         public base::SupportsWeakPtr<SerialConnection> {
 public:
  typedef base::Callback<void(bool success)> OpenCompleteCallback;

  
  
  
  typedef base::Callback<void(const std::string& data,
                              api::serial::ReceiveError error)>
      ReceiveCompleteCallback;

  
  
  
  typedef base::Callback<void(int bytes_sent,
                              api::serial::SendError error)>
      SendCompleteCallback;

  SerialConnection(const std::string& port,
                   const std::string& owner_extension_id);
  virtual ~SerialConnection();

  
  virtual bool IsPersistent() const OVERRIDE;

  void set_persistent(bool persistent) { persistent_ = persistent; }
  bool persistent() const { return persistent_; }

  void set_name(const std::string& name) { name_ = name; }
  const std::string& name() const { return name_; }

  void set_buffer_size(int buffer_size);
  int buffer_size() const { return buffer_size_; }

  void set_receive_timeout(int receive_timeout);
  int receive_timeout() const { return receive_timeout_; }

  void set_send_timeout(int send_timeout);
  int send_timeout() const { return send_timeout_; }

  void set_paused(bool paused);
  bool paused() const { return paused_; }

  
  
  
  virtual void Open(const OpenCompleteCallback& callback);

  
  
  
  virtual void Close();

  
  
  
  virtual bool Receive(const ReceiveCompleteCallback& callback);

  
  
  
  virtual bool Send(const std::string& data,
                    const SendCompleteCallback& callback);

  
  virtual bool Flush() const;

  
  
  
  virtual bool Configure(const api::serial::ConnectionOptions& options);

  
  
  
  virtual bool GetInfo(api::serial::ConnectionInfo* info) const;

  
  
  
  virtual bool GetControlSignals(api::serial::DeviceControlSignals*
                                 control_signals) const;

  
  
  
  virtual bool SetControlSignals(
      const api::serial::HostControlSignals& control_signals);

  static const BrowserThread::ID kThreadId = BrowserThread::IO;

 protected:
  
  virtual void SetIoHandlerForTest(scoped_refptr<SerialIoHandler> handler);

  
  bool PostOpen();

  
  
  bool ConfigurePort(const api::serial::ConnectionOptions& options);

  
  
  
  bool GetPortInfo(api::serial::ConnectionInfo* info) const;

  
  static std::string MaybeFixUpPortName(const std::string &port_name);

 private:
  friend class ApiResourceManager<SerialConnection>;
  static const char* service_name() {
    return "SerialConnectionManager";
  }

  
  
  
  class TimeoutTask {
   public:
    TimeoutTask(const base::Closure& closure, const base::TimeDelta& delay);
    ~TimeoutTask();

   private:
    void Run() const;

    base::WeakPtrFactory<TimeoutTask> weak_factory_;
    base::Closure closure_;
    base::TimeDelta delay_;
  };

  
  void StartOpen();

  
  void FinishOpen(base::PlatformFile file);

  
  static void DoClose(base::PlatformFile port);

  
  void OnReceiveTimeout();

  
  void OnSendTimeout();

  
  void OnAsyncReadComplete(const std::string& data,
                           api::serial::ReceiveError error);

  
  void OnAsyncWriteComplete(int bytes_sent, api::serial::SendError error);

  
  std::string port_;

  
  
  base::PlatformFile file_;

  
  
  bool persistent_;

  
  std::string name_;

  
  int buffer_size_;

  
  
  int receive_timeout_;

  
  
  int send_timeout_;

  
  
  bool paused_;

  
  OpenCompleteCallback open_complete_;

  
  ReceiveCompleteCallback receive_complete_;

  
  SendCompleteCallback send_complete_;

  
  
  scoped_ptr<TimeoutTask> receive_timeout_task_;

  
  
  scoped_ptr<TimeoutTask> send_timeout_task_;

  
  scoped_refptr<SerialIoHandler> io_handler_;
};

}  

#endif  
