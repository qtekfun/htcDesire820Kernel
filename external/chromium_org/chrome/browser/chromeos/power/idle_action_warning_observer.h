// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POWER_IDLE_ACTION_WARNING_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_POWER_IDLE_ACTION_WARNING_OBSERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chromeos/dbus/power_manager_client.h"

namespace chromeos {

class IdleActionWarningDialogView;

class IdleActionWarningObserver : public PowerManagerClient::Observer {
 public:
  IdleActionWarningObserver();
  virtual ~IdleActionWarningObserver();

  
  virtual void IdleActionImminent() OVERRIDE;
  virtual void IdleActionDeferred() OVERRIDE;

 private:
  IdleActionWarningDialogView* warning_dialog_;  

  DISALLOW_COPY_AND_ASSIGN(IdleActionWarningObserver);
};

}  

#endif  
