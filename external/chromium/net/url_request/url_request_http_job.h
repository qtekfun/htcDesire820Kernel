// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_HTTP_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_HTTP_JOB_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/time.h"
#include "net/base/auth.h"
#include "net/base/completion_callback.h"
#include "net/http/http_request_info.h"
#include "net/url_request/url_request_job.h"
#include "net/url_request/url_request_throttler_entry_interface.h"

namespace net {

class HttpResponseInfo;
class HttpTransaction;
class URLRequestContext;
class HttpResponseHeaders;

class URLRequestHttpJob : public URLRequestJob {
 public:
  static URLRequestJob* Factory(URLRequest* request,
                                const std::string& scheme);

 protected:
  explicit URLRequestHttpJob(URLRequest* request);

  
  void NotifyHeadersComplete();

  
  void NotifyDone(const URLRequestStatus& status);

  void DestroyTransaction();
  void StartTransaction();
  void AddExtraHeaders();
  void AddCookieHeaderAndStart();
  void SaveCookiesAndNotifyHeadersComplete();
  void SaveNextCookie();
  void FetchResponseCookies(const HttpResponseInfo* response_info,
                            std::vector<std::string>* cookies);

  
  void ProcessStrictTransportSecurityHeader();

  void OnCanGetCookiesCompleted(int result);
  void OnCanSetCookieCompleted(int result);
  void OnStartCompleted(int result);
  void OnReadCompleted(int result);

  bool ShouldTreatAsCertificateError(int result);

  void RestartTransactionWithAuth(const string16& username,
                                  const string16& password);

  
  virtual void SetUpload(UploadData* upload);
  virtual void SetExtraRequestHeaders(const HttpRequestHeaders& headers);
  virtual void Start();
  virtual void Kill();
  virtual LoadState GetLoadState() const;
  virtual uint64 GetUploadProgress() const;
  virtual bool GetMimeType(std::string* mime_type) const;
  virtual bool GetCharset(std::string* charset);
  virtual void GetResponseInfo(HttpResponseInfo* info);
  virtual bool GetResponseCookies(std::vector<std::string>* cookies);
  virtual int GetResponseCode() const;
  virtual Filter* SetupFilter() const;
  virtual bool IsSafeRedirect(const GURL& location);
  virtual bool NeedsAuth();
  virtual void GetAuthChallengeInfo(scoped_refptr<AuthChallengeInfo>*);
  virtual void SetAuth(const string16& username,
                       const string16& password);
  virtual void CancelAuth();
  virtual void ContinueWithCertificate(X509Certificate* client_cert);
  virtual void ContinueDespiteLastError();
  virtual bool ReadRawData(IOBuffer* buf, int buf_size, int *bytes_read);
  virtual void StopCaching();
  virtual HostPortPair GetSocketAddress() const;

  HttpResponseHeaders* GetResponseHeaders() const;

  
  
  scoped_refptr<URLRequestContext> context_;

  HttpRequestInfo request_info_;
  const HttpResponseInfo* response_info_;

  std::vector<std::string> response_cookies_;
  size_t response_cookies_save_index_;

  base::Time response_date_;

  
  AuthState proxy_auth_state_;
  AuthState server_auth_state_;

  string16 username_;
  string16 password_;

  CompletionCallbackImpl<URLRequestHttpJob> start_callback_;
  CompletionCallbackImpl<URLRequestHttpJob> read_callback_;

  bool read_in_progress_;

  
  GURL sdch_dictionary_url_;

  scoped_ptr<HttpTransaction> transaction_;

  
  scoped_refptr<URLRequestThrottlerEntryInterface> throttling_entry_;

  
  
  
  
  bool sdch_dictionary_advertised_;

  
  
  
  bool sdch_test_activated_;  
  bool sdch_test_control_;    

  
  bool is_cached_content_;

 private:
  class HttpFilterContext;

  virtual ~URLRequestHttpJob();

  void RecordTimer();
  void ResetTimer();

  virtual void UpdatePacketReadTimes();
  void RecordPacketStats(FilterContext::StatisticSelector statistic) const;

  void RecordCompressionHistograms();
  bool IsCompressibleContent() const;

  base::Time request_creation_time_;

  
  
  
  
  
  
  
  

  
  bool packet_timing_enabled_;

  
  
  int64 bytes_observed_in_packets_;

  
  std::vector<base::Time> packet_times_;

  
  
  base::Time request_time_snapshot_;

  
  
  base::Time final_packet_time_;

  
  
  int observed_packet_count_;

  scoped_ptr<HttpFilterContext> filter_context_;
  ScopedRunnableMethodFactory<URLRequestHttpJob> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestHttpJob);
};

}  

#endif  
