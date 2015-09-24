// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_OBJECT_PATH_H_
#define DBUS_OBJECT_PATH_H_

#include <iosfwd>
#include <string>

#include "dbus/dbus_export.h"

namespace dbus {

class CHROME_DBUS_EXPORT ObjectPath {
 public:
  
  
  
  
  
  
  
  ObjectPath() {}
  explicit ObjectPath(const std::string& value) : value_(value) {}

  
  const std::string& value() const { return value_; }

  
  bool IsValid() const;

  
  
  bool operator<(const ObjectPath&) const;

  
  
  bool operator==(const ObjectPath&) const;
  bool operator!=(const ObjectPath&) const;

 private:
  std::string value_;
};

CHROME_DBUS_EXPORT void PrintTo(const ObjectPath& path, std::ostream* out);

}  

#endif  
