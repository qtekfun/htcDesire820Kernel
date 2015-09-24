// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/threading/thread.h"
#include "base/time/time.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"

class PasswordStore;
class PasswordStoreConsumer;
class PasswordSyncableService;
class Task;

namespace autofill {
struct PasswordForm;
}

namespace browser_sync {
class PasswordChangeProcessor;
class PasswordDataTypeController;
class PasswordModelAssociator;
class PasswordModelWorker;
}

namespace passwords_helper {
void AddLogin(PasswordStore* store, const autofill::PasswordForm& form);
void RemoveLogin(PasswordStore* store, const autofill::PasswordForm& form);
void UpdateLogin(PasswordStore* store, const autofill::PasswordForm& form);
}

class PasswordStore
    : public RefcountedBrowserContextKeyedService,
      public CancelableRequestProvider {
 public:
  typedef base::Callback<
      void(Handle, const std::vector<autofill::PasswordForm*>&)>
      GetLoginsCallback;

  
  
  enum AuthorizationPromptPolicy {
    ALLOW_PROMPT,
    DISALLOW_PROMPT
  };

  
  
  
  class GetLoginsRequest
      : public CancelableRequest1<GetLoginsCallback,
                                  std::vector<autofill::PasswordForm*> > {
   public:
    explicit GetLoginsRequest(const GetLoginsCallback& callback);

    void set_ignore_logins_cutoff(const base::Time& cutoff) {
      ignore_logins_cutoff_ = cutoff;
    }

    
    void ApplyIgnoreLoginsCutoff();

   protected:
    virtual ~GetLoginsRequest();

   private:
    
    base::Time ignore_logins_cutoff_;

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

  
  virtual void AddLogin(const autofill::PasswordForm& form);

  
  void UpdateLogin(const autofill::PasswordForm& form);

  
  void RemoveLogin(const autofill::PasswordForm& form);

  
  void RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                  const base::Time& delete_end);

  
  
  
  
  
  
  
  virtual CancelableTaskTracker::TaskId GetLogins(
      const autofill::PasswordForm& form,
      AuthorizationPromptPolicy prompt_policy,
      PasswordStoreConsumer* consumer);

  
  
  
  
  Handle GetAutofillableLogins(PasswordStoreConsumer* consumer);

  
  
  
  Handle GetBlacklistLogins(PasswordStoreConsumer* consumer);

  
  void ReportMetrics();

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

 protected:
  friend class base::RefCountedThreadSafe<PasswordStore>;
  
  
  
  
  friend class browser_sync::PasswordChangeProcessor;
  friend class browser_sync::PasswordDataTypeController;
  friend class browser_sync::PasswordModelAssociator;
  friend class browser_sync::PasswordModelWorker;
  friend class PasswordSyncableService;
  friend void passwords_helper::AddLogin(PasswordStore*,
                                         const autofill::PasswordForm&);
  friend void passwords_helper::RemoveLogin(PasswordStore*,
                                            const autofill::PasswordForm&);
  friend void passwords_helper::UpdateLogin(PasswordStore*,
                                            const autofill::PasswordForm&);

  virtual ~PasswordStore();

  
  
  virtual GetLoginsRequest* NewGetLoginsRequest(
      const GetLoginsCallback& callback);

  
  
  
  virtual bool ScheduleTask(const base::Closure& task);

  
  
  virtual void ReportMetricsImpl() = 0;
  
  virtual void AddLoginImpl(const autofill::PasswordForm& form) = 0;
  
  virtual void UpdateLoginImpl(const autofill::PasswordForm& form) = 0;
  
  virtual void RemoveLoginImpl(const autofill::PasswordForm& form) = 0;
  
  virtual void RemoveLoginsCreatedBetweenImpl(const base::Time& delete_begin,
                                              const base::Time& delete_end) = 0;

  typedef base::Callback<void(const std::vector<autofill::PasswordForm*>&)>
      ConsumerCallbackRunner;  

  
  
  
  virtual void GetLoginsImpl(
      const autofill::PasswordForm& form,
      AuthorizationPromptPolicy prompt_policy,
      const ConsumerCallbackRunner& callback_runner) = 0;

  
  virtual void GetAutofillableLoginsImpl(GetLoginsRequest* request) = 0;
  
  virtual void GetBlacklistLoginsImpl(GetLoginsRequest* request) = 0;

  
  virtual bool FillAutofillableLogins(
      std::vector<autofill::PasswordForm*>* forms) = 0;
  
  virtual bool FillBlacklistLogins(
      std::vector<autofill::PasswordForm*>* forms) = 0;

  
  
  virtual void ForwardLoginsResult(GetLoginsRequest* request);

  
  void LogStatsForBulkDeletion(int num_deletions);

 private:
  
  
  template<typename BackendFunc>
  Handle Schedule(BackendFunc func, PasswordStoreConsumer* consumer);

  
  
  
  template<typename BackendFunc>
  Handle Schedule(BackendFunc func,
                  PasswordStoreConsumer* consumer,
                  const autofill::PasswordForm& form,
                  const base::Time& ignore_logins_cutoff);

  
  
  
  
  
  virtual void WrapModificationTask(base::Closure task);

  
  
  
  
  void PostNotifyLoginsChanged();

  
  
  
  void NotifyLoginsChanged();

  
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStore);
};

#endif  
