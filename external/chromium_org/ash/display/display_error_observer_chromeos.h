// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_ERROR_OBSERVER_CHROMEOS_H_
#define ASH_DISPLAY_DISPLAY_ERROR_OBSERVER_CHROMEOS_H_

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chromeos/display/output_configurator.h"

namespace ash {
namespace internal {

class ASH_EXPORT DisplayErrorObserver
    : public chromeos::OutputConfigurator::Observer {
 public:
  DisplayErrorObserver();
  virtual ~DisplayErrorObserver();

  
  virtual void OnDisplayModeChangeFailed(
      chromeos::OutputState failed_new_state) OVERRIDE;

 private:
  friend class DisplayErrorObserverTest;

  base::string16 GetTitleOfDisplayErrorNotificationForTest();

  DISALLOW_COPY_AND_ASSIGN(DisplayErrorObserver);
};

}  
}  

#endif  
