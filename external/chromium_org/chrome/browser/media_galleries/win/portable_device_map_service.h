// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_WIN_PORTABLE_DEVICE_MAP_SERVICE_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_WIN_PORTABLE_DEVICE_MAP_SERVICE_H_

#include <portabledeviceapi.h>
#include <map>

#include "base/lazy_instance.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/win/scoped_comptr.h"

class PortableDeviceMapService {
 public:
  static PortableDeviceMapService* GetInstance();

  
  
  void AddPortableDevice(const base::string16& device_location,
                         IPortableDevice* device);

  
  
  
  
  
  
  
  
  void MarkPortableDeviceForDeletion(const base::string16& device_location);

  
  
  
  
  void RemovePortableDevice(const base::string16& device_location);

  
  
  
  
  IPortableDevice* GetPortableDevice(const base::string16& device_location);

 private:
  friend struct base::DefaultLazyInstanceTraits<PortableDeviceMapService>;

  struct PortableDeviceInfo {
    PortableDeviceInfo();  
    explicit PortableDeviceInfo(IPortableDevice* device);

    
    base::win::ScopedComPtr<IPortableDevice> portable_device;

    
    bool scheduled_to_delete;
  };

  typedef std::map<const base::string16, PortableDeviceInfo> PortableDeviceMap;

  
  PortableDeviceMapService();
  ~PortableDeviceMapService();

  
  PortableDeviceMap device_map_;
  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(PortableDeviceMapService);
};

#endif  
