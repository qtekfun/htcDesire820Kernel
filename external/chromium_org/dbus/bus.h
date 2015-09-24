// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_BUS_H_
#define DBUS_BUS_H_

#include <dbus/dbus.h>

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/platform_thread.h"
#include "dbus/dbus_export.h"
#include "dbus/object_path.h"

namespace base {
class SequencedTaskRunner;
class SingleThreadTaskRunner;
class TaskRunner;
}

namespace tracked_objects {
class Location;
}

namespace dbus {

class ExportedObject;
class ObjectManager;
class ObjectProxy;

class CHROME_DBUS_EXPORT Bus : public base::RefCountedThreadSafe<Bus> {
 public:
  
  
  
  
  enum BusType {
    SESSION = DBUS_BUS_SESSION,
    SYSTEM = DBUS_BUS_SYSTEM,
    CUSTOM_ADDRESS,
  };

  
  
  
  
  
  
  
  
  
  enum ConnectionType {
    PRIVATE,
    SHARED,
  };

  
  
  enum GetServiceOwnerOption {
    REPORT_ERRORS,
    SUPPRESS_ERRORS,
  };

  
  
  
  
  
  
  
  
  
  enum ServiceOwnershipOptions {
    REQUIRE_PRIMARY = (DBUS_NAME_FLAG_DO_NOT_QUEUE |
                       DBUS_NAME_FLAG_REPLACE_EXISTING),
    REQUIRE_PRIMARY_ALLOW_REPLACEMENT = (REQUIRE_PRIMARY |
                                         DBUS_NAME_FLAG_ALLOW_REPLACEMENT),
  };

  
  struct CHROME_DBUS_EXPORT Options {
    Options();
    ~Options();

    BusType bus_type;  
    ConnectionType connection_type;  
    
    
    
    
    
    
    
    scoped_refptr<base::SequencedTaskRunner> dbus_task_runner;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    std::string address;

    
    
    
    
    base::Closure disconnected_callback;
  };

  
  
  explicit Bus(const Options& options);

  
  
  
  
  typedef base::Callback<void (const std::string&, bool)> OnOwnershipCallback;

  
  
  typedef base::Callback<void (const std::string& service_owner)>
      GetServiceOwnerCallback;

  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual ObjectProxy* GetObjectProxy(const std::string& service_name,
                                      const ObjectPath& object_path);

  
  
  virtual ObjectProxy* GetObjectProxyWithOptions(
      const std::string& service_name,
      const ObjectPath& object_path,
      int options);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool RemoveObjectProxy(const std::string& service_name,
                                 const ObjectPath& object_path,
                                 const base::Closure& callback);

  
  
