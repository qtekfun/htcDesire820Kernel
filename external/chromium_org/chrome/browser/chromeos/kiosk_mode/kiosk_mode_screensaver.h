// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_KIOSK_MODE_KIOSK_MODE_SCREENSAVER_H_
#define CHROME_BROWSER_CHROMEOS_KIOSK_MODE_KIOSK_MODE_SCREENSAVER_H_

#include "ash/wm/user_activity_observer.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"

namespace extensions {
class Extension;
}

namespace chromeos {

class KioskModeScreensaver : public ash::UserActivityObserver {
 public:
  KioskModeScreensaver();
  virtual ~KioskModeScreensaver();

 private:
  friend class KioskModeScreensaverTest;

  
  virtual void OnUserActivity(const ui::Event* event) OVERRIDE;

  
  
  void GetScreensaverCrxPath();

  
  
  void ScreensaverPathCallback(const base::FilePath& screensaver_crx);

  
  
  void SetupScreensaver(scoped_refptr<extensions::Extension> extension,
                        const base::FilePath& extension_base_path);

  base::WeakPtrFactory<KioskModeScreensaver> weak_ptr_factory_;

  base::FilePath extension_base_path_;

  DISALLOW_COPY_AND_ASSIGN(KioskModeScreensaver);
};

void InitializeKioskModeScreensaver();
void ShutdownKioskModeScreensaver();

}  

#endif  
