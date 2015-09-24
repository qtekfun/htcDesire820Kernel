// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_STORAGE_MONITOR_TRANSIENT_DEVICE_IDS_H_
#define CHROME_BROWSER_STORAGE_MONITOR_TRANSIENT_DEVICE_IDS_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/threading/thread_checker.h"

class TransientDeviceIds {
 public:
  TransientDeviceIds();
  ~TransientDeviceIds();

  
  
  
  
  std::string GetTransientIdForDeviceId(const std::string& device_id);

  
  
  std::string DeviceIdFromTransientId(const std::string& transient_id) const;

 private:
  typedef std::map<std::string, std::string> IdMap;

  IdMap device_id_map_;
  IdMap transient_id_map_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(TransientDeviceIds);
};

#endif  
