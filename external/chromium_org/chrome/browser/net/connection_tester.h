// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CONNECTION_TESTER_H_
#define CHROME_BROWSER_NET_CONNECTION_TESTER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "url/gurl.h"

namespace net {
class NetLog;
class URLRequestContext;
}  


class ConnectionTester {
 public:
  
  enum ProxySettingsExperiment {
    
    PROXY_EXPERIMENT_USE_DIRECT = 0,

    
    PROXY_EXPERIMENT_USE_SYSTEM_SETTINGS,

    
    PROXY_EXPERIMENT_USE_FIREFOX_SETTINGS,

    
    PROXY_EXPERIMENT_USE_AUTO_DETECT,

    PROXY_EXPERIMENT_COUNT,
  };

  
  enum HostResolverExperiment {
    
    HOST_RESOLVER_EXPERIMENT_PLAIN = 0,

    
    HOST_RESOLVER_EXPERIMENT_DISABLE_IPV6,

    
    HOST_RESOLVER_EXPERIMENT_IPV6_PROBE,

    HOST_RESOLVER_EXPERIMENT_COUNT,
  };

  
  struct Experiment {
    Experiment(const GURL& url,
               ProxySettingsExperiment proxy_settings_experiment,
               HostResolverExperiment host_resolver_experiment)
        : url(url),
          proxy_settings_experiment(proxy_settings_experiment),
          host_resolver_experiment(host_resolver_experiment) {
    }

    
    GURL url;

    
    ProxySettingsExperiment proxy_settings_experiment;

    
    HostResolverExperiment host_resolver_experiment;
  };

  typedef std::vector<Experiment> ExperimentList;

  
  
  
  
  
  class Delegate {
   public:
    
    virtual void OnStartConnectionTestSuite() = 0;

    
    virtual void OnStartConnectionTestExperiment(
        const Experiment& experiment) = 0;

    
    
    
    
    virtual void OnCompletedConnectionTestExperiment(
        const Experiment& experiment,
        int result) = 0;

    
    virtual void OnCompletedConnectionTestSuite() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  ConnectionTester(Delegate* delegate,
                   net::URLRequestContext* proxy_request_context,
                   net::NetLog* net_log);

  
  ~ConnectionTester();

  
  
  void RunAllTests(const GURL& url);

  
  static base::string16 ProxySettingsExperimentDescription(
      ProxySettingsExperiment experiment);
  static base::string16 HostResolverExperimentDescription(
      HostResolverExperiment experiment);

 private:
  
  
  class TestRunner;
  friend class TestRunner;

  
  static void GetAllPossibleExperimentCombinations(const GURL& url,
                                                   ExperimentList* list);

  
  void StartNextExperiment();

  
  void OnExperimentCompleted(int result);

  
  const Experiment& current_experiment() const {
    return remaining_experiments_.front();
  }

  
  Delegate* delegate_;

  
  scoped_ptr<TestRunner> current_test_runner_;

  
  
  ExperimentList remaining_experiments_;

  net::URLRequestContext* const proxy_request_context_;

  net::NetLog* net_log_;

  DISALLOW_COPY_AND_ASSIGN(ConnectionTester);
};

#endif  
