// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_HANDLER_CALLBACKS_H_
#define CHROMEOS_NETWORK_NETWORK_HANDLER_CALLBACKS_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {
namespace network_handler {

CHROMEOS_EXPORT extern const char kDBusFailedError[];
CHROMEOS_EXPORT extern const char kDBusFailedErrorMessage[];
CHROMEOS_EXPORT extern const char kErrorName[];
CHROMEOS_EXPORT extern const char kErrorDetail[];
CHROMEOS_EXPORT extern const char kDbusErrorName[];
CHROMEOS_EXPORT extern const char kDbusErrorMessage[];

typedef base::Callback<
  void(const std::string& error_name,
       scoped_ptr<base::DictionaryValue> error_data)> ErrorCallback;

typedef base::Callback<
  void(const std::string& service_path,
       const base::DictionaryValue& dictionary)> DictionaryResultCallback;

typedef base::Callback<
  void(const std::string& service_path)> StringResultCallback;

CHROMEOS_EXPORT base::DictionaryValue* CreateErrorData(
    const std::string& path,
    const std::string& error_name,
    const std::string& error_detail);

CHROMEOS_EXPORT void RunErrorCallback(const ErrorCallback& error_callback,
                                      const std::string& path,
                                      const std::string& error_name,
                                      const std::string& error_detail);

CHROMEOS_EXPORT base::DictionaryValue* CreateDBusErrorData(
    const std::string& path,
    const std::string& error_name,
    const std::string& error_detail,
    const std::string& dbus_error_name,
    const std::string& dbus_error_message);

CHROMEOS_EXPORT void ShillErrorCallbackFunction(
    const std::string& error_name,
    const std::string& path,
    const ErrorCallback& error_callback,
    const std::string& dbus_error_name,
    const std::string& dbus_error_message);

CHROMEOS_EXPORT void GetPropertiesCallback(
    const DictionaryResultCallback& callback,
    const ErrorCallback& error_callback,
    const std::string& path,
    DBusMethodCallStatus call_status,
    const base::DictionaryValue& value);

}  
}  

#endif  
