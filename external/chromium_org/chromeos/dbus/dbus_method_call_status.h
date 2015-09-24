// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_DBUS_METHOD_CALL_STATUS_H_
#define CHROMEOS_DBUS_DBUS_METHOD_CALL_STATUS_H_

#include <string>

#include "base/callback.h"
#include "chromeos/chromeos_export.h"

namespace dbus {

class ObjectPath;

}  

namespace chromeos {

enum DBusMethodCallStatus {
  DBUS_METHOD_CALL_FAILURE,
  DBUS_METHOD_CALL_SUCCESS,
};

typedef base::Callback<void(
    DBusMethodCallStatus call_status)> VoidDBusMethodCallback;

typedef base::Callback<void(DBusMethodCallStatus call_status,
                            bool result)> BoolDBusMethodCallback;

typedef base::Callback<void(
    DBusMethodCallStatus call_status,
    const std::string& result)> StringDBusMethodCallback;

typedef base::Callback<void(
    DBusMethodCallStatus call_status,
    bool result)> BooleanDBusMethodCallback;

typedef base::Callback<void(
    DBusMethodCallStatus call_status,
    const dbus::ObjectPath& result)> ObjectPathDBusMethodCallback;

typedef base::Callback<void(const dbus::ObjectPath& result)> ObjectPathCallback;

CHROMEOS_EXPORT VoidDBusMethodCallback EmptyVoidDBusMethodCallback();

}  

#endif  
