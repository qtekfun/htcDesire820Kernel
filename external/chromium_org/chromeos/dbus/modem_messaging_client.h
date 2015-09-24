// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_MODEM_MESSAGING_CLIENT_H_
#define CHROMEOS_DBUS_MODEM_MESSAGING_CLIENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

namespace dbus {
class ObjectPath;
}

namespace chromeos {

class CHROMEOS_EXPORT ModemMessagingClient : public DBusClient {
 public:
  typedef base::Callback<void()> DeleteCallback;
  typedef base::Callback<void(const dbus::ObjectPath& message_path,
                              bool complete)> SmsReceivedHandler;
  typedef base::Callback<void(const std::vector<dbus::ObjectPath>& paths)>
      ListCallback;

  virtual ~ModemMessagingClient();

  
  
  static ModemMessagingClient* Create();

  
  virtual void SetSmsReceivedHandler(const std::string& service_name,
                                     const dbus::ObjectPath& object_path,
                                     const SmsReceivedHandler& handler) = 0;

  
  virtual void ResetSmsReceivedHandler(const std::string& service_name,
                                       const dbus::ObjectPath& object_path) = 0;

  
  virtual void Delete(const std::string& service_name,
                      const dbus::ObjectPath& object_path,
                      const dbus::ObjectPath& sms_path,
                      const DeleteCallback& callback) = 0;

  
  virtual void List(const std::string& service_name,
                    const dbus::ObjectPath& object_path,
                    const ListCallback& callback) = 0;

 protected:
  friend class ModemMessagingClientTest;

  
  ModemMessagingClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(ModemMessagingClient);
};

}  

#endif  
