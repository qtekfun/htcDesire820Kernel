// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_PREF_STORE_H_
#define CHROME_COMMON_PREF_STORE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

class Value;

class PrefStore : public base::RefCounted<PrefStore> {
 public:
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void OnPrefValueChanged(const std::string& key) = 0;
    
    virtual void OnInitializationCompleted() = 0;
  };

  
  enum ReadResult {
    
    READ_OK,
    
    READ_USE_DEFAULT,
    
    READ_NO_VALUE,
  };

  PrefStore() {}

  
  virtual void AddObserver(Observer* observer) {}
  virtual void RemoveObserver(Observer* observer) {}

  
  virtual bool IsInitializationComplete() const;

  
  
  
  virtual ReadResult GetValue(const std::string& key,
                              const Value** result) const = 0;

 protected:
  friend class base::RefCounted<PrefStore>;
  virtual ~PrefStore() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(PrefStore);
};

#endif  
