// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SIGNIN_LOGIN_UI_SERVICE_H_
#define CHROME_BROWSER_UI_WEBUI_SIGNIN_LOGIN_UI_SERVICE_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class Browser;
class Profile;

class LoginUIService : public BrowserContextKeyedService {
 public:
  
  
  class LoginUI {
   public:
    
    virtual void FocusUI() = 0;

    
    
    virtual void CloseUI() = 0;
   protected:
    virtual ~LoginUI() {}
  };

  
  class Observer {
   public:
    
    
    virtual void OnLoginUIShown(LoginUI* ui) = 0;

    
    
    virtual void OnLoginUIClosed(LoginUI* ui) = 0;

   protected:
    virtual ~Observer() {}
  };

  explicit LoginUIService(Profile* profile);
  virtual ~LoginUIService();

  
  LoginUI* current_login_ui() const {
    return ui_;
  }

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  void SetLoginUI(LoginUI* ui);

  
  
  void LoginUIClosed(LoginUI* ui);

  
  
  
  void ShowLoginPopup();

 private:
  
  LoginUI* ui_;
  Profile* profile_;

  
  ObserverList<Observer> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(LoginUIService);
};

#endif  
