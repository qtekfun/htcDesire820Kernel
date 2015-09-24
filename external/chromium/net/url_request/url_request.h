// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_H_
#define NET_URL_REQUEST_URL_REQUEST_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/debug/leak_tracker.h"
#include "base/logging.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "base/threading/non_thread_safe.h"
#include "googleurl/src/gurl.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_info.h"
#include "net/url_request/url_request_status.h"

namespace base {
class Time;
}  

class FilePath;

typedef std::vector<std::string> ResponseCookies;

namespace net {

class CookieOptions;
class HostPortPair;
class IOBuffer;
class SSLCertRequestInfo;
class UploadData;
class URLRequestContext;
class URLRequestJob;
class X509Certificate;

class NET_EXPORT URLRequest : public base::NonThreadSafe {
 public:
  
  
  
  
  typedef URLRequestJob* (ProtocolFactory)(URLRequest* request,
                                           const std::string& scheme);

  
  
  enum {
#define HTTP_ATOM(x) HTTP_ ## x,
#include "net/http/http_atom_list.h"
#undef HTTP_ATOM
  };

  
  
  class UserData {
   public:
    UserData() {}
    virtual ~UserData() {}
  };

  
  
  class NET_EXPORT Interceptor {
  public:
    virtual ~Interceptor() {}

    
    
    
    virtual URLRequestJob* MaybeIntercept(URLRequest* request) = 0;

    
    
    
    
    
    
    virtual URLRequestJob* MaybeInterceptRedirect(URLRequest* request,
                                                  const GURL& location);

    
    
    
    
    
    
    
    
    virtual URLRequestJob* MaybeInterceptResponse(URLRequest* request);
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class NET_EXPORT Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnReceivedRedirect(URLRequest* request,
                                    const GURL& new_url,
                                    bool* defer_redirect);

    
    
    
    
    
    virtual void OnAuthRequired(URLRequest* request,
                                AuthChallengeInfo* auth_info);

    
    
    
    
    
    virtual void OnCertificateRequested(
        URLRequest* request,
        SSLCertRequestInfo* cert_request_info);

    
    
    
    
    
    
    virtual void OnSSLCertificateError(URLRequest* request,
                                       int cert_error,
                                       X509Certificate* cert);

    
    
    
    virtual void OnGetCookies(URLRequest* request, bool blocked_by_policy);

    
    
    
    virtual void OnSetCookie(URLRequest* request,
                             const std::string& cookie_line,
                             const CookieOptions& options,
                             bool blocked_by_policy);

    
    
    
    
    
    
    virtual void OnResponseStarted(URLRequest* request) = 0;

    
    
    
    
    
    
    
    virtual void OnReadCompleted(URLRequest* request, int bytes_read) = 0;
  };

  
  URLRequest(const GURL& url, Delegate* delegate);

  
  
  
  ~URLRequest();

  
  
  
  
  UserData* GetUserData(const void* key) const;
  void SetUserData(const void* key, UserData* data);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ProtocolFactory* RegisterProtocolFactory(const std::string& scheme,
                                                  ProtocolFactory* factory);

  
  static void RegisterRequestInterceptor(Interceptor* interceptor);
  static void UnregisterRequestInterceptor(Interceptor* interceptor);

  
  static bool IsHandledProtocol(const std::string& scheme);

  
  
  
  
  
  
  static bool IsHandledURL(const GURL& url);

  
  static void AllowFileAccess();
  static bool IsFileAccessAllowed();

  
  
  const GURL& original_url() const { return url_chain_.front(); }
  
  
  const std::vector<GURL>& url_chain() const { return url_chain_; }
  const GURL& url() const { return url_chain_.back(); }

  
  
  const GURL& first_party_for_cookies() const {
      return first_party_for_cookies_;
  }
  
  
  void set_first_party_for_cookies(const GURL& first_party_for_cookies);

  
  
  
  const std::string& method() const { return method_; }
  void set_method(const std::string& method);

  
  
  
  
  const std::string& referrer() const { return referrer_; }
  void set_referrer(const std::string& referrer);
  
  GURL GetSanitizedReferrer() const;

  
  
  Delegate* delegate() const { return delegate_; }
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  void AppendBytesToUpload(const char* bytes, int bytes_len);  
  void AppendFileRangeToUpload(const FilePath& file_path,
                               uint64 offset, uint64 length,
                               const base::Time& expected_modification_time);
  void AppendFileToUpload(const FilePath& file_path) {
    AppendFileRangeToUpload(file_path, 0, kuint64max, base::Time());
  }

  
  
  void EnableChunkedUpload();

  
  
  
  
  
  void AppendChunkToUpload(const char* bytes,
                           int bytes_len,
                           bool is_last_chunk);

  
  void set_upload(UploadData* upload);

  
  UploadData* get_upload();

  
  bool has_upload() const;

  
  
  void SetExtraRequestHeaderById(int header_id, const std::string& value,
                                 bool overwrite);
  void SetExtraRequestHeaderByName(const std::string& name,
                                   const std::string& value, bool overwrite);

  
  // methods are overwritten by this method.  This method may only be called
  
