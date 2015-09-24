// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_EULA_ACCEPTED_NOTIFIER_H_
#define CHROME_BROWSER_WEB_RESOURCE_EULA_ACCEPTED_NOTIFIER_H_

#include "base/basictypes.h"
#include "base/prefs/pref_change_registrar.h"

class PrefService;

class EulaAcceptedNotifier {
 public:
  
  class Observer {
   public:
    virtual void OnEulaAccepted() = 0;
  };

  explicit EulaAcceptedNotifier(PrefService* local_state);
  virtual ~EulaAcceptedNotifier();

  
  
  void Init(Observer* observer);

  
  
  
  virtual bool IsEulaAccepted();

  
  static EulaAcceptedNotifier* Create();

 protected:
  
  
  void NotifyObserver();

 private:
  
  void OnPrefChanged();

  
  PrefService* local_state_;

  
  PrefChangeRegistrar registrar_;

  
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(EulaAcceptedNotifier);
};

#endif  
