// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_TASK_HELPER_MAP_SERVICE_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_TASK_HELPER_MAP_SERVICE_H_

#include <map>
#include <string>

#include "base/lazy_instance.h"

class MTPDeviceTaskHelper;

class MTPDeviceTaskHelperMapService {
 public:
  static MTPDeviceTaskHelperMapService* GetInstance();

  
  
  MTPDeviceTaskHelper* CreateDeviceTaskHelper(const std::string& storage_name);

  
  
  
  void DestroyDeviceTaskHelper(const std::string& storage_name);

  
  
  
  
  MTPDeviceTaskHelper* GetDeviceTaskHelper(const std::string& storage_name);

 private:
  friend struct base::DefaultLazyInstanceTraits<MTPDeviceTaskHelperMapService>;

  
  
  typedef std::map<std::string, MTPDeviceTaskHelper*> TaskHelperMap;

  
  MTPDeviceTaskHelperMapService();
  ~MTPDeviceTaskHelperMapService();

  
  
  TaskHelperMap task_helper_map_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceTaskHelperMapService);
};

#endif  
