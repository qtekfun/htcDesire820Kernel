// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SHILL_CLIENT_HELPER_H_
#define CHROMEOS_DBUS_SHILL_CLIENT_HELPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/values.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/dbus/shill_property_changed_observer.h"

namespace base {

class Value;
class DictionaryValue;

}  

namespace dbus {

class Bus;
class ErrorResponse;
class MessageWriter;
class MethodCall;
class ObjectPath;
class ObjectProxy;
class Response;
class Signal;

}  

namespace chromeos {

class ShillClientHelper {
 public:
  class RefHolder;

  
  typedef base::Callback<void(const std::string& name,
                              const base::Value& value)> PropertyChangedHandler;

  
  typedef base::Callback<void(
      DBusMethodCallStatus call_status,
      const base::DictionaryValue& result)> DictionaryValueCallback;

  
  
  typedef base::Callback<void(const base::DictionaryValue& result)>
      DictionaryValueCallbackWithoutStatus;

  
  typedef base::Callback<void(const base::ListValue& result)> ListValueCallback;

  
  typedef base::Callback<void(const std::string& error_name,
                              const std::string& error_message)> ErrorCallback;

  
  typedef base::Callback<void(const std::string& result)> StringCallback;

  
  typedef base::Callback<void(bool result)> BooleanCallback;

  
  typedef base::Callback<void(ShillClientHelper* helper)> ReleasedCallback;

  explicit ShillClientHelper(dbus::ObjectProxy* proxy);

  virtual ~ShillClientHelper();

  
  
  void SetReleasedCallback(ReleasedCallback callback);

  
  void AddPropertyChangedObserver(ShillPropertyChangedObserver* observer);

  
  void RemovePropertyChangedObserver(ShillPropertyChangedObserver* observer);

  
  
  
  void MonitorPropertyChanged(const std::string& interface_name);

  
  void CallVoidMethod(dbus::MethodCall* method_call,
                      const VoidDBusMethodCallback& callback);

  
  void CallObjectPathMethod(dbus::MethodCall* method_call,
                            const ObjectPathDBusMethodCallback& callback);

  
  void CallObjectPathMethodWithErrorCallback(
      dbus::MethodCall* method_call,
      const ObjectPathCallback& callback,
      const ErrorCallback& error_callback);

  
  void CallDictionaryValueMethod(dbus::MethodCall* method_call,
                                 const DictionaryValueCallback& callback);

  
  void CallVoidMethodWithErrorCallback(dbus::MethodCall* method_call,
                                       const base::Closure& callback,
                                       const ErrorCallback& error_callback);

  
  void CallBooleanMethodWithErrorCallback(
      dbus::MethodCall* method_call,
      const BooleanCallback& callback,
      const ErrorCallback& error_callback);

  
  void CallStringMethodWithErrorCallback(dbus::MethodCall* method_call,
                                         const StringCallback& callback,
                                         const ErrorCallback& error_callback);


  
  void CallDictionaryValueMethodWithErrorCallback(
      dbus::MethodCall* method_call,
      const DictionaryValueCallbackWithoutStatus& callback,
      const ErrorCallback& error_callback);

  
  void CallListValueMethodWithErrorCallback(
      dbus::MethodCall* method_call,
      const ListValueCallback& callback,
      const ErrorCallback& error_callback);

  const dbus::ObjectProxy* object_proxy() const { return proxy_; }

  
  
  static void AppendValueDataAsVariant(dbus::MessageWriter* writer,
                                       const base::Value& value);

  
  static void AppendServicePropertiesDictionary(
      dbus::MessageWriter* writer,
      const base::DictionaryValue& dictionary);

 protected:
  
  
  
  void AddRef();
  void Release();

 private:
  
  void MonitorPropertyChangedInternal(const std::string& interface_name);

  
  void OnSignalConnected(const std::string& interface,
                         const std::string& signal,
                         bool success);

  
  void OnPropertyChanged(dbus::Signal* signal);

  dbus::ObjectProxy* proxy_;
  ReleasedCallback released_callback_;
  int active_refs_;
  PropertyChangedHandler property_changed_handler_;
  ObserverList<ShillPropertyChangedObserver, true >
      observer_list_;
  std::vector<std::string> interfaces_to_be_monitored_;

  
  
  base::WeakPtrFactory<ShillClientHelper> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ShillClientHelper);
};

}  

#endif  
