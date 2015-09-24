// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_INTROSPECTABLE_CLIENT_H_
#define CHROMEOS_DBUS_INTROSPECTABLE_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "dbus/object_path.h"

namespace chromeos {

class CHROMEOS_EXPORT IntrospectableClient : public DBusClient {
 public:
  virtual ~IntrospectableClient();

  
  
  
  
  
  
  typedef base::Callback<void(const std::string&, const dbus::ObjectPath&,
                              const std::string&, bool)> IntrospectCallback;

  
  
  
  virtual void Introspect(const std::string& service_name,
                          const dbus::ObjectPath& object_path,
                          const IntrospectCallback& callback) = 0;

  
  
  
  static std::vector<std::string> GetInterfacesFromIntrospectResult(
      const std::string& xml_data);

  
  static IntrospectableClient* Create();

 protected:
  IntrospectableClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(IntrospectableClient);
};

}  

#endif  
