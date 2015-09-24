// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_JOB_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/power_monitor/power_observer.h"
#include "net/base/filter.h"
#include "net/base/host_port_pair.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/base/upload_progress.h"
#include "net/cookies/canonical_cookie.h"
#include "url/gurl.h"

namespace net {

class AuthChallengeInfo;
class AuthCredentials;
class CookieOptions;
class HttpRequestHeaders;
class HttpResponseInfo;
class IOBuffer;
struct LoadTimingInfo;
class NetworkDelegate;
class SSLCertRequestInfo;
class SSLInfo;
class URLRequest;
class UploadDataStream;
class URLRequestStatus;
class X509Certificate;

class NET_EXPORT URLRequestJob
    : public base::RefCounted<URLRequestJob>,
      public base::PowerObserver {
 public:
  explicit URLRequestJob(URLRequest* request,
                         NetworkDelegate* network_delegate);

  
  
  URLRequest* request() const {
    return request_;
  }

  
  
  virtual void SetUpload(UploadDataStream* upload_data_stream);

  
  
  virtual void SetExtraRequestHeaders(const HttpRequestHeaders& headers);

  
  
  virtual void SetPriority(RequestPriority priority);

  
  
  
  
  virtual void Start() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Kill();

  
  
  void DetachRequest();

  
  
  
  
  bool Read(IOBuffer* buf, int buf_size, int* bytes_read);

  
  
  virtual void StopCaching();

  virtual bool GetFullRequestHeaders(HttpRequestHeaders* headers) const;

  
  virtual LoadState GetLoadState() const;

  
  virtual UploadProgress GetUploadProgress() const;

  
  
  
  virtual bool GetCharset(std::string* charset);

  
  virtual void GetResponseInfo(HttpResponseInfo* info);

  
  
  
  virtual void GetLoadTimingInfo(LoadTimingInfo* load_timing_info) const;

  
  
  
  
  virtual bool GetResponseCookies(std::vector<std::string>* cookies);

  
  
  
  
  
  
  virtual Filter* SetupFilter() const;

  
  
  
  
  
  
  
  
  
  
  
  virtual bool IsRedirectResponse(GURL* location, int* http_status_code);

  
  
  
  
  virtual bool IsSafeRedirect(const GURL& location);

  
  
  
  virtual bool NeedsAuth();

  
  virtual void GetAuthChallengeInfo(
      scoped_refptr<AuthChallengeInfo>* auth_info);

  
  virtual void SetAuth(const AuthCredentials& credentials);

  
  virtual void CancelAuth();

  virtual void ContinueWithCertificate(X509Certificate* client_cert);

  
  virtual void ContinueDespiteLastError();

  void FollowDeferredRedirect();

  
  
  bool is_done() const { return done_; }

  
  int64 expected_content_size() const { return expected_content_size_; }
  void set_expected_content_size(const int64& size) {
    expected_content_size_ = size;
  }

  
  bool has_response_started() const { return has_handled_response_; }

  
  virtual bool GetMimeType(std::string* mime_type) const;
  virtual int GetResponseCode() const;

  
  
  virtual HostPortPair GetSocketAddress() const;

  
  
  virtual void OnSuspend() OVERRIDE;

  
  
  
  
  virtual void NotifyURLRequestDestroyed();

 protected:
  friend class base::RefCounted<URLRequestJob>;
  virtual ~URLRequestJob();

  
  void NotifyCertificateRequested(SSLCertRequestInfo* cert_request_info);

  
  void NotifySSLCertificateError(const SSLInfo& ssl_info, bool fatal);

  
  bool CanGetCookies(const CookieList& cookie_list) const;

  
  bool CanSetCookie(const std::string& cookie_line,
                    CookieOptions* options) const;

  
  bool CanEnablePrivacyMode() const;

  
  void NotifyHeadersComplete();

  
  void NotifyReadComplete(int bytes_read);

  
  void NotifyStartError(const URLRequestStatus& status);

  
  
  
  
  void NotifyDone(const URLRequestStatus& status);

  
  
  
  void CompleteNotifyDone();

  
  
  void NotifyCanceled();

  
  
  void NotifyRestartRequired();

  
  void OnCallToDelegate();
  void OnCallToDelegateComplete();

  
  
  
  
  
  
  
  
  
  
  virtual bool ReadRawData(IOBuffer* buf, int buf_size, int *bytes_read);

  
  
  virtual void DoneReading();

  
  void FilteredDataRead(int bytes_read);

  
  
  
  
  bool ReadFilteredData(int *bytes_read);

  
  
  bool HasFilter() { return filter_ != NULL; }

  
  
  
  
  void DestroyFilters() { filter_.reset(); }

  
  NetworkDelegate* network_delegate() { return network_delegate_; }

  
  const URLRequestStatus GetStatus();

  
  void SetStatus(const URLRequestStatus& status);

  
  int prefilter_bytes_read() const { return prefilter_bytes_read_; }

  
  int postfilter_bytes_read() const { return postfilter_bytes_read_; }

  
  
  
  int64 filter_input_byte_count() const { return filter_input_byte_count_; }

  
  
  URLRequest* request_;

 private:
  
  
  
  bool ReadRawDataForFilter(int *bytes_read);

  
  
  bool ReadRawDataHelper(IOBuffer* buf, int buf_size, int* bytes_read);

  
  
  
  void FollowRedirect(const GURL& location, int http_status_code);

  
  
  
  
  void OnRawReadComplete(int bytes_read);

  
  
  void RecordBytesRead(int bytes_read);

  
  
  bool FilterHasData();

  
  
  virtual void UpdatePacketReadTimes();

  
  virtual void OnDetachRequest() {}

  
  
  
  bool done_;

  int prefilter_bytes_read_;
  int postfilter_bytes_read_;
  int64 filter_input_byte_count_;

  
  scoped_ptr<Filter> filter_;

  
  
  bool filter_needs_more_output_space_;

  
  
  
  
  scoped_refptr<IOBuffer> filtered_read_buffer_;
  int filtered_read_buffer_len_;

  
  
  scoped_refptr<IOBuffer> raw_read_buffer_;

  
  
  bool has_handled_response_;

  
  int64 expected_content_size_;

  
  GURL deferred_redirect_url_;
  int deferred_redirect_status_code_;

  
  NetworkDelegate* network_delegate_;

  base::WeakPtrFactory<URLRequestJob> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestJob);
};

}  

#endif  
