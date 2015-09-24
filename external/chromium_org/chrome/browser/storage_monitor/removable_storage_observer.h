// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_REMOVABLE_STORAGE_OBSERVER_H_
#define CHROME_BROWSER_STORAGE_MONITOR_REMOVABLE_STORAGE_OBSERVER_H_

#include "chrome/browser/storage_monitor/storage_info.h"

class RemovableStorageObserver {
 public:
  
  
  virtual void OnRemovableStorageAttached(const StorageInfo& info) {}

  
  
  virtual void OnRemovableStorageDetached(const StorageInfo& info) {}

 protected:
  virtual ~RemovableStorageObserver() {}
};

#endif  
