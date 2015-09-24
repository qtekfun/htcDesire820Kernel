// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_USB_USB_IDS_H_
#define DEVICE_USB_USB_IDS_H_

#include <stdint.h>

#include "base/basictypes.h"

namespace device {

struct UsbProduct {
  const uint16_t id;
  const char* name;
};

struct UsbVendor {
  const uint16_t id;
  const char* name;
  const size_t product_size;
  const UsbProduct* products;
};

class UsbIds {
 public:
  
  
  static const char* GetVendorName(uint16_t vendor_id);

  
  
  
  
  static const char* GetProductName(uint16_t vendor_id, uint16_t product_id);

 private:
  UsbIds();
  ~UsbIds();

  
  
  static const UsbVendor* FindVendor(uint16_t vendor_id);

  
  
  static const size_t vendor_size_;
  static const UsbVendor vendors_[];

  DISALLOW_COPY_AND_ASSIGN(UsbIds);
};

}  

#endif  
