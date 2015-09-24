// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_USB_USB_DEVICE_H_
#define CHROME_BROWSER_USB_USB_DEVICE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/usb/usb_interface.h"

struct libusb_device;
class UsbDeviceHandle;
class UsbContext;

typedef libusb_device* PlatformUsbDevice;

class UsbDevice : public base::RefCountedThreadSafe<UsbDevice> {
 public:
  
  PlatformUsbDevice platform_device() const { return platform_device_; }
  uint16 vendor_id() const { return vendor_id_; }
  uint16 product_id() const { return product_id_; }
  uint32 unique_id() const { return unique_id_; }

#if defined(OS_CHROMEOS)
  
  
  
  
  virtual void RequestUsbAcess(
      int interface_id, const base::Callback<void(bool success)>& callback);
#endif  

  
  
  virtual scoped_refptr<UsbDeviceHandle> Open();

  
  
  
  
  virtual bool Close(scoped_refptr<UsbDeviceHandle> handle);

  
  
  
  virtual scoped_refptr<UsbConfigDescriptor> ListInterfaces();

 protected:
  friend class UsbService;
  friend class base::RefCountedThreadSafe<UsbDevice>;

  
  UsbDevice(scoped_refptr<UsbContext> context,
            PlatformUsbDevice platform_device,
            uint16 vendor_id,
            uint16 product_id,
            uint32 unique_id);

  
  UsbDevice();
  virtual ~UsbDevice();

  
  virtual void OnDisconnect();

 private:
  PlatformUsbDevice platform_device_;
  uint16 vendor_id_;
  uint16 product_id_;
  uint32 unique_id_;

  
  scoped_refptr<UsbContext> context_;

  
  typedef std::vector<scoped_refptr<UsbDeviceHandle> > HandlesVector;
  HandlesVector handles_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(UsbDevice);
};

#endif  
