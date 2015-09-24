// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_EXPORTED_OBJECT_H_
#define DBUS_EXPORTED_OBJECT_H_

#include <dbus/dbus.h>

#include <map>
#include <string>
#include <utility>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/platform_thread.h"
#include "base/time/time.h"
#include "dbus/dbus_export.h"
#include "dbus/object_path.h"

namespace dbus {

class Bus;
class MethodCall;
class Response;
class Signal;

class CHROME_DBUS_EXPORT ExportedObject
    : public base::RefCountedThreadSafe<ExportedObject> {
 public:
  
  
  ExportedObject(Bus* bus, const ObjectPath& object_path);

  
  
  
  typedef base::Callback<void (scoped_ptr<Response> response)> ResponseSender;

  
  
  
  
  
  typedef base::Callback<void (MethodCall* method_call, ResponseSender sender)>
      MethodCallCallback;

  
  
  typedef base::Callback<void (const std::string& interface_name,
                               const std::string& method_name,
                               bool success)>
      OnExportedCallback;

  
  
  
  
  
  
  
  
  
  virtual bool ExportMethodAndBlock(const std::string& interface_name,
                                    const std::string& method_name,
                                    MethodCallCallback method_call_callback);

  
  
  
  
  
  
  
  virtual void ExportMethod(const std::string& interface_name,
                            const std::string& method_name,
                            MethodCallCallback method_call_callback,
                            OnExportedCallback on_exported_callback);

  
  
  virtual void SendSignal(Signal* signal);

  
  
  
  
  virtual void Unregister();

 protected:
  
  virtual ~ExportedObject();

 private:
  friend class base::RefCountedThreadSafe<ExportedObject>;

  
  void ExportMethodInternal(const std::string& interface_name,
                            const std::string& method_name,
                            MethodCallCallback method_call_callback,
                            OnExportedCallback exported_callback);

  
  void OnExported(OnExportedCallback on_exported_callback,
                  const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

  
  void SendSignalInternal(base::TimeTicks start_time,
                          DBusMessage* signal_message);

  
  
  
  
  bool Register();

  
  
  DBusHandlerResult HandleMessage(DBusConnection* connection,
                                  DBusMessage* raw_message);

  
  void RunMethod(MethodCallCallback method_call_callback,
                 scoped_ptr<MethodCall> method_call,
                 base::TimeTicks start_time);

  
  
  
  void SendResponse(base::TimeTicks start_time,
                    scoped_ptr<MethodCall> method_call,
                    scoped_ptr<Response> response);

  
  
  void OnMethodCompleted(scoped_ptr<MethodCall> method_call,
                         scoped_ptr<Response> response,
                         base::TimeTicks start_time);

  
  void OnUnregistered(DBusConnection* connection);

  
  static DBusHandlerResult HandleMessageThunk(DBusConnection* connection,
                                              DBusMessage* raw_message,
                                              void* user_data);

  
  static void OnUnregisteredThunk(DBusConnection* connection,
                                  void* user_data);

  scoped_refptr<Bus> bus_;
  ObjectPath object_path_;
  bool object_is_registered_;

  
  
  typedef std::map<std::string, MethodCallCallback> MethodTable;
  MethodTable method_table_;
};

}  

#endif  
