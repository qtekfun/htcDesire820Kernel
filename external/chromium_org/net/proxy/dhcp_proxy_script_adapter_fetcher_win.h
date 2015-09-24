// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_DHCP_PROXY_SCRIPT_ADAPTER_FETCHER_WIN_H_
#define NET_PROXY_DHCP_PROXY_SCRIPT_ADAPTER_FETCHER_WIN_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "url/gurl.h"

namespace base {
class TaskRunner;
}

namespace net {

class ProxyScriptFetcher;
class URLRequestContext;

class NET_EXPORT_PRIVATE DhcpProxyScriptAdapterFetcher
    : public base::SupportsWeakPtr<DhcpProxyScriptAdapterFetcher>,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  DhcpProxyScriptAdapterFetcher(URLRequestContext* url_request_context,
                                scoped_refptr<base::TaskRunner> task_runner);
  virtual ~DhcpProxyScriptAdapterFetcher();

  
  
  
  
  
  
  
  
  virtual void Fetch(const std::string& adapter_name,
                     const net::CompletionCallback& callback);

  
  virtual void Cancel();

  
  virtual bool DidFinish() const;

  
  
  
  virtual int GetResult() const;

  
  
  
  virtual base::string16 GetPacScript() const;

  
  
  
  
  
  virtual GURL GetPacURL() const;

  
  
  
  
  
  static std::string GetPacURLFromDhcp(const std::string& adapter_name);

  
  static std::string SanitizeDhcpApiString(const char* data,
                                           size_t count_bytes);

 protected:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum State {
    STATE_START,
    STATE_WAIT_DHCP,
    STATE_WAIT_URL,
    STATE_FINISH,
    STATE_CANCEL,
  };

  State state() const;

  
  
  
  class NET_EXPORT_PRIVATE DhcpQuery
      : public base::RefCountedThreadSafe<DhcpQuery> {
   public:
    DhcpQuery();
    virtual ~DhcpQuery();

    
    
    
    void GetPacURLForAdapter(const std::string& adapter_name);

    
    const std::string& url() const;

   protected:
    
    virtual std::string ImplGetPacURLFromDhcp(const std::string& adapter_name);

   private:
    
    std::string url_;

    DISALLOW_COPY_AND_ASSIGN(DhcpQuery);
  };

  
  virtual ProxyScriptFetcher* ImplCreateScriptFetcher();
  virtual DhcpQuery* ImplCreateDhcpQuery();
  virtual base::TimeDelta ImplGetTimeout() const;

 private:
  
  void OnDhcpQueryDone(scoped_refptr<DhcpQuery> dhcp_query);
  void OnTimeout();
  void OnFetcherDone(int result);
  void TransitionToFinish();

  
  scoped_refptr<base::TaskRunner> task_runner_;

  
  State state_;

  
  int result_;

  
  base::string16 pac_script_;

  
  GURL pac_url_;

  
  
  net::CompletionCallback callback_;

  
  scoped_ptr<ProxyScriptFetcher> script_fetcher_;

  
  base::OneShotTimer<DhcpProxyScriptAdapterFetcher> wait_timer_;

  URLRequestContext* const url_request_context_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(DhcpProxyScriptAdapterFetcher);
};

}  

#endif  
