// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_DAEMON_CONTROLLER_H_
#define REMOTING_HOST_SETUP_DAEMON_CONTROLLER_H_

#include <queue>
#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class SingleThreadTaskRunner;
}  

namespace remoting {

class AutoThread;
class AutoThreadTaskRunner;

class DaemonController : public base::RefCountedThreadSafe<DaemonController> {
 public:
  
  
  enum State {
    
    
    
    STATE_NOT_IMPLEMENTED = -1,
    
    
    
    
    STATE_NOT_INSTALLED = 0,
    
    STATE_INSTALLING = 1,
    
    STATE_STOPPED = 2,
    
    STATE_STARTING = 3,
    
    
    STATE_STARTED = 4,
    
    STATE_STOPPING = 5,
    
    
    
    STATE_UNKNOWN = 6
  };

  
  enum AsyncResult {
    RESULT_OK = 0,

    
    RESULT_FAILED = 1,

    
    
    RESULT_CANCELLED = 2,

    
    RESULT_FAILED_DIRECTORY = 3

    
    
  };

  
  
  
  
  typedef base::Callback<void (scoped_ptr<base::DictionaryValue> config)>
      GetConfigCallback;

  
  
  typedef base::Callback<void (AsyncResult result)> CompletionCallback;

  
  typedef base::Callback<void (const std::string&)> GetVersionCallback;

  struct UsageStatsConsent {
    
    bool supported;

    
    bool allowed;

    
    
    bool set_by_policy;
  };

  
  typedef base::Callback<void (const UsageStatsConsent&)>
      GetUsageStatsConsentCallback;

  
  
  
  
  
  
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    virtual State GetState() = 0;

    
    
    virtual scoped_ptr<base::DictionaryValue> GetConfig() = 0;

    
    
    
    virtual void SetConfigAndStart(
        scoped_ptr<base::DictionaryValue> config,
        bool consent,
        const CompletionCallback& done) = 0;

    
    
    
    
    
    virtual void UpdateConfig(
        scoped_ptr<base::DictionaryValue> config,
        const CompletionCallback& done) = 0;

    
    
    virtual void Stop(const CompletionCallback& done) = 0;

    
    
    virtual void SetWindow(void* window_handle) = 0;

    
    
    virtual std::string GetVersion() = 0;

    
    virtual UsageStatsConsent GetUsageStatsConsent() = 0;
  };

  static scoped_refptr<DaemonController> Create();

  explicit DaemonController(scoped_ptr<Delegate> delegate);

  
  
  
  
  
  
  State GetState();

  
  
  
  void GetConfig(const GetConfigCallback& done);

  
  
  
  
  
  
  
  
  void SetConfigAndStart(scoped_ptr<base::DictionaryValue> config,
                         bool consent,
                         const CompletionCallback& done);

  
  
  
  
  
  void UpdateConfig(scoped_ptr<base::DictionaryValue> config,
                    const CompletionCallback& done);

  
  
  
  
  
  
  void Stop(const CompletionCallback& done);

  
  
  void SetWindow(void* window_handle);

  
  
  void GetVersion(const GetVersionCallback& done);

  
  void GetUsageStatsConsent(const GetUsageStatsConsentCallback& done);

 private:
  friend class base::RefCountedThreadSafe<DaemonController>;
  virtual ~DaemonController();

  
  void DoGetConfig(const GetConfigCallback& done);
  void DoSetConfigAndStart(scoped_ptr<base::DictionaryValue> config,
                           bool consent,
                           const CompletionCallback& done);
  void DoUpdateConfig(scoped_ptr<base::DictionaryValue> config,
                      const CompletionCallback& done);
  void DoStop(const CompletionCallback& done);
  void DoSetWindow(void* window_handle, const base::Closure& done);
  void DoGetVersion(const GetVersionCallback& done);
  void DoGetUsageStatsConsent(const GetUsageStatsConsentCallback& done);

  
  
  void InvokeCompletionCallbackAndScheduleNext(
      const CompletionCallback& done,
      AsyncResult result);
  void InvokeConfigCallbackAndScheduleNext(
      const GetConfigCallback& done,
      scoped_ptr<base::DictionaryValue> config);
  void InvokeConsentCallbackAndScheduleNext(
      const GetUsageStatsConsentCallback& done,
      const UsageStatsConsent& consent);
  void InvokeVersionCallbackAndScheduleNext(
      const GetVersionCallback& done,
      const std::string& version);

  
  void ScheduleNext();
  void ServiceOrQueueRequest(const base::Closure& request);
  void ServiceNextRequest();

  
  scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner_;

  
  
  
  scoped_refptr<AutoThreadTaskRunner> delegate_task_runner_;

  scoped_ptr<AutoThread> delegate_thread_;

  scoped_ptr<Delegate> delegate_;

  std::queue<base::Closure> pending_requests_;

  DISALLOW_COPY_AND_ASSIGN(DaemonController);
};

}  

#endif  
