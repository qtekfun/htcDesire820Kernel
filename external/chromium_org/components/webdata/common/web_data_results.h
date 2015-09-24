// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATA_RESULTS_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATA_RESULTS_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "components/webdata/common/webdata_export.h"

class WDTypedResult;

typedef enum {
  BOOL_RESULT = 1,             
  KEYWORDS_RESULT,             
  INT64_RESULT,                
#if defined(OS_WIN)
  PASSWORD_IE7_RESULT,         
#endif
  WEB_APP_IMAGES,              
  TOKEN_RESULT,                
  AUTOFILL_VALUE_RESULT,       
  AUTOFILL_CHANGES,            
  AUTOFILL_PROFILE_RESULT,     
  AUTOFILL_PROFILES_RESULT,    
  AUTOFILL_CREDITCARD_RESULT,  
  AUTOFILL_CREDITCARDS_RESULT, 
  WEB_INTENTS_RESULT,          
  WEB_INTENTS_DEFAULTS_RESULT, 
} WDResultType;


typedef base::Callback<void(const WDTypedResult*)> DestroyCallback;

class WEBDATA_EXPORT WDTypedResult {
 public:
  virtual ~WDTypedResult() {
  }

  
  WDResultType GetType() const {
    return type_;
  }

  virtual void Destroy() {
  }

 protected:
  explicit WDTypedResult(WDResultType type)
    : type_(type) {
  }

 private:
  WDResultType type_;
  DISALLOW_COPY_AND_ASSIGN(WDTypedResult);
};

template <class T> class WDResult : public WDTypedResult {
 public:
  WDResult(WDResultType type, const T& v)
      : WDTypedResult(type), value_(v) {
  }

  virtual ~WDResult() {
  }

  
  T GetValue() const {
    return value_;
  }

 private:
  T value_;

  DISALLOW_COPY_AND_ASSIGN(WDResult);
};

template <class T> class WDDestroyableResult : public WDTypedResult {
 public:
  WDDestroyableResult(
      WDResultType type,
      const T& v,
      const DestroyCallback& callback)
      : WDTypedResult(type),
        value_(v),
        callback_(callback) {
  }

  virtual ~WDDestroyableResult() {
  }


  virtual void Destroy()  OVERRIDE {
    if (!callback_.is_null()) {
      callback_.Run(this);
    }
  }

  
  T GetValue() const {
    return value_;
  }

 private:
  T value_;
  DestroyCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(WDDestroyableResult);
};

template <class T> class WDObjectResult : public WDTypedResult {
 public:
  explicit WDObjectResult(WDResultType type)
    : WDTypedResult(type) {
  }

  T* GetValue() const {
    return &value_;
  }

 private:
  
  mutable T value_;
  DISALLOW_COPY_AND_ASSIGN(WDObjectResult);
};

#endif  
