// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SMS_CLIENT_H_
#define CHROMEOS_DBUS_SMS_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

namespace base {
class DictionaryValue;
}

namespace dbus {
class ObjectPath;
}

namespace chromeos {

class CHROMEOS_EXPORT SMSClient : public DBusClient {
 public:
  typedef base::Callback<void(const base::DictionaryValue& sms)> GetAllCallback;

  virtual ~SMSClient();

  
  
  static SMSClient* Create();

  
  virtual void GetAll(const std::string& service_name,
                      const dbus::ObjectPath& object_path,
                      const GetAllCallback& callback) = 0;

 protected:
  
  SMSClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(SMSClient);
};

}  

#endif  
