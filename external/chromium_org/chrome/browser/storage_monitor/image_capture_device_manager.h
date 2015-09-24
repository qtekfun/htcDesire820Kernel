// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_IMAGE_CAPTURE_DEVICE_MANAGER_H_
#define CHROME_BROWSER_STORAGE_MONITOR_IMAGE_CAPTURE_DEVICE_MANAGER_H_

#import <Foundation/Foundation.h>
#include <string>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"

@protocol ICDeviceBrowserDelegate;
@class ImageCaptureDevice;
@class ImageCaptureDeviceManagerImpl;

class ImageCaptureDeviceManager {
 public:
  ImageCaptureDeviceManager();
  ~ImageCaptureDeviceManager();

  
  
  
  static ImageCaptureDevice* deviceForUUID(const std::string& uuid);

  
  id<ICDeviceBrowserDelegate> device_browser();

  
  
  void SetNotifications(StorageMonitor::Receiver* notifications);

  
  
  void EjectDevice(const std::string& uuid,
                   base::Callback<void(StorageMonitor::EjectStatus)> callback);

 private:
  base::scoped_nsobject<ImageCaptureDeviceManagerImpl> device_browser_;
};

#endif  
