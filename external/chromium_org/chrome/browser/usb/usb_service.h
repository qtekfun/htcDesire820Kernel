// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_USB_USB_SERVICE_H_
#define CHROME_BROWSER_USB_USB_SERVICE_H_

#include <map>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"

namespace base {

template <class T> class DeleteHelper;

}  

struct InitUsbContextTraits;
template <typename T> struct DefaultSingletonTraits;

typedef struct libusb_device* PlatformUsbDevice;
typedef struct libusb_context* PlatformUsbContext;

class UsbContext;
class UsbDevice;

class UsbService {
 public:
  typedef scoped_ptr<std::vector<scoped_refptr<UsbDevice> > >
      ScopedDeviceVector;

  
  
  static UsbService* GetInstance();

  scoped_refptr<UsbDevice> GetDeviceById(uint32 unique_id);

  
  
  
  void GetDevices(std::vector<scoped_refptr<UsbDevice> >* devices);

 private:
  friend struct InitUsbContextTraits;
  friend struct DefaultSingletonTraits<UsbService>;
  friend class base::DeleteHelper<UsbService>;

  explicit UsbService(PlatformUsbContext context);
  virtual ~UsbService();

  
  
  static bool DeviceMatches(scoped_refptr<UsbDevice> device,
                            const uint16 vendor_id,
                            const uint16 product_id);

  
  void RefreshDevices();

  scoped_refptr<UsbContext> context_;

  
  uint32 next_unique_id_;

  
  typedef std::map<PlatformUsbDevice, scoped_refptr<UsbDevice> > DeviceMap;
  DeviceMap devices_;

  DISALLOW_COPY_AND_ASSIGN(UsbService);
};

#endif  
