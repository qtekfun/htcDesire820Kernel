// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_ACCESSIBILITY_MAGNIFICATION_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_ACCESSIBILITY_MAGNIFICATION_MANAGER_H_

#include "chrome/browser/chromeos/accessibility/accessibility_manager.h"

class Profile;

namespace chromeos {


class MagnificationManager {
 public:
  
  
  
  static void Initialize();

  
  static void Shutdown();

  
  static MagnificationManager* Get();

  
  virtual bool IsMagnifierEnabled() const = 0;

  
  virtual ash::MagnifierType GetMagnifierType() const = 0;

  
  virtual void SetMagnifierEnabled(bool enabled) = 0;

  
  virtual void SetMagnifierType(ash::MagnifierType type) = 0;

  
  virtual void SaveScreenMagnifierScale(double scale) = 0;

  
  virtual double GetSavedScreenMagnifierScale() const = 0;

  virtual void SetProfileForTest(Profile* profile) = 0;

 protected:
  virtual ~MagnificationManager() {}
};

}  

#endif  
