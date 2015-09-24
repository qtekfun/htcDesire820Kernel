// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_TEST_SERVICE_H_
#define DBUS_TEST_SERVICE_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread.h"
#include "base/synchronization/waitable_event.h"
#include "dbus/bus.h"
#include "dbus/exported_object.h"

namespace base {
class SequencedTaskRunner;
}

namespace dbus {

class MethodCall;
class MessageWriter;
class Response;

class TestService : public base::Thread {
 public:
  
  struct Options {
    Options();
    ~Options();

    
    scoped_refptr<base::SequencedTaskRunner> dbus_task_runner;

    
    Bus::ServiceOwnershipOptions request_ownership_options;
  };

  
  static const int kNumMethodsToExport;

  explicit TestService(const Options& options);
  virtual ~TestService();

  
  
  bool StartService();

  
  
  bool WaitUntilServiceIsStarted() WARN_UNUSED_RESULT;

  
  void ShutdownAndBlock();

  
  bool HasDBusThread();

  
  void SendTestSignal(const std::string& message);

  
  
  void SendTestSignalFromRoot(const std::string& message);

  
  
  
  void RequestOwnership(base::Callback<void(bool)> callback);

  
  bool has_ownership() const { return has_ownership_; }

 private:
  
  void SendTestSignalInternal(const std::string& message);

  
  void SendTestSignalFromRootInternal(const std::string& message);

  
  void ShutdownAndBlockInternal();

  
  
  
  
  
  void OnOwnership(base::Callback<void(bool)> callback,
                   const std::string& service_name,
                   bool success);

  
  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

  
  virtual void Run(base::MessageLoop* message_loop) OVERRIDE;

  
  
  

  
  void Echo(MethodCall* method_call,
            dbus::ExportedObject::ResponseSender response_sender);

  
  
  void SlowEcho(MethodCall* method_call,
                dbus::ExportedObject::ResponseSender response_sender);

  
  
  void AsyncEcho(MethodCall* method_call,
                 dbus::ExportedObject::ResponseSender response_sender);

  
  void BrokenMethod(MethodCall* method_call,
                    dbus::ExportedObject::ResponseSender response_sender);

  
  void GetAllProperties(MethodCall* method_call,
                        dbus::ExportedObject::ResponseSender response_sender);

  
  void GetProperty(MethodCall* method_call,
                   dbus::ExportedObject::ResponseSender response_sender);

  
  void SetProperty(MethodCall* method_call,
                   dbus::ExportedObject::ResponseSender response_sender);

  
  void PerformAction(MethodCall* method_call,
                     dbus::ExportedObject::ResponseSender response_sender);

  
  void GetManagedObjects(MethodCall* method_call,
                         dbus::ExportedObject::ResponseSender response_sender);

  
  void AddPropertiesToWriter(MessageWriter* writer);

  
  void AddObject(const dbus::ObjectPath& object_path);
  void AddObjectInternal(const dbus::ObjectPath& object_path);

  
  void RemoveObject(const dbus::ObjectPath& object_path);
  void RemoveObjectInternal(const dbus::ObjectPath& object_path);

  
  void SendPropertyChangedSignal(const std::string& name);

  
  void SendPropertyChangedSignalInternal(const std::string& name);

  
  void RequestOwnershipInternal(base::Callback<void(bool)> callback);

  
  Bus::ServiceOwnershipOptions request_ownership_options_;

  scoped_refptr<base::SequencedTaskRunner> dbus_task_runner_;
  base::WaitableEvent on_all_methods_exported_;
  
  int num_exported_methods_;

  
  bool has_ownership_;

  scoped_refptr<Bus> bus_;
  ExportedObject* exported_object_;
  ExportedObject* exported_object_manager_;
};

}  

#endif  
