// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_EULA_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_EULA_SCREEN_ACTOR_H_

#include <string>

#include "ui/gfx/size.h"
#include "url/gurl.h"

namespace chromeos {

class EulaScreenActor {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    virtual GURL GetOemEulaUrl() const = 0;

    
    
    virtual void OnExit(bool accepted, bool usage_stats_enabled) = 0;

    
    
    virtual void InitiatePasswordFetch() = 0;

    
    virtual bool IsUsageStatsEnabled() const = 0;

    
    
    virtual void OnActorDestroyed(EulaScreenActor* actor) = 0;
  };

  virtual ~EulaScreenActor() {}

  virtual void PrepareToShow() = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual void SetDelegate(Delegate* delegate) = 0;
  virtual void OnPasswordFetched(const std::string& tpm_password) = 0;
};

}  

#endif  
