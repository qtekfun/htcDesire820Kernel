// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_FILE_DESCRIPTOR_H_
#define DBUS_FILE_DESCRIPTOR_H_

#include "base/basictypes.h"
#include "dbus/dbus_export.h"

namespace dbus {

class CHROME_DBUS_EXPORT FileDescriptor {
 public:
  
  
  FileDescriptor() : value_(-1), owner_(false), valid_(false) {}
  explicit FileDescriptor(int value) : value_(value), owner_(false),
      valid_(false) {}

  virtual ~FileDescriptor();

  
  int value() const;

  
  int is_valid() const { return valid_; }

  
  void PutValue(int value) {
    value_ = value;
    owner_ = true;
    valid_ = false;
  }

  
  int TakeValue();

  
  
  
  void CheckValidity();

 private:
  int value_;
  bool owner_;
  bool valid_;

  DISALLOW_COPY_AND_ASSIGN(FileDescriptor);
};

}  

#endif  
