// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_DHCP_PROXY_SCRIPT_FETCHER_WIN_H_
#define NET_PROXY_DHCP_PROXY_SCRIPT_FETCHER_WIN_H_

#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/proxy/dhcp_proxy_script_fetcher.h"

namespace base {
class SequencedWorkerPool;
}

namespace net {

class DhcpProxyScriptAdapterFetcher;
class URLRequestContext;

class NET_EXPORT_PRIVATE DhcpProxyScriptFetcherWin
    : public DhcpProxyScriptFetcher,
      public base::SupportsWeakPtr<DhcpProxyScriptFetcherWin>,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  
  explicit DhcpProxyScriptFetcherWin(URLRequestContext* url_request_context);
  virtual ~DhcpProxyScriptFetcherWin();

  
  int Fetch(base::string16* utf16_text,
            const net::CompletionCallback& callback) OVERRIDE;
  void Cancel() OVERRIDE;
  const GURL& GetPacURL() const OVERRIDE;
  std::string GetFetcherName() const OVERRIDE;

  
  
  
  static bool GetCandidateAdapterNames(std::set<std::string>* adapter_names);

 protected:
  int num_pending_fetchers() const;

  URLRequestContext* url_request_context() const;

  scoped_refptr<base::TaskRunner> GetTaskRunner();

  
  
  
  class NET_EXPORT_PRIVATE AdapterQuery
      : public base::RefCountedThreadSafe<AdapterQuery> {
   public:
    AdapterQuery();
    virtual ~AdapterQuery();

    
    void GetCandidateAdapterNames();

    
    
    const std::set<std::string>& adapter_names() const;

   protected:
    
    virtual bool ImplGetCandidateAdapterNames(
        std::set<std::string>* adapter_names);

   private:
    
    
    
    std::set<std::string> adapter_names_;

    DISALLOW_COPY_AND_ASSIGN(AdapterQuery);
  };

  
  virtual DhcpProxyScriptAdapterFetcher* ImplCreateAdapterFetcher();
  virtual AdapterQuery* ImplCreateAdapterQuery();
  virtual base::TimeDelta ImplGetMaxWait();
  virtual void ImplOnGetCandidateAdapterNamesDone() {}

 private:
  
  void CancelImpl();
  void OnGetCandidateAdapterNamesDone(scoped_refptr<AdapterQuery> query);
  void OnFetcherDone(int result);
  void OnWaitTimer();
  void TransitionToDone();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum State {
    STATE_START,
    STATE_WAIT_ADAPTERS,
    STATE_NO_RESULTS,
    STATE_SOME_RESULTS,
    STATE_DONE,
  };

  
  State state_;

  
  
  
  typedef ScopedVector<DhcpProxyScriptAdapterFetcher> FetcherVector;
  FetcherVector fetchers_;

  
  int num_pending_fetchers_;

  
  net::CompletionCallback callback_;

  
  
  base::string16* destination_string_;

  
  GURL pac_url_;

  base::OneShotTimer<DhcpProxyScriptFetcherWin> wait_timer_;

  URLRequestContext* const url_request_context_;

  
  scoped_refptr<AdapterQuery> last_query_;

  
  base::TimeTicks fetch_start_time_;

  
  scoped_refptr<base::SequencedWorkerPool> worker_pool_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(DhcpProxyScriptFetcherWin);
};

}  

#endif  
