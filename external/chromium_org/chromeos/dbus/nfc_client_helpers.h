// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_NFC_CLIENT_HELPERS_H_
#define CHROMEOS_DBUS_NFC_CLIENT_HELPERS_H_

#include <map>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/values.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/nfc_property_set.h"
#include "dbus/bus.h"
#include "dbus/message.h"
#include "dbus/object_path.h"
#include "dbus/object_proxy.h"

namespace chromeos {
namespace nfc_client_helpers {

CHROMEOS_EXPORT extern const char kNoResponseError[];
CHROMEOS_EXPORT extern const char kUnknownObjectError[];

typedef base::Callback<void(const std::string& error_name,
                            const std::string& error_message)> ErrorCallback;

typedef std::vector<dbus::ObjectPath> ObjectPathVector;

CHROMEOS_EXPORT void OnSuccess(const base::Closure& callback,
                               dbus::Response* response);

CHROMEOS_EXPORT void OnError(const ErrorCallback& error_callback,
                             dbus::ErrorResponse* response);

CHROMEOS_EXPORT void AppendValueDataAsVariant(dbus::MessageWriter* writer,
                                              const base::Value& value);

class CHROMEOS_EXPORT DBusObjectMap {
 public:
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    
    virtual NfcPropertySet* CreateProperties(
        dbus::ObjectProxy* object_proxy) = 0;

    
    
    virtual void ObjectAdded(const dbus::ObjectPath& object_path) {}

    
    
    
    
    
    virtual void ObjectRemoved(const dbus::ObjectPath& object_path) {}
  };

  
  
  
  
  DBusObjectMap(const std::string& service_name,
                Delegate* delegate,
                dbus::Bus* bus);

  
  
  virtual ~DBusObjectMap();

  
  
  dbus::ObjectProxy* GetObjectProxy(const dbus::ObjectPath& object_path);

  
  
  
  NfcPropertySet* GetObjectProperties(const dbus::ObjectPath& object_path);

  
  
  
  void UpdateObjects(const ObjectPathVector& object_paths);

  
  
  
  bool AddObject(const dbus::ObjectPath& object_path);

  
  
  
  void RemoveObject(const dbus::ObjectPath& object_path);

  
  
  void RefreshProperties(const dbus::ObjectPath& object_path);

  
  
  void RefreshAllProperties();

  
  
  ObjectPathVector GetObjectPaths();

 private:
  typedef std::pair<dbus::ObjectProxy*, NfcPropertySet*> ObjectPropertyPair;
  typedef std::map<dbus::ObjectPath, ObjectPropertyPair> ObjectMap;

  
  
  
  ObjectPropertyPair GetObjectPropertyPair(const dbus::ObjectPath& object_path);

  
  
  
  
  
  
  
  void CleanUpObjectPropertyPair(const ObjectPropertyPair& pair);

  dbus::Bus* bus_;
  ObjectMap object_map_;
  std::string service_name_;
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(DBusObjectMap);
};

class ObjectProxyTree {
 public:
  ObjectProxyTree();

  
  
  
  virtual ~ObjectProxyTree();

  
  
  
  
  
  bool CreateObjectMap(const dbus::ObjectPath& object_path,
                       const std::string& service_name,
                       DBusObjectMap::Delegate* delegate,
                       dbus::Bus* bus);

  
  void RemoveObjectMap(const dbus::ObjectPath& object_path);

  
  
  DBusObjectMap* GetObjectMap(const dbus::ObjectPath& object_path);

  
  
  dbus::ObjectProxy* FindObjectProxy(
      const dbus::ObjectPath& object_proxy_path);

  
  
  NfcPropertySet* FindObjectProperties(
      const dbus::ObjectPath& object_proxy_path);

 private:
  typedef std::map<dbus::ObjectPath, DBusObjectMap*> PathsToObjectMapsType;
  PathsToObjectMapsType paths_to_object_maps_;

  DISALLOW_COPY_AND_ASSIGN(ObjectProxyTree);
};

}  
}  

#endif  
