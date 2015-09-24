// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_H_
#define NET_URL_REQUEST_URL_REQUEST_H_

#include <string>
#include <vector>

#include "base/debug/leak_tracker.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/supports_user_data.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/auth.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/load_timing_info.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/network_delegate.h"
#include "net/base/request_priority.h"
#include "net/base/upload_progress.h"
#include "net/cookies/canonical_cookie.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_info.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"

class ChildProcessSecurityPolicyTest;
class TestAutomationProvider;
class URLRequestAutomationJob;

namespace base {
class Value;

namespace debug {
class StackTrace;
}  
}  

namespace appcache {
class AppCacheInterceptor;
class AppCacheRequestHandlerTest;
class AppCacheURLRequestJobTest;
}

namespace content {
class ResourceDispatcherHostTest;
}

namespace fileapi {
class FileSystemDirURLRequestJobTest;
class FileSystemURLRequestJobTest;
class FileWriterDelegateTest;
}

namespace webkit_blob {
class BlobURLRequestJobTest;
}

namespace net {

class CookieOptions;
class HostPortPair;
class IOBuffer;
struct LoadTimingInfo;
class SSLCertRequestInfo;
class SSLInfo;
class UploadDataStream;
class URLRequestContext;
class URLRequestJob;
class X509Certificate;

typedef std::vector<std::string> ResponseCookies;

class NET_EXPORT URLRequest : NON_EXPORTED_BASE(public base::NonThreadSafe),
                              public base::SupportsUserData {
 public:
  
  
  
  
  typedef URLRequestJob* (ProtocolFactory)(URLRequest* request,
                                           NetworkDelegate* network_delegate,
                                           const std::string& scheme);

  
  
  enum {
#define HTTP_ATOM(x) HTTP_ ## x,
#include "net/http/http_atom_list.h"
#undef HTTP_ATOM
  };

  
  
  
  
  
  
  
  
  
  
  enum ReferrerPolicy {
    CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE,
    NEVER_CLEAR_REFERRER,
  };

  
  
  class NET_EXPORT Interceptor {
  public:
    virtual ~Interceptor() {}

    
    
    
    virtual URLRequestJob* MaybeIntercept(
        URLRequest* request, NetworkDelegate* network_delegate) = 0;

    
    
    
    
    
    
    virtual URLRequestJob* MaybeInterceptRedirect(
        URLRequest* request,
        NetworkDelegate* network_delegate,
        const GURL& location);

    
    
    
    
    
    
    
    
    virtual URLRequestJob* MaybeInterceptResponse(
        URLRequest* request, NetworkDelegate* network_delegate);
  };

  
  
  
  class NET_EXPORT Deprecated {
   private:
    
    friend class ::ChildProcessSecurityPolicyTest;
    friend class ::TestAutomationProvider;
    friend class ::URLRequestAutomationJob;
    friend class TestInterceptor;
    friend class URLRequestFilter;
    friend class appcache::AppCacheInterceptor;
    friend class appcache::AppCacheRequestHandlerTest;
    friend class appcache::AppCacheURLRequestJobTest;
    friend class content::ResourceDispatcherHostTest;
    friend class fileapi::FileSystemDirURLRequestJobTest;
    friend class fileapi::FileSystemURLRequestJobTest;
    friend class fileapi::FileWriterDelegateTest;
    friend class webkit_blob::BlobURLRequestJobTest;

    
    static ProtocolFactory* RegisterProtocolFactory(const std::string& scheme,
                                                    ProtocolFactory* factory);

    
    
    static void RegisterRequestInterceptor(Interceptor* interceptor);
    static void UnregisterRequestInterceptor(Interceptor* interceptor);

    DISALLOW_IMPLICIT_CONSTRUCTORS(Deprecated);
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class NET_EXPORT Delegate {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnReceivedRedirect(URLRequest* request,
                                    const GURL& new_url,
                                    bool* defer_redirect);

    
    
    
    
    
    virtual void OnAuthRequired(URLRequest* request,
                                AuthChallengeInfo* auth_info);

    
    
    
    
    
    virtual void OnCertificateRequested(
        URLRequest* request,
        SSLCertRequestInfo* cert_request_info);

    
    
    
    
    
    
    
    
    
    
    virtual void OnSSLCertificateError(URLRequest* request,
                                       const SSLInfo& ssl_info,
                                       bool fatal);

    
    
    
    
    
    
    virtual void OnResponseStarted(URLRequest* request) = 0;

    
    
    
    
    
    
    
    virtual void OnReadCompleted(URLRequest* request, int bytes_read) = 0;

   protected:
    virtual ~Delegate() {}
  };

  URLRequest(const GURL& url,
             RequestPriority priority,
             Delegate* delegate,
             const URLRequestContext* context);

  
  
  
  virtual ~URLRequest();

  
  
  
  
  
  
  static void SetDefaultCookiePolicyToBlock();

  
  static bool IsHandledProtocol(const std::string& scheme);

  
  
  
  
  
  
  static bool IsHandledURL(const GURL& url);

  
  
  const GURL& original_url() const { return url_chain_.front(); }
  
  
  const std::vector<GURL>& url_chain() const { return url_chain_; }
  const GURL& url() const { return url_chain_.back(); }

  
  
  
  
  
  
  
  
  
  
  
  
  const GURL& first_party_for_cookies() const {
    return first_party_for_cookies_;
  }
  
  
  void set_first_party_for_cookies(const GURL& first_party_for_cookies);

  
  
  
  const std::string& method() const { return method_; }
  void set_method(const std::string& method);

  
  
  
  static std::string ComputeMethodForRedirect(const std::string& method,
                                              int http_status_code);

  
  
  
  
  const std::string& referrer() const { return referrer_; }
  
  void SetReferrer(const std::string& referrer);

  
  
  void set_referrer_policy(ReferrerPolicy referrer_policy);

  
  
  void set_delegate(Delegate* delegate);

  
  
  void EnableChunkedUpload();

  
  
  
  
  
  void AppendChunkToUpload(const char* bytes,
                           int bytes_len,
                           bool is_last_chunk);

  
  void set_upload(scoped_ptr<UploadDataStream> upload);

  
  const UploadDataStream* get_upload() const;

  
  bool has_upload() const;

  
  
  
  void SetExtraRequestHeaderById(int header_id, const std::string& value,
                                 bool overwrite);
  void SetExtraRequestHeaderByName(const std::string& name,
                                   const std::string& value, bool overwrite);
  void RemoveRequestHeaderByName(const std::string& name);

  
  // methods are overwritten by this method.  This method may only be called
  
  void SetExtraRequestHeaders(const HttpRequestHeaders& headers);

  const HttpRequestHeaders& extra_request_headers() const {
    return extra_request_headers_;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetFullRequestHeaders(HttpRequestHeaders* headers) const;

  
  
  
  LoadStateWithParam GetLoadState() const;

  
  
  base::Value* GetStateAsValue() const;

  
  
  
  
  
  void LogBlockedBy(const char* blocked_by);

  
  
  
  
  void LogAndReportBlockedBy(const char* blocked_by);

  
  
  void LogUnblocked();

  
  
  UploadProgress GetUploadProgress() const;

  
  
  
  
  
  
  void GetResponseHeaderById(int header_id, std::string* value);
  void GetResponseHeaderByName(const std::string& name, std::string* value);

  
  
  void GetAllResponseHeaders(std::string* headers);

  
  base::TimeTicks creation_time() const { return creation_time_; }

  
  
  const base::Time& request_time() const {
    return response_info_.request_time;
  }

  
  
  const base::Time& response_time() const {
    return response_info_.response_time;
  }

  
  bool was_cached() const { return response_info_.was_cached; }

  
  bool was_fetched_via_proxy() const {
    return response_info_.was_fetched_via_proxy;
  }

  
  
  HostPortPair GetSocketAddress() const;

  
  
  HttpResponseHeaders* response_headers() const;

  
  const SSLInfo& ssl_info() const {
    return response_info_.ssl_info;
  }

  
  
  
  
  
  
  void GetLoadTimingInfo(LoadTimingInfo* load_timing_info) const;

  
  
  
  
  bool GetResponseCookies(ResponseCookies* cookies);

  
  
  void GetMimeType(std::string* mime_type);

  
  
  void GetCharset(std::string* charset);

  
  
  
  int GetResponseCode() const;

  
  const HttpResponseInfo& response_info() const { return response_info_; }

  
  int load_flags() const { return load_flags_; }

  
  
  
  void SetLoadFlags(int flags);

  
  
  bool is_pending() const { return is_pending_; }

  
  
  bool is_redirecting() const { return is_redirecting_; }

  
  const URLRequestStatus& status() const { return status_; }

  
  uint64 identifier() const { return identifier_; }

  
  
  void Start();

  
  
  
  
  
  
  void Cancel();

  
  
  void CancelWithError(int error);

  
  
  
  
  void CancelWithSSLError(int error, const SSLInfo& ssl_info);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Read(IOBuffer* buf, int max_bytes, int* bytes_read);

  
  
  
  
  
  void StopCaching();

  
  
  void FollowDeferredRedirect();

  
  
  
  
  void SetAuth(const AuthCredentials& credentials);
  void CancelAuth();

  
  
  
  void ContinueWithCertificate(X509Certificate* client_cert);

  
  
  
  void ContinueDespiteLastError();

  
  const URLRequestContext* context() const;

  const BoundNetLog& net_log() const { return net_log_; }

  
  int64 GetExpectedContentSize() const;

  
  RequestPriority priority() const { return priority_; }

  
  
  
  void SetPriority(RequestPriority priority);

  
  // due to HSTS. If so, |redirect_url| is rewritten to the new HTTPS URL.
  bool GetHSTSRedirect(GURL* redirect_url) const;

  
  bool has_delegate() const { return delegate_ != NULL; }

  
  
  
  void set_stack_trace(const base::debug::StackTrace& stack_trace);
  const base::debug::StackTrace* stack_trace() const;

  void set_received_response_content_length(int64 received_content_length) {
    received_response_content_length_ = received_content_length;
  }
  int64 received_response_content_length() {
    return received_response_content_length_;
  }

 protected:
  
  void set_is_pending(bool value) { is_pending_ = value; }

  
  void set_status(const URLRequestStatus& value) { status_ = value; }

  
  
  int Redirect(const GURL& location, int http_status_code);

  
  void NotifyReceivedRedirect(const GURL& location, bool* defer_redirect);

  
  
  void Restart();

 private:
  friend class URLRequestJob;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ProtocolFactory* RegisterProtocolFactory(const std::string& scheme,
                                                  ProtocolFactory* factory);

  
  static void RegisterRequestInterceptor(Interceptor* interceptor);
  static void UnregisterRequestInterceptor(Interceptor* interceptor);

  
  
  
  
  
  void BeforeRequestComplete(int error);

  void StartJob(URLRequestJob* job);

  
  
  void RestartWithJob(URLRequestJob* job);
  void PrepareToRestart();

  
  
  
  void OrphanJob();

  
  
  void DoCancel(int error, const SSLInfo& ssl_info);

  
  
  void OnHeadersComplete();

  
  
  
  void NotifyRequestCompleted();

  
  
  void NotifyResponseStarted();

  
  
  
  void NotifyAuthRequired(AuthChallengeInfo* auth_info);
  void NotifyAuthRequiredComplete(NetworkDelegate::AuthRequiredResponse result);
  void NotifyCertificateRequested(SSLCertRequestInfo* cert_request_info);
  void NotifySSLCertificateError(const SSLInfo& ssl_info, bool fatal);
  void NotifyReadCompleted(int bytes_read);

  
  
  
  bool CanGetCookies(const CookieList& cookie_list) const;
  bool CanSetCookie(const std::string& cookie_line,
                    CookieOptions* options) const;
  bool CanEnablePrivacyMode() const;

  
  void OnCallToDelegate();
  
  
  void OnCallToDelegateComplete();

  
  
  
  const URLRequestContext* context_;

  NetworkDelegate* network_delegate_;

  
  BoundNetLog net_log_;

  scoped_refptr<URLRequestJob> job_;
  scoped_ptr<UploadDataStream> upload_data_stream_;
  std::vector<GURL> url_chain_;
  GURL first_party_for_cookies_;
  GURL delegate_redirect_url_;
  std::string method_;  
  std::string referrer_;
  ReferrerPolicy referrer_policy_;
  HttpRequestHeaders extra_request_headers_;
  int load_flags_;  
                    

  
  
  Delegate* delegate_;

  
  
  
  URLRequestStatus status_;

  
  HttpResponseInfo response_info_;

  
  
  
  bool is_pending_;

  
  
  
  bool is_redirecting_;

  
  
  int redirect_limit_;

  
  
  UploadProgress final_upload_progress_;

  
  
  
  RequestPriority priority_;

  
  
  
  
  
  
  
  const uint64 identifier_;

  
  
  bool calling_delegate_;

  
  
  std::string blocked_by_;
  bool use_blocked_by_as_load_param_;

  base::debug::LeakTracker<URLRequest> leak_tracker_;

  
  
  CompletionCallback before_request_callback_;

  
  
  
  bool has_notified_completion_;

  
  
  
  
  AuthCredentials auth_credentials_;
  scoped_refptr<AuthChallengeInfo> auth_info_;

  int64 received_response_content_length_;

  base::TimeTicks creation_time_;

  
  
  LoadTimingInfo load_timing_info_;

  scoped_ptr<const base::debug::StackTrace> stack_trace_;

  DISALLOW_COPY_AND_ASSIGN(URLRequest);
};

}  

#endif  
