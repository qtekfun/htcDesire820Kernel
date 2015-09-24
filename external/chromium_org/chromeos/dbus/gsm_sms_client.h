// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_GSM_SMS_CLIENT_H_
#define CHROMEOS_DBUS_GSM_SMS_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace dbus {
class ObjectPath;
}

namespace chromeos {

class CHROMEOS_EXPORT GsmSMSClient : public DBusClient {
 public:
  typedef base::Callback<void(uint32 index, bool complete)> SmsReceivedHandler;
  typedef base::Callback<void()> DeleteCallback;
  typedef base::Callback<void(const base::DictionaryValue& sms)> GetCallback;
  typedef base::Callback<void(const base::ListValue& result)> ListCallback;

  virtual ~GsmSMSClient();

  
  
  static GsmSMSClient* Create();

  
  virtual void SetSmsReceivedHandler(const std::string& service_name,
                                     const dbus::ObjectPath& object_path,
                                     const SmsReceivedHandler& handler) = 0;

  
  virtual void ResetSmsReceivedHandler(const std::string& service_name,
                                       const dbus::ObjectPath& object_path) = 0;

  
  virtual void Delete(const std::string& service_name,
                      const dbus::ObjectPath& object_path,
                      uint32 index,
                      const DeleteCallback& callback) = 0;

  
  virtual void Get(const std::string& service_name,
                   const dbus::ObjectPath& object_path,
                   uint32 index,
                   const GetCallback& callback) = 0;

  
  virtual void List(const std::string& service_name,
                    const dbus::ObjectPath& object_path,
                    const ListCallback& callback) = 0;

  
  
  virtual void RequestUpdate(const std::string& service_name,
                             const dbus::ObjectPath& object_path) = 0;

 protected:
  friend class GsmSMSClientTest;

  
  GsmSMSClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(GsmSMSClient);
};

}  

#endif  
