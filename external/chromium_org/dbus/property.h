// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_PROPERTY_H_
#define DBUS_PROPERTY_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "dbus/dbus_export.h"
#include "dbus/message.h"
#include "dbus/object_proxy.h"


namespace dbus {

const char kPropertiesInterface[] = "org.freedesktop.DBus.Properties";
const char kPropertiesGetAll[] = "GetAll";
const char kPropertiesGet[] = "Get";
const char kPropertiesSet[] = "Set";
const char kPropertiesChanged[] = "PropertiesChanged";

class PropertySet;

class PropertyBase {
 public:
  PropertyBase() : property_set_(NULL) {}

  
  
  
  
  
  
  void Init(PropertySet* property_set, const std::string& name);

  
  
  
  
  
  
  
  
  
  
  const std::string& name() const { return name_; }

  
  
  
  
  virtual bool PopValueFromReader(MessageReader*) = 0;

  
  
  
  virtual void AppendSetValueToWriter(MessageWriter* writer) = 0;

  
  
  
  virtual void ReplaceValueWithSetValue() = 0;

 protected:
  
  PropertySet* property_set() { return property_set_; }

 private:
  
  
  PropertySet* property_set_;

  
  std::string name_;

  DISALLOW_COPY_AND_ASSIGN(PropertyBase);
};

class CHROME_DBUS_EXPORT PropertySet {
 public:
  
  
  
  typedef base::Callback<void(const std::string& name)> PropertyChangedCallback;

  
  
  
  
  
  
  PropertySet(ObjectProxy* object_proxy, const std::string& interface,
              const PropertyChangedCallback& property_changed_callback);

  
  
  virtual ~PropertySet();

  
  
  
  
  void RegisterProperty(const std::string& name, PropertyBase* property);

  
  
  
  virtual void ConnectSignals();

  
  
  
  virtual void ChangedReceived(Signal*);
  virtual void ChangedConnected(const std::string& interface_name,
                                const std::string& signal_name,
                                bool success);

  
  
  
  typedef base::Callback<void(bool success)> GetCallback;

  
  
  
  
  
  virtual void Get(PropertyBase* property, GetCallback callback);
  virtual void OnGet(PropertyBase* property, GetCallback callback,
                     Response* response);

  
  
  
  
  virtual void GetAll();
  virtual void OnGetAll(Response* response);

  
  
  typedef base::Callback<void(bool success)> SetCallback;

  
  
  
  
  
  virtual void Set(PropertyBase* property, SetCallback callback);
  virtual void OnSet(PropertyBase* property, SetCallback callback,
                     Response* response);

  
  
  
  bool UpdatePropertiesFromReader(MessageReader* reader);

  
  
  
  bool UpdatePropertyFromReader(MessageReader* reader);

  
  
  
  void NotifyPropertyChanged(const std::string& name);

  
  
  
  ObjectProxy* object_proxy() { return object_proxy_; }

  
  const std::string& interface() const { return interface_; }

 protected:
  
  
  
  base::WeakPtr<PropertySet> GetWeakPtr() {
    return weak_ptr_factory_.GetWeakPtr();
  }

 private:
  
  
  ObjectProxy* object_proxy_;

  
  
  
  std::string interface_;

  
  PropertyChangedCallback property_changed_callback_;

  
  
  
  
  typedef std::map<const std::string, PropertyBase*> PropertiesMap;
  PropertiesMap properties_map_;

  
  
  base::WeakPtrFactory<PropertySet> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PropertySet);
};

template <class T>
class CHROME_DBUS_EXPORT Property : public PropertyBase {
 public:
  Property() {}

  
  const T& value() const { return value_; }

  
  
  
  virtual void Get(dbus::PropertySet::GetCallback callback) {
    property_set()->Get(this, callback);
  }

  
  
  
  
  virtual void Set(const T& value, dbus::PropertySet::SetCallback callback) {
    set_value_ = value;
    property_set()->Set(this, callback);
  }

  
  
  
  virtual bool PopValueFromReader(MessageReader*);

  
  
  
  virtual void AppendSetValueToWriter(MessageWriter* writer);

  
  
  
  virtual void ReplaceValueWithSetValue() {
    value_ = set_value_;
    property_set()->NotifyPropertyChanged(name());
  }

  
  
  void ReplaceValue(const T& value) {
    value_ = value;
    property_set()->NotifyPropertyChanged(name());
  }

 private:
  
  T value_;

  
  T set_value_;
};

template <> Property<uint8>::Property();
template <> bool Property<uint8>::PopValueFromReader(MessageReader* reader);
template <> void Property<uint8>::AppendSetValueToWriter(MessageWriter* writer);

template <> Property<bool>::Property();
template <> bool Property<bool>::PopValueFromReader(MessageReader* reader);
template <> void Property<bool>::AppendSetValueToWriter(MessageWriter* writer);

template <> Property<int16>::Property();
template <> bool Property<int16>::PopValueFromReader(MessageReader* reader);
template <> void Property<int16>::AppendSetValueToWriter(MessageWriter* writer);

template <> Property<uint16>::Property();
template <> bool Property<uint16>::PopValueFromReader(MessageReader* reader);
template <> void Property<uint16>::AppendSetValueToWriter(
  MessageWriter* writer);

template <> Property<int32>::Property();
template <> bool Property<int32>::PopValueFromReader(MessageReader* reader);
template <> void Property<int32>::AppendSetValueToWriter(MessageWriter* writer);

template <> Property<uint32>::Property();
template <> bool Property<uint32>::PopValueFromReader(MessageReader* reader);
template <> void Property<uint32>::AppendSetValueToWriter(
  MessageWriter* writer);

template <> Property<int64>::Property();
template <> bool Property<int64>::PopValueFromReader(MessageReader* reader);
template <> void Property<int64>::AppendSetValueToWriter(MessageWriter* writer);

template <> Property<uint64>::Property();
template <> bool Property<uint64>::PopValueFromReader(MessageReader* reader);
template <> void Property<uint64>::AppendSetValueToWriter(
  MessageWriter* writer);

template <> Property<double>::Property();
template <> bool Property<double>::PopValueFromReader(MessageReader* reader);
template <> void Property<double>::AppendSetValueToWriter(
  MessageWriter* writer);

template <> bool Property<std::string>::PopValueFromReader(
  MessageReader* reader);
template <> void Property<std::string>::AppendSetValueToWriter(
  MessageWriter* writer);

template <> bool Property<ObjectPath>::PopValueFromReader(
  MessageReader* reader);
template <> void Property<ObjectPath>::AppendSetValueToWriter(
  MessageWriter* writer);

template <> bool Property<std::vector<std::string> >::PopValueFromReader(
  MessageReader* reader);
template <> void Property<std::vector<std::string> >::AppendSetValueToWriter(
  MessageWriter* writer);

template <> bool Property<std::vector<ObjectPath> >::PopValueFromReader(
  MessageReader* reader);
template <> void Property<std::vector<ObjectPath> >::AppendSetValueToWriter(
  MessageWriter* writer);

}  

#endif  
