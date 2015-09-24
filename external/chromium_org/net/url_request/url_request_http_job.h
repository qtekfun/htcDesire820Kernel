// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_HTTP_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_HTTP_JOB_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "net/base/auth.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/cookies/cookie_store.h"
#include "net/http/http_request_info.h"
#include "net/url_request/url_request_job.h"
#include "net/url_request/url_request_throttler_entry_interface.h"

namespace net {

class HttpResponseHeaders;
class HttpResponseInfo;
class HttpTransaction;
class HttpUserAgentSettings;
class UploadDataStream;
class URLRequestContext;

class NET_EXPORT_PRIVATE URLRequestHttpJob : public URLRequestJob {
 public:
  static URLRequestJob* Factory(URLRequest* request,
                                NetworkDelegate* network_delegate,
                                const std::string& scheme);

 protected:
  URLRequestHttpJob(URLRequest* request,
                    NetworkDelegate* network_delegate,
                    const HttpUserAgentSettings* http_user_agent_settings);

  virtual ~URLRequestHttpJob();

  
  virtual void SetPriority(RequestPriority priority) OVERRIDE;
  virtual void Start() OVERRIDE;
  virtual void Kill() OVERRIDE;

  RequestPriority priority() const {
    return priority_;
  }

 private:
  enum CompletionCause {
    ABORTED,
    FINISHED
  };

  typedef base::RefCountedData<bool> SharedBoolean;

  class HttpFilterContext;
  class HttpTransactionDelegateImpl;

  
  void NotifyHeadersComplete();

  
  void NotifyDone(const URLRequestStatus& status);

  void DestroyTransaction();

  void AddExtraHeaders();
  void AddCookieHeaderAndStart();
  void SaveCookiesAndNotifyHeadersComplete(int result);
  void SaveNextCookie();
  void FetchResponseCookies(std::vector<std::string>* cookies);

  
  void ProcessStrictTransportSecurityHeader();

  
  void ProcessPublicKeyPinsHeader();

  
  void OnHeadersReceivedCallback(int result);
  void OnStartCompleted(int result);
  void OnReadCompleted(int result);
  void NotifyBeforeSendHeadersCallback(int result);

  void RestartTransactionWithAuth(const AuthCredentials& credentials);

  
  virtual void SetUpload(UploadDataStream* upload) OVERRIDE;
  virtual void SetExtraRequestHeaders(
      const HttpRequestHeaders& headers) OVERRIDE;
  virtual LoadState GetLoadState() const OVERRIDE;
  virtual UploadProgress GetUploadProgress() const OVERRIDE;
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual bool GetCharset(std::string* charset) OVERRIDE;
  virtual void GetResponseInfo(HttpResponseInfo* info) OVERRIDE;
  virtual void GetLoadTimingInfo(
      LoadTimingInfo* load_timing_info) const OVERRIDE;
  virtual bool GetResponseCookies(std::vector<std::string>* cookies) OVERRIDE;
  virtual int GetResponseCode() const OVERRIDE;
  virtual Filter* SetupFilter() const OVERRIDE;
  virtual bool IsSafeRedirect(const GURL& location) OVERRIDE;
  virtual bool NeedsAuth() OVERRIDE;
  virtual void GetAuthChallengeInfo(scoped_refptr<AuthChallengeInfo>*) OVERRIDE;
  virtual void SetAuth(const AuthCredentials& credentials) OVERRIDE;
  virtual void CancelAuth() OVERRIDE;
  virtual void ContinueWithCertificate(X509Certificate* client_cert) OVERRIDE;
  virtual void ContinueDespiteLastError() OVERRIDE;
  virtual bool ReadRawData(IOBuffer* buf, int buf_size,
                           int* bytes_read) OVERRIDE;
  virtual void StopCaching() OVERRIDE;
  virtual bool GetFullRequestHeaders(
      HttpRequestHeaders* headers) const OVERRIDE;
  virtual void DoneReading() OVERRIDE;
  virtual HostPortPair GetSocketAddress() const OVERRIDE;
  virtual void NotifyURLRequestDestroyed() OVERRIDE;

  void RecordTimer();
  void ResetTimer();

  virtual void UpdatePacketReadTimes() OVERRIDE;
  void RecordPacketStats(FilterContext::StatisticSelector statistic) const;

  void RecordCompressionHistograms();
  bool IsCompressibleContent() const;

  
  
  void StartTransaction();
  
  
  void MaybeStartTransactionInternal(int result);
  void StartTransactionInternal();

  void RecordPerfHistograms(CompletionCause reason);
  void DoneWithRequest(CompletionCause reason);

  
  void DoLoadCookies();
  void CheckCookiePolicyAndLoad(const CookieList& cookie_list);
  void OnCookiesLoaded(const std::string& cookie_line);
  void DoStartTransaction();

  
  
  void OnCookieSaved(scoped_refptr<SharedBoolean> save_next_cookie_running,
                     scoped_refptr<SharedBoolean> callback_pending,
                     bool cookie_status);

  
  
  
  
  
  bool ShouldFixMismatchedContentLength(int rv) const;

  
  
  HttpResponseHeaders* GetResponseHeaders() const;

  
  virtual void OnDetachRequest() OVERRIDE;

  RequestPriority priority_;

  HttpRequestInfo request_info_;
  const HttpResponseInfo* response_info_;

  std::vector<std::string> response_cookies_;
  size_t response_cookies_save_index_;
  base::Time response_date_;

  
  AuthState proxy_auth_state_;
  AuthState server_auth_state_;
  AuthCredentials auth_credentials_;

  CompletionCallback start_callback_;
  CompletionCallback notify_before_headers_sent_callback_;

  bool read_in_progress_;

  
  GURL sdch_dictionary_url_;

  scoped_ptr<HttpTransaction> transaction_;

  
  
  scoped_refptr<URLRequestThrottlerEntryInterface> throttling_entry_;

  
  
  
  
  bool sdch_dictionary_advertised_;

  
  
  
  bool sdch_test_activated_;  
  bool sdch_test_control_;    

  
  bool is_cached_content_;

  base::Time request_creation_time_;

  
  
  
  
  
  
  
  

  
  bool packet_timing_enabled_;
  bool done_;  

  
  
  int64 bytes_observed_in_packets_;

  
  
  base::Time request_time_snapshot_;

  
  
  base::Time final_packet_time_;

  
  base::TimeTicks start_time_;

  
  base::TimeTicks receive_headers_end_;

  scoped_ptr<HttpFilterContext> filter_context_;
  base::WeakPtrFactory<URLRequestHttpJob> weak_factory_;

  CompletionCallback on_headers_received_callback_;

  
  
  
  scoped_refptr<HttpResponseHeaders> override_response_headers_;

  
  
  
  
  
  bool awaiting_callback_;

  scoped_ptr<HttpTransactionDelegateImpl> http_transaction_delegate_;

  const HttpUserAgentSettings* http_user_agent_settings_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestHttpJob);
};

}  

#endif  
