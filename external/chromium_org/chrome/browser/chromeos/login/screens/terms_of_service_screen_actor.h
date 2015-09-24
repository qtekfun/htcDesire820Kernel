// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_TERMS_OF_SERVICE_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_TERMS_OF_SERVICE_SCREEN_ACTOR_H_

#include <string>

namespace chromeos {

class TermsOfServiceScreenActor {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnDecline() = 0;

    
    virtual void OnAccept() = 0;

    
    virtual void OnActorDestroyed(TermsOfServiceScreenActor* actor) = 0;
  };

  virtual ~TermsOfServiceScreenActor() {}

  
  virtual void SetDelegate(Delegate* screen) = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual void SetDomain(const std::string& domain) = 0;

  
  
  virtual void OnLoadError() = 0;

  
  
  virtual void OnLoadSuccess(const std::string& terms_of_service) = 0;
};

}  

#endif  
