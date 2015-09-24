// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_CHANGE_REGISTRAR_H_
#define CHROME_BROWSER_PREFS_PREF_CHANGE_REGISTRAR_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"

class PrefService;
class NotificationObserver;

class PrefChangeRegistrar {
 public:
  PrefChangeRegistrar();
  virtual ~PrefChangeRegistrar();

  
  
  void Init(PrefService* service);

  
  
  
  
  void Add(const char* path,
           NotificationObserver* obs);

  
  
  void Remove(const char* path,
              NotificationObserver* obs);

  
  void RemoveAll();

  
  bool IsEmpty() const;

 private:
  typedef std::pair<std::string, NotificationObserver*> ObserverRegistration;

  std::set<ObserverRegistration> observers_;
  PrefService* service_;

  DISALLOW_COPY_AND_ASSIGN(PrefChangeRegistrar);
};

#endif  
