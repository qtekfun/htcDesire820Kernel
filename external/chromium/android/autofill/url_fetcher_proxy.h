/*
 * Copyright 2010, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef URL_FETCHER_PROXY_H_
#define URL_FETCHER_PROXY_H_

#include "android/autofill/android_url_request_context_getter.h"
#include "android/autofill/profile_android.h"
#include "base/scoped_ptr.h"
#include "base/threading/thread.h"
#include "common/net/url_fetcher.h"
#include "net/url_request/url_request_status.h"

class URLFetcherProxy;

template <>
struct RunnableMethodTraits<class URLFetcherProxy> {
  void RetainCallee(URLFetcherProxy* obj) {
  }

  void ReleaseCallee(URLFetcherProxy* obj) {
  }
};

class URLFetcherProxy : public URLFetcher, public URLFetcher::Delegate {
public:
  URLFetcherProxy(const GURL& url,
                  URLFetcher::RequestType request_type,
                  URLFetcher::Delegate* d)
      : URLFetcher(url , URLFetcher::POST , d),
        request_type_(request_type),
        retry_(true),
        url_(url)
  {
  }

  virtual ~URLFetcherProxy()
  {
      pending_callbacks_.erase(this);
  }

  virtual void set_automatically_retry_on_5xx(bool retry)
  {
    retry_ = retry;
  }

  virtual void set_upload_data(const std::string& upload_content_type,
                               const std::string& upload_content)
  {
    upload_content_type_ = upload_content_type;
    upload_content_ = upload_content;
  }

  virtual void Start()
  {
    if ( net::URLRequestContextGetter::is_freed ) {
       net::URLRequestContextGetter::is_freed = false;
       return;
    }

    scoped_refptr<net::URLRequestContextGetter> con = request_context();
    CHECK(con.get()) << "No URLRequestContextGetter!";

    if (!con->GetURLRequestContext() || con->GetURLRequestContext() == NULL) return;

    scoped_refptr<base::MessageLoopProxy> mlp = con->GetIOMessageLoopProxy();
    
    
    mlp->PostTask(FROM_HERE, NewRunnableMethod(this, &URLFetcherProxy::DoStart));
  };

  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data)
  {
    url_ = url;
    status_ = status;
    response_code_ = response_code;
    cookies_ = cookies;
    data_ = data;
    pending_callbacks_[this] = true;
    DoComplete(this);
  }

  net::HttpResponseHeaders* response_headers() const {return real_fetcher_->response_headers();};

  
  
  base::TimeDelta backoff_delay() const { return real_fetcher_->backoff_delay();};

private:
  void DoStart()
  {
    if ( net::URLRequestContextGetter::is_freed ) {
       net::URLRequestContextGetter::is_freed = false;
       return;
    }
    real_fetcher_.reset(new URLFetcher(url_, request_type_, this));
    real_fetcher_->set_automatically_retry_on_5xx(retry_);
    
    real_fetcher_->set_upload_data(upload_content_type_, upload_content_);
    net::URLRequestContextGetter* context_getter = request_context();
    if (!context_getter || context_getter == NULL ) return;
    if (!context_getter->GetURLRequestContext() || context_getter->GetURLRequestContext() == NULL) return;
    
    real_fetcher_->set_request_context(context_getter);
    real_fetcher_->Start();
  };

  static void DoComplete(void* context)
  {
    URLFetcherProxy* that = static_cast<URLFetcherProxy*>(context);
    if (pending_callbacks_[that]) {
      that->DoCompleteImpl();
    }
  }

  void DoCompleteImpl()
  {
    delegate()->OnURLFetchComplete(this, url_, status_, response_code_, cookies_, data_);
  }

  scoped_ptr<URLFetcher> real_fetcher_;
  URLFetcher::RequestType request_type_;

  bool retry_;
  std::string upload_content_type_;
  std::string upload_content_;

  GURL url_;
  net::URLRequestStatus status_;
  int response_code_;
  ResponseCookies cookies_;
  std::string data_;

  static std::map<URLFetcherProxy*, bool> pending_callbacks_;

  DISALLOW_EVIL_CONSTRUCTORS(URLFetcherProxy);
};

#endif  
