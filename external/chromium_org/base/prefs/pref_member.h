// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_MEMBER_H_
#define BASE_PREFS_PREF_MEMBER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_observer.h"
#include "base/values.h"

class PrefService;

namespace subtle {

class BASE_PREFS_EXPORT PrefMemberBase : public PrefObserver {
 public:
  
  
  typedef base::Callback<void(const std::string&)> NamedChangeCallback;

  PrefService* prefs() { return prefs_; }
  const PrefService* prefs() const { return prefs_; }

 protected:
  class BASE_PREFS_EXPORT Internal
      : public base::RefCountedThreadSafe<Internal> {
   public:
    Internal();

    
    
    
    void UpdateValue(base::Value* value,
                     bool is_managed,
                     bool is_user_modifiable,
                     const base::Closure& callback) const;

    void MoveToThread(
        const scoped_refptr<base::MessageLoopProxy>& message_loop);

    
    bool IsManaged() const {
      return is_managed_;
    }

    bool IsUserModifiable() const {
      return is_user_modifiable_;
    }

   protected:
    friend class base::RefCountedThreadSafe<Internal>;
    virtual ~Internal();

    void CheckOnCorrectThread() const {
      DCHECK(IsOnCorrectThread());
    }

   private:
    
    
    virtual bool UpdateValueInternal(const base::Value& value) const = 0;

    bool IsOnCorrectThread() const;

    scoped_refptr<base::MessageLoopProxy> thread_loop_;
    mutable bool is_managed_;
    mutable bool is_user_modifiable_;

    DISALLOW_COPY_AND_ASSIGN(Internal);
  };

  PrefMemberBase();
  virtual ~PrefMemberBase();

  
  void Init(const char* pref_name, PrefService* prefs,
            const NamedChangeCallback& observer);
  void Init(const char* pref_name, PrefService* prefs);

  virtual void CreateInternal() const = 0;

  
  void Destroy();

  void MoveToThread(const scoped_refptr<base::MessageLoopProxy>& message_loop);

  
  virtual void OnPreferenceChanged(PrefService* service,
                                   const std::string& pref_name) OVERRIDE;

  void VerifyValuePrefName() const {
    DCHECK(!pref_name_.empty());
  }

  
  
  
  void UpdateValueFromPref(const base::Closure& callback) const;

  
  
  void VerifyPref() const;

  const std::string& pref_name() const { return pref_name_; }

  virtual Internal* internal() const = 0;

  
  static void InvokeUnnamedCallback(const base::Closure& callback,
                                    const std::string& pref_name);

 private:
  
  std::string pref_name_;
  NamedChangeCallback observer_;
  PrefService* prefs_;

 protected:
  bool setting_value_;
};

bool BASE_PREFS_EXPORT PrefMemberVectorStringUpdate(
    const base::Value& value,
    std::vector<std::string>* string_vector);

}  

template <typename ValueType>
class PrefMember : public subtle::PrefMemberBase {
 public:
  
  
  PrefMember() {}
  virtual ~PrefMember() {}

  
  
  
  void Init(const char* pref_name, PrefService* prefs,
            const NamedChangeCallback& observer) {
    subtle::PrefMemberBase::Init(pref_name, prefs, observer);
  }
  void Init(const char* pref_name, PrefService* prefs,
            const base::Closure& observer) {
    subtle::PrefMemberBase::Init(
        pref_name, prefs,
        base::Bind(&PrefMemberBase::InvokeUnnamedCallback, observer));
  }
  void Init(const char* pref_name, PrefService* prefs) {
    subtle::PrefMemberBase::Init(pref_name, prefs);
  }

  
  
  
  
  
  
  void Destroy() {
    subtle::PrefMemberBase::Destroy();
  }

  
  
  
  
  
  void MoveToThread(const scoped_refptr<base::MessageLoopProxy>& message_loop) {
    subtle::PrefMemberBase::MoveToThread(message_loop);
  }

  
  
  
  
  
  bool IsManaged() const {
    VerifyPref();
    return internal_->IsManaged();
  }

  
  
  
  
  
  bool IsUserModifiable() const {
    VerifyPref();
    return internal_->IsUserModifiable();
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

    virtual BASE_PREFS_EXPORT bool UpdateValueInternal(
        const base::Value& value) const OVERRIDE;

    
    
    mutable ValueType value_;

    DISALLOW_COPY_AND_ASSIGN(Internal);
  };

  virtual Internal* internal() const OVERRIDE { return internal_.get(); }
  virtual void CreateInternal() const OVERRIDE { internal_ = new Internal(); }

  
  void BASE_PREFS_EXPORT UpdatePref(const ValueType& value);

  mutable scoped_refptr<Internal> internal_;

  DISALLOW_COPY_AND_ASSIGN(PrefMember);
};


template <>
BASE_PREFS_EXPORT void PrefMember<bool>::UpdatePref(const bool& value);

template <>
BASE_PREFS_EXPORT bool PrefMember<bool>::Internal::UpdateValueInternal(
    const base::Value& value) const;

template <>
BASE_PREFS_EXPORT void PrefMember<int>::UpdatePref(const int& value);

template <>
BASE_PREFS_EXPORT bool PrefMember<int>::Internal::UpdateValueInternal(
    const base::Value& value) const;

template <>
BASE_PREFS_EXPORT void PrefMember<double>::UpdatePref(const double& value);

template <>
BASE_PREFS_EXPORT bool PrefMember<double>::Internal::UpdateValueInternal(
    const base::Value& value) const;

template <>
BASE_PREFS_EXPORT void PrefMember<std::string>::UpdatePref(
    const std::string& value);

template <>
BASE_PREFS_EXPORT bool PrefMember<std::string>::Internal::UpdateValueInternal(
    const base::Value& value) const;

template <>
BASE_PREFS_EXPORT void PrefMember<base::FilePath>::UpdatePref(
    const base::FilePath& value);

template <>
BASE_PREFS_EXPORT bool
PrefMember<base::FilePath>::Internal::UpdateValueInternal(
    const base::Value& value) const;

template <>
BASE_PREFS_EXPORT void PrefMember<std::vector<std::string> >::UpdatePref(
    const std::vector<std::string>& value);

template <>
BASE_PREFS_EXPORT bool
PrefMember<std::vector<std::string> >::Internal::UpdateValueInternal(
    const base::Value& value) const;

typedef PrefMember<bool> BooleanPrefMember;
typedef PrefMember<int> IntegerPrefMember;
typedef PrefMember<double> DoublePrefMember;
typedef PrefMember<std::string> StringPrefMember;
typedef PrefMember<base::FilePath> FilePathPrefMember;
typedef PrefMember<std::vector<std::string> > StringListPrefMember;

#endif  