  void SetExtraRequestHeaders(const HttpRequestHeaders& headers);

  const HttpRequestHeaders& extra_request_headers() const {
    return extra_request_headers_;
  }

  
  LoadState GetLoadState() const;

  
  uint64 GetUploadProgress() const;

  
  
  
  
  
  
  void GetResponseHeaderById(int header_id, std::string* value);
  void GetResponseHeaderByName(const std::string& name, std::string* value);

  
  
  void GetAllResponseHeaders(std::string* headers);

  
  
  const base::Time& request_time() const {
    return response_info_.request_time;
  }

  
  
  const base::Time& response_time() const {
    return response_info_.response_time;
  }

  
  bool was_cached() const { return response_info_.was_cached; }

  
  
  bool was_fetched_via_spdy() const {
    return response_info_.was_fetched_via_spdy;
  }

  
  
  bool was_npn_negotiated() const {
    return response_info_.was_npn_negotiated;
  }

  
  bool was_fetched_via_proxy() const {
    return response_info_.was_fetched_via_proxy;
  }

  
  
  HostPortPair GetSocketAddress() const;

  
  
  HttpResponseHeaders* response_headers() const;

  
  const SSLInfo& ssl_info() const {
    return response_info_.ssl_info;
  }

  
  
  
  
  bool GetResponseCookies(ResponseCookies* cookies);

  
  
  void GetMimeType(std::string* mime_type);

  
  
  void GetCharset(std::string* charset);

  
  
  
  int GetResponseCode();

  
  const HttpResponseInfo& response_info() const { return response_info_; }

  
  int load_flags() const { return load_flags_; }
  void set_load_flags(int flags) { load_flags_ = flags; }

  
  
  bool is_pending() const { return is_pending_; }

  
  const URLRequestStatus& status() const { return status_; }

  
  uint64 identifier() const { return identifier_; }

  
  
  void Start();

  
  
  
  
  
  
  void Cancel();

  
  
  void SimulateError(int os_error);

  
  
  
  
  void SimulateSSLError(int os_error, const SSLInfo& ssl_info);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Read(IOBuffer* buf, int max_bytes, int* bytes_read);

  
  
  
  
  
  void StopCaching();

  
  
  void FollowDeferredRedirect();

  
  
  
  
  void SetAuth(const string16& username, const string16& password);
  void CancelAuth();

  
  
  
  void ContinueWithCertificate(X509Certificate* client_cert);

  
  
  
  void ContinueDespiteLastError();

  
  URLRequestContext* context() const;
  void set_context(URLRequestContext* context);

  const BoundNetLog& net_log() const { return net_log_; }

  
  int64 GetExpectedContentSize() const;

  
  RequestPriority priority() const { return priority_; }
  void set_priority(RequestPriority priority) {
#ifdef ANDROID
    DCHECK_GE(static_cast<int>(priority), static_cast<int>(HIGHEST));
    DCHECK_LT(static_cast<int>(priority), static_cast<int>(NUM_PRIORITIES));
#else
    DCHECK_GE(priority, HIGHEST);
    DCHECK_LT(priority, NUM_PRIORITIES);
#endif
    priority_ = priority;
  }

#ifdef UNIT_TEST
  URLRequestJob* job() { return job_; }
#endif

 protected:
  
  void set_is_pending(bool value) { is_pending_ = value; }

  
  void set_status(const URLRequestStatus& value) { status_ = value; }

  
  
  int Redirect(const GURL& location, int http_status_code);

  
  void ReceivedRedirect(const GURL& location, bool* defer_redirect);

  
  
  void ResponseStarted();

  
  
  void Restart();

 private:
  friend class URLRequestJob;
  typedef std::map<const void*, linked_ptr<UserData> > UserDataMap;

  void StartInternal();

  
  
  
  
  
  void BeforeRequestComplete(int error);

  void StartJob(URLRequestJob* job);

  
  
  void RestartWithJob(URLRequestJob* job);
  void PrepareToRestart();

  
  
  
  void OrphanJob();

  
  
  void DoCancel(int os_error, const SSLInfo& ssl_info);

  
  
  
  scoped_refptr<URLRequestContext> context_;

  
  BoundNetLog net_log_;

  scoped_refptr<URLRequestJob> job_;
  scoped_refptr<UploadData> upload_;
  std::vector<GURL> url_chain_;
  GURL first_party_for_cookies_;
  GURL delegate_redirect_url_;
  std::string method_;  
  std::string referrer_;
  HttpRequestHeaders extra_request_headers_;
  int load_flags_;  
                    

  Delegate* delegate_;

  
  
  
  URLRequestStatus status_;

  
  HttpResponseInfo response_info_;

  
  
  
  bool is_pending_;

  
  UserDataMap user_data_;

  
  
  int redirect_limit_;

  
  
  uint64 final_upload_progress_;

  
  
  RequestPriority priority_;

  
  const uint64 identifier_;

  base::debug::LeakTracker<URLRequest> leak_tracker_;

  
  
  CompletionCallbackImpl<URLRequest> before_request_callback_;

  DISALLOW_COPY_AND_ASSIGN(URLRequest);
};

}  

#endif  
