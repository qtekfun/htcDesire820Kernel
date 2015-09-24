// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_VALUES_UTIL_H_
#define DBUS_VALUES_UTIL_H_

#include "dbus/dbus_export.h"

namespace base {
class Value;
}

namespace dbus {

class MessageReader;
class MessageWriter;

CHROME_DBUS_EXPORT base::Value* PopDataAsValue(MessageReader* reader);

CHROME_DBUS_EXPORT void AppendBasicTypeValueData(MessageWriter* writer,
                                                 const base::Value& value);

CHROME_DBUS_EXPORT void AppendBasicTypeValueDataAsVariant(
    MessageWriter* writer,
    const base::Value& value);

}  

#endif  
