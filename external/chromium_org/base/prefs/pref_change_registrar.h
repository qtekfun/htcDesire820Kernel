// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_CHANGE_REGISTRAR_H_
#define BASE_PREFS_PREF_CHANGE_REGISTRAR_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_observer.h"

class PrefService;

class BASE_PREFS_EXPORT PrefChangeRegistrar : public PrefObserver {
 public:
  
  
  typedef base::Callback<void(const std::string&)> NamedChangeCallback;

  PrefChangeRegistrar();
  virtual ~PrefChangeRegistrar();

  
  
  void Init(PrefService* service);

  
  
  
  
  
  
  
  
  void Add(const char* path, const base::Closure& obs);
  void Add(const char* path, const NamedChangeCallback& obs);

  
  void Remove(const char* path);

  
  void RemoveAll();

  
  bool IsEmpty() const;

  
  bool IsObserved(const std::string& pref);

  
  bool IsManaged();

  
  PrefService* prefs();
  const PrefService* prefs() const;

 private:
  
  virtual void OnPreferenceChanged(PrefService* service,
                                   const std::string& pref_name) OVERRIDE;

  static void InvokeUnnamedCallback(const base::Closure& callback,
                                    const std::string& pref_name);

  typedef std::map<std::string, NamedChangeCallback> ObserverMap;

  ObserverMap observers_;
  PrefService* service_;

  DISALLOW_COPY_AND_ASSIGN(PrefChangeRegistrar);
};

#endif  
