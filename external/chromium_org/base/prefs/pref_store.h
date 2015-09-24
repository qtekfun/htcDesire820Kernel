// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_STORE_H_
#define BASE_PREFS_PREF_STORE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/prefs/base_prefs_export.h"

namespace base {
class Value;
}

class BASE_PREFS_EXPORT PrefStore : public base::RefCounted<PrefStore> {
 public:
  
  class BASE_PREFS_EXPORT Observer {
   public:
    
    virtual void OnPrefValueChanged(const std::string& key) = 0;
    
    virtual void OnInitializationCompleted(bool succeeded) = 0;

   protected:
    virtual ~Observer() {}
  };

  PrefStore() {}

  
  virtual void AddObserver(Observer* observer) {}
  virtual void RemoveObserver(Observer* observer) {}
  virtual bool HasObservers() const;

  
  virtual bool IsInitializationComplete() const;

  
  
  
  virtual bool GetValue(const std::string& key,
                        const base::Value** result) const = 0;

 protected:
  friend class base::RefCounted<PrefStore>;
  virtual ~PrefStore() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(PrefStore);
};

#endif  
