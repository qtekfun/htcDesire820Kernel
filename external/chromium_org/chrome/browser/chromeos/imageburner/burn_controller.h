// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_IMAGEBURNER_BURN_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_IMAGEBURNER_BURN_CONTROLLER_H_

#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/imageburner/burn_manager.h"
#include "chromeos/disks/disk_mount_manager.h"

namespace base {
class FilePath;
class TimeDelta;
}

namespace content {
class WebContents;
}

namespace chromeos {
namespace imageburner {

class BurnController {
 public:
  class Delegate {
   public:
    
    virtual void OnSuccess() = 0;
    
    virtual void OnFail(int error_message_id) = 0;
    
    virtual void OnDeviceAdded(const disks::DiskMountManager::Disk& disk) = 0;
    
    virtual void OnDeviceRemoved(const disks::DiskMountManager::Disk& disk) = 0;
    
    virtual void OnDeviceTooSmall(int64 device_size) = 0;
    
    virtual void OnProgress(ProgressType progress_type,
                            int64 amount_finished,
                            int64 amount_total) = 0;
    
    
    virtual void OnProgressWithRemainingTime(
        ProgressType progress_type,
        int64 amount_finished,
        int64 amount_total,
        const base::TimeDelta& time_remaining) = 0;
    
    virtual void OnNetworkDetected() = 0;
    
    virtual void OnNoNetwork() = 0;
  };

  virtual ~BurnController() {}

  
  virtual void Init() = 0;
  
  virtual std::vector<disks::DiskMountManager::Disk> GetBurnableDevices() = 0;
  
  virtual void StartBurnImage(const base::FilePath& target_device_path,
                              const base::FilePath& target_file_path) = 0;
  
  virtual void CancelBurnImage() = 0;
  
  static BurnController* CreateBurnController(
      content::WebContents* web_contents, Delegate* delegate);

 protected:
  BurnController() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(BurnController);
};

}  
}  

#endif  
