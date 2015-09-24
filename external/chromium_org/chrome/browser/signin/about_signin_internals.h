// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_ABOUT_SIGNIN_INTERNALS_H_
#define CHROME_BROWSER_SIGNIN_ABOUT_SIGNIN_INTERNALS_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/values.h"
#include "chrome/browser/signin/signin_internals_util.h"
#include "chrome/browser/signin/signin_manager.h"
#include "chrome/browser/signin/signin_manager_factory.h"
#include "chrome/common/chrome_version_info.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class Profile;

class AboutSigninInternals
    : public BrowserContextKeyedService,
      public signin_internals_util::SigninDiagnosticsObserver {
 public:
  class Observer {
   public:
    
    
    virtual void OnSigninStateChanged(scoped_ptr<DictionaryValue> info) = 0;
  };

  AboutSigninInternals();
  virtual ~AboutSigninInternals();

  
  
  void AddSigninObserver(Observer* observer);
  void RemoveSigninObserver(Observer* observer);

  
  void RefreshSigninPrefs();

  
  virtual void NotifySigninValueChanged(
      const signin_internals_util::UntimedSigninStatusField& field,
      const std::string& value) OVERRIDE;

  virtual void NotifySigninValueChanged(
      const signin_internals_util::TimedSigninStatusField& field,
      const std::string& value) OVERRIDE;

  void Initialize(Profile* profile);

  
  virtual void Shutdown() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_ptr<DictionaryValue> GetSigninStatus();

  
  
  
  base::Time GetTokenTime(const std::string& token_name) const;

 private:
  void NotifyObservers();

  
  Profile* profile_;

  
  
  signin_internals_util::SigninStatus signin_status_;

  ObserverList<Observer> signin_observers_;

  DISALLOW_COPY_AND_ASSIGN(AboutSigninInternals);
};

#endif  