  virtual bool RemoveObjectProxyWithOptions(
      const std::string& service_name,
      const ObjectPath& object_path,
      int options,
      const base::Closure& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual ExportedObject* GetExportedObject(const ObjectPath& object_path);

  
  
  
  
  
  
  
  virtual void UnregisterExportedObject(const ObjectPath& object_path);


  
  
  
  
  
  
  
  
  
  
  virtual ObjectManager* GetObjectManager(const std::string& service_name,
                                          const ObjectPath& object_path);

  
  
  
  
  
  
  
  
  virtual void RemoveObjectManager(const std::string& service_name,
                                   const ObjectPath& object_path);

  
  
  
  
  virtual void GetManagedObjects();

  
  
  
  
  
  
  
  
  
  
  
  virtual void ShutdownAndBlock();

  
  
  
  
  
  
  
  virtual void ShutdownOnDBusThreadAndBlock();

  
  bool shutdown_completed() { return shutdown_completed_; }

  
  
  
  

  
  
  
  
  virtual bool Connect();

  
  
  
  
  
  virtual void ClosePrivateConnection();

  
  
  
  
  
  
  
  virtual void RequestOwnership(const std::string& service_name,
                                ServiceOwnershipOptions options,
                                OnOwnershipCallback on_ownership_callback);

  
  
  
  
  virtual bool RequestOwnershipAndBlock(const std::string& service_name,
                                        ServiceOwnershipOptions options);

  
  
  
  
  virtual bool ReleaseOwnership(const std::string& service_name);

  
  
  
  
  
  virtual bool SetUpAsyncOperations();

  
  
  
  
  virtual DBusMessage* SendWithReplyAndBlock(DBusMessage* request,
                                             int timeout_ms,
                                             DBusError* error);

  
  
  
  
  virtual void SendWithReply(DBusMessage* request,
                             DBusPendingCall** pending_call,
                             int timeout_ms);

  
  
  
  
  virtual void Send(DBusMessage* request, uint32* serial);

  
  
  
  
  
  
  
  
  
  
  
  virtual bool AddFilterFunction(DBusHandleMessageFunction filter_function,
                                 void* user_data);

  
  
  
  
  virtual bool RemoveFilterFunction(DBusHandleMessageFunction filter_function,
                                    void* user_data);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void AddMatch(const std::string& match_rule, DBusError* error);

  
  
  
  
  
  virtual bool RemoveMatch(const std::string& match_rule, DBusError* error);

  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool TryRegisterObjectPath(const ObjectPath& object_path,
                                     const DBusObjectPathVTable* vtable,
                                     void* user_data,
                                     DBusError* error);

  
  
  
  virtual void UnregisterObjectPath(const ObjectPath& object_path);

  
  virtual base::TaskRunner* GetDBusTaskRunner();

  
  virtual base::TaskRunner* GetOriginTaskRunner();

  
  virtual bool HasDBusThread();

  
  
  virtual void AssertOnOriginThread();

  
  
  
  virtual void AssertOnDBusThread();

  
  
  
  
  
  virtual std::string GetServiceOwnerAndBlock(const std::string& service_name,
                                              GetServiceOwnerOption options);

  
  
  virtual void GetServiceOwner(const std::string& service_name,
                               const GetServiceOwnerCallback& callback);

  
  
  
  
  
  
  
  
  virtual void ListenForServiceOwnerChange(
      const std::string& service_name,
      const GetServiceOwnerCallback& callback);

  
  
  
  
  
  
  virtual void UnlistenForServiceOwnerChange(
      const std::string& service_name,
      const GetServiceOwnerCallback& callback);

  
  bool is_connected() { return connection_ != NULL; }

 protected:
  
  virtual ~Bus();

 private:
  friend class base::RefCountedThreadSafe<Bus>;

  
  void RemoveObjectProxyInternal(scoped_refptr<dbus::ObjectProxy> object_proxy,
                                 const base::Closure& callback);

  
  void UnregisterExportedObjectInternal(
      scoped_refptr<dbus::ExportedObject> exported_object);

  
  void ShutdownOnDBusThreadAndBlockInternal();

  
  void RequestOwnershipInternal(const std::string& service_name,
                                ServiceOwnershipOptions options,
                                OnOwnershipCallback on_ownership_callback);

  
  void GetServiceOwnerInternal(const std::string& service_name,
                               const GetServiceOwnerCallback& callback);

  
  void ListenForServiceOwnerChangeInternal(
      const std::string& service_name,
      const GetServiceOwnerCallback& callback);

  
  void UnlistenForServiceOwnerChangeInternal(
      const std::string& service_name,
      const GetServiceOwnerCallback& callback);

  
  
  
  void ProcessAllIncomingDataIfAny();

  
  
  dbus_bool_t OnAddWatch(DBusWatch* raw_watch);

  
  void OnRemoveWatch(DBusWatch* raw_watch);

  
  void OnToggleWatch(DBusWatch* raw_watch);

  
  
  dbus_bool_t OnAddTimeout(DBusTimeout* raw_timeout);

  
  void OnRemoveTimeout(DBusTimeout* raw_timeout);

  
  void OnToggleTimeout(DBusTimeout* raw_timeout);

  
  
  void OnDispatchStatusChanged(DBusConnection* connection,
                               DBusDispatchStatus status);

  
  void OnConnectionDisconnected(DBusConnection* connection);

  
  void OnServiceOwnerChanged(DBusMessage* message);

  
  static dbus_bool_t OnAddWatchThunk(DBusWatch* raw_watch, void* data);
  static void OnRemoveWatchThunk(DBusWatch* raw_watch, void* data);
  static void OnToggleWatchThunk(DBusWatch* raw_watch, void* data);
  static dbus_bool_t OnAddTimeoutThunk(DBusTimeout* raw_timeout, void* data);
  static void OnRemoveTimeoutThunk(DBusTimeout* raw_timeout, void* data);
  static void OnToggleTimeoutThunk(DBusTimeout* raw_timeout, void* data);
  static void OnDispatchStatusChangedThunk(DBusConnection* connection,
                                           DBusDispatchStatus status,
                                           void* data);

  
  static DBusHandlerResult OnConnectionDisconnectedFilter(
      DBusConnection* connection,
      DBusMessage* message,
      void* user_data);

  
  static DBusHandlerResult OnServiceOwnerChangedFilter(
      DBusConnection* connection,
      DBusMessage* message,
      void* user_data);

  const BusType bus_type_;
  const ConnectionType connection_type_;
  scoped_refptr<base::SequencedTaskRunner> dbus_task_runner_;
  base::WaitableEvent on_shutdown_;
  DBusConnection* connection_;

  scoped_refptr<base::SingleThreadTaskRunner> origin_task_runner_;
  base::PlatformThreadId origin_thread_id_;

  std::set<std::string> owned_service_names_;
  
  
  
  
  std::map<std::string, int> match_rules_added_;
  std::set<ObjectPath> registered_object_paths_;
  std::set<std::pair<DBusHandleMessageFunction, void*> >
      filter_functions_added_;

  
  
  
  
  
  typedef std::map<std::pair<std::string, int>,
                   scoped_refptr<dbus::ObjectProxy> > ObjectProxyTable;
  ObjectProxyTable object_proxy_table_;

  
  
  
  typedef std::map<const dbus::ObjectPath,
                   scoped_refptr<dbus::ExportedObject> > ExportedObjectTable;
  ExportedObjectTable exported_object_table_;

  
  
  
  typedef std::map<std::string,
                   scoped_refptr<dbus::ObjectManager> > ObjectManagerTable;
  ObjectManagerTable object_manager_table_;

  
  
  
  
  
  
  typedef std::map<std::string, std::vector<GetServiceOwnerCallback> >
      ServiceOwnerChangedListenerMap;
  ServiceOwnerChangedListenerMap service_owner_changed_listener_map_;

  bool async_operations_set_up_;
  bool shutdown_completed_;

  
  
  int num_pending_watches_;
  int num_pending_timeouts_;

  std::string address_;
  base::Closure on_disconnected_closure_;

  DISALLOW_COPY_AND_ASSIGN(Bus);
};

}  

#endif  
