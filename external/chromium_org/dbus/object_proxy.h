// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_OBJECT_PROXY_H_
#define DBUS_OBJECT_PROXY_H_

#include <dbus/dbus.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "base/time/time.h"
#include "dbus/dbus_export.h"
#include "dbus/object_path.h"

namespace dbus {

class Bus;
class ErrorResponse;
class MethodCall;
class Response;
class Signal;

class CHROME_DBUS_EXPORT ObjectProxy
    : public base::RefCountedThreadSafe<ObjectProxy> {
 public:
  
  
  ObjectProxy(Bus* bus,
              const std::string& service_name,
              const ObjectPath& object_path,
              int options);

  
  
  
  enum Options {
    DEFAULT_OPTIONS = 0,
    IGNORE_SERVICE_UNKNOWN_ERRORS = 1 << 0
  };

  
  
  
  
  
  enum {
    TIMEOUT_USE_DEFAULT = -1,
    TIMEOUT_INFINITE = 0x7fffffff,
  };

  
  
  typedef base::Callback<void(ErrorResponse*)> ErrorCallback;

  
  typedef base::Callback<void(Response*)> ResponseCallback;

  
  typedef base::Callback<void (Signal*)> SignalCallback;

  
  typedef base::Callback<void(
      const std::string& old_owner,
      const std::string& new_owner)> NameOwnerChangedCallback;

  
  typedef base::Callback<void(
      bool service_is_available)> WaitForServiceToBeAvailableCallback;

  
  
  
  
  
  typedef base::Callback<void (const std::string&, const std::string&, bool)>
      OnConnectedCallback;

  
  
  
  
  virtual scoped_ptr<Response> CallMethodAndBlock(MethodCall* method_call,
                                                  int timeout_ms);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void CallMethod(MethodCall* method_call,
                          int timeout_ms,
                          ResponseCallback callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void CallMethodWithErrorCallback(MethodCall* method_call,
                                           int timeout_ms,
                                           ResponseCallback callback,
                                           ErrorCallback error_callback);

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ConnectToSignal(const std::string& interface_name,
                               const std::string& signal_name,
                               SignalCallback signal_callback,
                               OnConnectedCallback on_connected_callback);

  
  
  
  virtual void SetNameOwnerChangedCallback(NameOwnerChangedCallback callback);

  
  virtual void WaitForServiceToBeAvailable(
      WaitForServiceToBeAvailableCallback callback);

  
  
  
  
  virtual void Detach();

  const ObjectPath& object_path() const { return object_path_; }

  
  
  static ResponseCallback EmptyResponseCallback();

 protected:
  
  virtual ~ObjectProxy();

 private:
  friend class base::RefCountedThreadSafe<ObjectProxy>;

  
  
  struct OnPendingCallIsCompleteData {
    OnPendingCallIsCompleteData(ObjectProxy* in_object_proxy,
                                ResponseCallback in_response_callback,
                                ErrorCallback error_callback,
                                base::TimeTicks start_time);
    ~OnPendingCallIsCompleteData();

    ObjectProxy* object_proxy;
    ResponseCallback response_callback;
    ErrorCallback error_callback;
    base::TimeTicks start_time;
  };

  
  
  void StartAsyncMethodCall(int timeout_ms,
                            DBusMessage* request_message,
                            ResponseCallback response_callback,
                            ErrorCallback error_callback,
                            base::TimeTicks start_time);

  
  void OnPendingCallIsComplete(DBusPendingCall* pending_call,
                               ResponseCallback response_callback,
                               ErrorCallback error_callback,
                               base::TimeTicks start_time);

  
  void RunResponseCallback(ResponseCallback response_callback,
                           ErrorCallback error_callback,
                           base::TimeTicks start_time,
                           DBusMessage* response_message);

  
  static void OnPendingCallIsCompleteThunk(DBusPendingCall* pending_call,
                                           void* user_data);

  
  bool ConnectToNameOwnerChangedSignal();

  
  bool ConnectToSignalInternal(const std::string& interface_name,
                               const std::string& signal_name,
                               SignalCallback signal_callback);

  
  void WaitForServiceToBeAvailableInternal();

  
  
  DBusHandlerResult HandleMessage(DBusConnection* connection,
                                  DBusMessage* raw_message);

  
  void RunMethod(base::TimeTicks start_time,
                 std::vector<SignalCallback> signal_callbacks,
                 Signal* signal);

  
  static DBusHandlerResult HandleMessageThunk(DBusConnection* connection,
                                              DBusMessage* raw_message,
                                              void* user_data);

  
  void LogMethodCallFailure(const base::StringPiece& interface_name,
                            const base::StringPiece& method_name,
                            const base::StringPiece& error_name,
                            const base::StringPiece& error_message) const;

  
  void OnCallMethodError(const std::string& interface_name,
                         const std::string& method_name,
                         ResponseCallback response_callback,
                         ErrorResponse* error_response);

  
  bool AddMatchRuleWithCallback(const std::string& match_rule,
                                const std::string& absolute_signal_name,
                                SignalCallback signal_callback);

  
  bool AddMatchRuleWithoutCallback(const std::string& match_rule,
                                   const std::string& absolute_signal_name);

  
  
  
  
  void UpdateNameOwnerAndBlock();

  
  DBusHandlerResult HandleNameOwnerChanged(scoped_ptr<dbus::Signal> signal);

  
  void RunNameOwnerChangedCallback(const std::string& old_owner,
                                   const std::string& new_owner);

  
  void RunWaitForServiceToBeAvailableCallbacks(bool service_is_available);

  scoped_refptr<Bus> bus_;
  std::string service_name_;
  ObjectPath object_path_;

  
  bool filter_added_;

  
  
  typedef std::map<std::string, std::vector<SignalCallback> > MethodTable;
  MethodTable method_table_;

  
  NameOwnerChangedCallback name_owner_changed_callback_;

  
  std::vector<WaitForServiceToBeAvailableCallback>
      wait_for_service_to_be_available_callbacks_;

  std::set<std::string> match_rules_;

  const bool ignore_service_unknown_errors_;

  
  std::string service_name_owner_;

  DISALLOW_COPY_AND_ASSIGN(ObjectProxy);
};

}  

#endif  
