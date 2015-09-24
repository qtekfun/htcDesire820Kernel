// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_MEMBER_H_
#define CHROME_BROWSER_PREFS_PREF_MEMBER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"

class PrefService;

namespace subtle {

class PrefMemberBase : public NotificationObserver {
 protected:
  class Internal : public base::RefCountedThreadSafe<Internal> {
   public:
    Internal();

    
    
    
    virtual void UpdateValue(Value* value, bool is_managed) const;

    void MoveToThread(BrowserThread::ID thread_id);

    
    bool IsManaged() const {
      return is_managed_;
    }

   protected:
    friend class base::RefCountedThreadSafe<Internal>;
    virtual ~Internal();

    void CheckOnCorrectThread() const {
      DCHECK(IsOnCorrectThread());
    }

   private:
    
    
    virtual bool UpdateValueInternal(const Value& value) const = 0;

    bool IsOnCorrectThread() const;

    BrowserThread::ID thread_id_;
    mutable bool is_managed_;

    DISALLOW_COPY_AND_ASSIGN(Internal);
  };

  PrefMemberBase();
  virtual ~PrefMemberBase();

  
  void Init(const char* pref_name, PrefService* prefs,
            NotificationObserver* observer);

  virtual void CreateInternal() const = 0;

  
  void Destroy();

  void MoveToThread(BrowserThread::ID thread_id);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void VerifyValuePrefName() const {
    DCHECK(!pref_name_.empty());
  }

  
  
  
  virtual void UpdateValueFromPref() const;

  
  
  void VerifyPref() const;

  const std::string& pref_name() const { return pref_name_; }
  PrefService* prefs() { return prefs_; }
  const PrefService* prefs() const { return prefs_; }

  virtual Internal* internal() const = 0;

 
 private:
  std::string pref_name_;
  NotificationObserver* observer_;
  PrefService* prefs_;

 protected:
  bool setting_value_;
};

}  

template <typename ValueType>
class PrefMember : public subtle::PrefMemberBase {
 public:
  
  
  PrefMember() {}
  virtual ~PrefMember() {}

  
  
  
  void Init(const char* pref_name, PrefService* prefs,
            NotificationObserver* observer) {
    subtle::PrefMemberBase::Init(pref_name, prefs, observer);
  }

  
  
  
  void Destroy() {
    subtle::PrefMemberBase::Destroy();
  }

  
  
  
  
  
  void MoveToThread(BrowserThread::ID thread_id) {
    subtle::PrefMemberBase::MoveToThread(thread_id);
  }

  
  
  
  
  
  bool IsManaged() const {
    VerifyPref();
    return internal_->IsManaged();
  }

  
  
  
  ValueType GetValue() const {
    VerifyPref();
    return internal_->value();
  }

  
  ValueType operator*() const {
    return GetValue();
  }

  
  
  void SetValue(const ValueType& value) {
    VerifyValuePrefName();
    setting_value_ = true;
    UpdatePref(value);
    setting_value_ = false;
  }

  
  
  void SetValueIfNotManaged(const ValueType& value) {
    if (!IsManaged()) {
      SetValue(value);
    }
  }

  
  const std::string& GetPrefName() const {
    return pref_name();
  }

 private:
  class Internal : public subtle::PrefMemberBase::Internal {
   public:
    Internal() : value_(ValueType()) {}

    ValueType value() {
      CheckOnCorrectThread();
      return value_;
    }

   protected:
    virtual ~Internal() {}

    virtual bool UpdateValueInternal(const Value& value) const;

    
    
    mutable ValueType value_;

    DISALLOW_COPY_AND_ASSIGN(Internal);
  };

  virtual Internal* internal() const { return internal_; }
  virtual void CreateInternal() const {
    internal_ = new Internal();
  }

  
  virtual void UpdatePref(const ValueType& value);

  mutable scoped_refptr<Internal> internal_;

  DISALLOW_COPY_AND_ASSIGN(PrefMember);
};

typedef PrefMember<bool> BooleanPrefMember;
typedef PrefMember<int> IntegerPrefMember;
typedef PrefMember<double> DoublePrefMember;
typedef PrefMember<std::string> StringPrefMember;
typedef PrefMember<FilePath> FilePathPrefMember;
typedef PrefMember<ListValue*> ListPrefMember;

#endif  
