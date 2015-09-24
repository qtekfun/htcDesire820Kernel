// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_OBJECT_MANAGER_H_
#define DBUS_OBJECT_MANAGER_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "dbus/object_path.h"
#include "dbus/property.h"


namespace dbus {

const char kObjectManagerInterface[] = "org.freedesktop.DBus.ObjectManager";
const char kObjectManagerGetManagedObjects[] = "GetManagedObjects";
const char kObjectManagerInterfacesAdded[] = "InterfacesAdded";
const char kObjectManagerInterfacesRemoved[] = "InterfacesRemoved";

class Bus;
class MessageReader;
class ObjectProxy;
class Response;
class Signal;

class CHROME_DBUS_EXPORT ObjectManager
    : public base::RefCountedThreadSafe<ObjectManager> {
public:
  
  
  class Interface {
   public:
    virtual ~Interface() {}

    
    
    
    
    
    
    
    
    virtual PropertySet* CreateProperties(
        ObjectProxy *object_proxy,
        const dbus::ObjectPath& object_path,
        const std::string& interface_name) = 0;

    
    
    
    
    
    
    
    
    
    virtual void ObjectAdded(const ObjectPath& object_path,
                             const std::string& interface_name) { }

    
    
    
    
    
    
    
    
    
    virtual void ObjectRemoved(const ObjectPath& object_path,
                               const std::string& interface_name) { }
  };

  
  ObjectManager(Bus* bus,
                const std::string& service_name,
                const ObjectPath& object_path);

  
  
  
  
  void RegisterInterface(const std::string& interface_name,
                         Interface* interface);

  
  
  
  void UnregisterInterface(const std::string& interface_name);

  
  
  std::vector<ObjectPath> GetObjects();

  
  
  std::vector<ObjectPath> GetObjectsWithInterface(
      const std::string& interface_name);

  
  
  
  ObjectProxy* GetObjectProxy(const ObjectPath& object_path);

  
  
  
  
  
  PropertySet* GetProperties(const ObjectPath& object_path,
                             const std::string& interface_name);

  
  
  
  void GetManagedObjects();

 protected:
  virtual ~ObjectManager();

 private:
  friend class base::RefCountedThreadSafe<ObjectManager>;


  
  void OnGetManagedObjects(Response* response);

  
  
  void InterfacesAddedReceived(Signal* signal);
  void InterfacesAddedConnected(const std::string& interface_name,
                                const std::string& signal_name,
                                bool success);

  
  
  void InterfacesRemovedReceived(Signal* signal);
  void InterfacesRemovedConnected(const std::string& interface_name,
                                  const std::string& signal_name,
                                  bool success);

  
  
  
  
  void UpdateObject(const ObjectPath& object_path, MessageReader* reader);

  
  
  
  
  
  
  
  
  
  void AddInterface(const ObjectPath& object_path,
                    const std::string& interface_name,
                    MessageReader* reader);

  
  
  
  
  void RemoveInterface(const ObjectPath& object_path,
                       const std::string& interface_name);

  Bus* bus_;
  std::string service_name_;
  ObjectPath object_path_;
  ObjectProxy* object_proxy_;

  
  
  typedef std::map<std::string, Interface*> InterfaceMap;
  InterfaceMap interface_map_;

  
  
  
  struct Object {
    Object();
    ~Object();

    ObjectProxy* object_proxy;

    
    
    typedef std::map<const std::string, PropertySet*> PropertiesMap;
    PropertiesMap properties_map;
  };

  
  typedef std::map<const ObjectPath, Object*> ObjectMap;
  ObjectMap object_map_;

  
  
  
  
  base::WeakPtrFactory<ObjectManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ObjectManager);
};

}  

#endif  
