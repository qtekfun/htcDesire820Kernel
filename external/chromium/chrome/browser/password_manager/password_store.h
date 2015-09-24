// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_H_
#pragma once

#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/threading/thread.h"
#include "base/time.h"
#include "content/browser/cancelable_request.h"

class PasswordStoreConsumer;
class Task;

namespace browser_sync {
class PasswordDataTypeController;
class PasswordModelAssociator;
class PasswordModelWorker;
};

namespace webkit_glue {
struct PasswordForm;
};

class PasswordStore
    : public base::RefCountedThreadSafe<PasswordStore>,
      public CancelableRequestProvider {
 public:
  typedef Callback2<Handle,
                    const std::vector<webkit_glue::PasswordForm*>&>::Type
      GetLoginsCallback;

  
  
  
  
  
  
  
  
  class GetLoginsRequest : public CancelableRequest1<
    GetLoginsCallback, std::vector<webkit_glue::PasswordForm*> > {
   public:
    explicit GetLoginsRequest(GetLoginsCallback* callback);
    virtual ~GetLoginsRequest();

   private:
    DISALLOW_COPY_AND_ASSIGN(GetLoginsRequest);
  };

  
  
  
  class Observer {
   public:
    
    virtual void OnLoginsChanged() = 0;

   protected:
    virtual ~Observer() {}
  };

  PasswordStore();

  
  virtual bool Init();

  
  virtual void Shutdown();

  
  virtual void AddLogin(const webkit_glue::PasswordForm& form);

  
  void UpdateLogin(const webkit_glue::PasswordForm& form);

  
  void RemoveLogin(const webkit_glue::PasswordForm& form);

  
  void RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                  const base::Time& delete_end);

  
  
  
  virtual Handle GetLogins(const webkit_glue::PasswordForm& form,
                           PasswordStoreConsumer* consumer);

  
  
  
  
  Handle GetAutofillableLogins(PasswordStoreConsumer* consumer);

  
  
  
  Handle GetBlacklistLogins(PasswordStoreConsumer* consumer);

  
  void ReportMetrics();

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

 protected:
  friend class base::RefCountedThreadSafe<PasswordStore>;
  friend class browser_sync::PasswordDataTypeController;
  friend class browser_sync::PasswordModelAssociator;
  friend class browser_sync::PasswordModelWorker;
  friend class LivePasswordsSyncTest;

  virtual ~PasswordStore();

  
  
  virtual GetLoginsRequest* NewGetLoginsRequest(GetLoginsCallback* callback);

  
  virtual void ScheduleTask(Task* task);

  
  
  virtual void ReportMetricsImpl() = 0;
  
  virtual void AddLoginImpl(const webkit_glue::PasswordForm& form) = 0;
  
  virtual void UpdateLoginImpl(const webkit_glue::PasswordForm& form) = 0;
  
  virtual void RemoveLoginImpl(const webkit_glue::PasswordForm& form) = 0;
  
  virtual void RemoveLoginsCreatedBetweenImpl(const base::Time& delete_begin,
                                              const base::Time& delete_end) = 0;
  
  
  
  virtual void GetLoginsImpl(GetLoginsRequest* request,
                             const webkit_glue::PasswordForm& form) = 0;
  
  virtual void GetAutofillableLoginsImpl(GetLoginsRequest* request) = 0;
  
  virtual void GetBlacklistLoginsImpl(GetLoginsRequest* request) = 0;

  
  virtual bool FillAutofillableLogins(
      std::vector<webkit_glue::PasswordForm*>* forms) = 0;
  
  virtual bool FillBlacklistLogins(
      std::vector<webkit_glue::PasswordForm*>* forms) = 0;

  
  
  
  virtual void ForwardLoginsResult(GetLoginsRequest* request);

  
  
  template<typename BackendFunc>
  Handle Schedule(BackendFunc func, PasswordStoreConsumer* consumer);

  
  
  template<typename BackendFunc, typename ArgA>
  Handle Schedule(BackendFunc func, PasswordStoreConsumer* consumer,
                  const ArgA& a);

 private:
  
  
  
  
  
  
  void WrapModificationTask(Task* task);

  
  
  
  
  void PostNotifyLoginsChanged();

  
  
  
  void NotifyLoginsChanged();

  
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStore);
};

#endif  
