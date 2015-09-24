// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2012 The Linux Foundation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_JOB_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/time.h"
#include "googleurl/src/gurl.h"
#include "net/base/filter.h"
#include "net/base/host_port_pair.h"
#include "net/base/load_states.h"

namespace net {

class AuthChallengeInfo;
class HttpRequestHeaders;
class HttpResponseInfo;
class IOBuffer;
class URLRequest;
class UploadData;
class URLRequestStatus;
class X509Certificate;

class URLRequestJob : public base::RefCounted<URLRequestJob> {
 public:
  explicit URLRequestJob(URLRequest* request);

  
  
  URLRequest* request() const {
    return request_;
  }

  
  
  virtual void SetUpload(UploadData* upload);

  
  virtual void SetExtraRequestHeaders(const HttpRequestHeaders& headers);

  
  
  
  
  virtual void Start() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Kill();

  
  
  void DetachRequest();

  
  
  
  
  bool Read(IOBuffer* buf, int buf_size, int* bytes_read);

  
  
  virtual void StopCaching();

  
  virtual LoadState GetLoadState() const;

  
  virtual uint64 GetUploadProgress() const;

  
  
  
  virtual bool GetCharset(std::string* charset);

  
  virtual void GetResponseInfo(HttpResponseInfo* info);

  
  
  
  
  virtual bool GetResponseCookies(std::vector<std::string>* cookies);

  
  
  
  
  
  
  virtual Filter* SetupFilter() const;

  
  
  
  
  
  
  
  
  
  
  
  virtual bool IsRedirectResponse(GURL* location, int* http_status_code);

  
  
  
  
  virtual bool IsSafeRedirect(const GURL& location);

  
  
  
  virtual bool NeedsAuth();

  
  virtual void GetAuthChallengeInfo(
      scoped_refptr<AuthChallengeInfo>* auth_info);

  
  virtual void SetAuth(const string16& username,
                       const string16& password);

  
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

 protected:
  friend class base::RefCounted<URLRequestJob>;
  virtual ~URLRequestJob();

  
  void NotifyHeadersComplete();

  
  void NotifyReadComplete(int bytes_read);

  
  void NotifyStartError(const URLRequestStatus& status);

  
  
  
  
virtual void NotifyDone(const URLRequestStatus& status);

  
  
  
  void CompleteNotifyDone();

  
  
  void NotifyCanceled();

  
  
  void NotifyRestartRequired();

  
  
  
  
  
  
  
  
  
  
  virtual bool ReadRawData(IOBuffer* buf, int buf_size, int *bytes_read);

  
  void FilteredDataRead(int bytes_read);

  
  
  
  
  bool ReadFilteredData(int *bytes_read);

  
  
  bool HasFilter() { return filter_ != NULL; }

  
  
  
  
  void DestroyFilters() { filter_.reset(); }

  
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

  ScopedRunnableMethodFactory<URLRequestJob> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestJob);
};

}  

#endif  
