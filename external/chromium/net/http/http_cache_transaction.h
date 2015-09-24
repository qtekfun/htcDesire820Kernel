// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2012 The Linux Foundation. All rights reserved.

// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_HTTP_CACHE_TRANSACTION_H_
#define NET_HTTP_HTTP_CACHE_TRANSACTION_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "base/time.h"
#include "net/base/net_log.h"
#include "net/http/http_cache.h"
#include "net/http/http_response_info.h"
#include "net/http/http_transaction.h"

namespace net {

class HttpResponseHeaders;
class PartialData;
struct HttpRequestInfo;

class HttpCache::Transaction : public HttpTransaction {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum Mode {
    NONE            = 0,
    READ_META       = 1 << 0,
    READ_DATA       = 1 << 1,
    READ            = READ_META | READ_DATA,
    WRITE           = 1 << 2,
    READ_WRITE      = READ | WRITE,
    UPDATE          = READ_META | WRITE,  
  };

  Transaction(HttpCache* cache);
  virtual ~Transaction();

  Mode mode() const { return mode_; }

  const std::string& key() const { return cache_key_; }

  
  
  // Returns the number of bytes actually written, or a net error code. If the
  
  
  
  
  
  // meta-data will be overwritten with the provided data, and subsequent
  
  
  
  
  
  
  int WriteMetadata(IOBuffer* buf, int buf_len, CompletionCallback* callback);

  
  
  
  bool AddTruncatedFlag();

  
  
  
  
  LoadState GetWriterLoadState() const;

  CompletionCallback* io_callback() { return &io_callback_; }

  const BoundNetLog& net_log() const;

  
  virtual int Start(const HttpRequestInfo*, CompletionCallback*,
                    const BoundNetLog&);
  virtual int RestartIgnoringLastError(CompletionCallback* callback);
  virtual int RestartWithCertificate(X509Certificate* client_cert,
                                     CompletionCallback* callback);
  virtual int RestartWithAuth(const string16& username,
                              const string16& password,
                              CompletionCallback* callback);
  virtual bool IsReadyToRestartForAuth();
  virtual int Read(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual void StopCaching();
  virtual const HttpResponseInfo* GetResponseInfo() const;
  virtual LoadState GetLoadState() const;
  virtual uint64 GetUploadProgress(void) const;

 private:
  static const size_t kNumValidationHeaders = 2;
  
  
  struct ValidationHeaders {
    ValidationHeaders() : initialized(false) {}

    std::string values[kNumValidationHeaders];
    bool initialized;
  };

  enum State {
    STATE_NONE,
    STATE_GET_BACKEND,
    STATE_GET_BACKEND_COMPLETE,
    STATE_SEND_REQUEST,
    STATE_SEND_REQUEST_COMPLETE,
    STATE_SUCCESSFUL_SEND_REQUEST,
    STATE_NETWORK_READ,
    STATE_NETWORK_READ_COMPLETE,
    STATE_INIT_ENTRY,
    STATE_OPEN_ENTRY,
    STATE_OPEN_ENTRY_COMPLETE,
    STATE_CREATE_ENTRY,
    STATE_CREATE_ENTRY_COMPLETE,
    STATE_DOOM_ENTRY,
    STATE_DOOM_ENTRY_COMPLETE,
    STATE_ADD_TO_ENTRY,
    STATE_ADD_TO_ENTRY_COMPLETE,
    STATE_NOTIFY_BEFORE_SEND_HEADERS,
    STATE_NOTIFY_BEFORE_SEND_HEADERS_COMPLETE,
    STATE_START_PARTIAL_CACHE_VALIDATION,
    STATE_COMPLETE_PARTIAL_CACHE_VALIDATION,
    STATE_UPDATE_CACHED_RESPONSE,
    STATE_UPDATE_CACHED_RESPONSE_COMPLETE,
    STATE_OVERWRITE_CACHED_RESPONSE,
    STATE_TRUNCATE_CACHED_DATA,
    STATE_TRUNCATE_CACHED_DATA_COMPLETE,
    STATE_TRUNCATE_CACHED_METADATA,
    STATE_TRUNCATE_CACHED_METADATA_COMPLETE,
    STATE_PARTIAL_HEADERS_RECEIVED,
    STATE_CACHE_READ_RESPONSE,
    STATE_CACHE_READ_RESPONSE_COMPLETE,
    STATE_CACHE_WRITE_RESPONSE,
    STATE_CACHE_WRITE_TRUNCATED_RESPONSE,
    STATE_CACHE_WRITE_RESPONSE_COMPLETE,
    STATE_CACHE_READ_METADATA,
    STATE_CACHE_READ_METADATA_COMPLETE,
    STATE_CACHE_QUERY_DATA,
    STATE_CACHE_QUERY_DATA_COMPLETE,
    STATE_CACHE_READ_DATA,
    STATE_CACHE_READ_DATA_COMPLETE,
    STATE_CACHE_WRITE_DATA,
    STATE_CACHE_WRITE_DATA_COMPLETE
  };

  
  
  void DoCallback(int rv);

  
  int HandleResult(int rv);

  
  int DoLoop(int result);

  
  
  
  int DoGetBackend();
  int DoGetBackendComplete(int result);
  int DoSendRequest();
  int DoSendRequestComplete(int result);
  int DoSuccessfulSendRequest();
  int DoNetworkRead();
  int DoNetworkReadComplete(int result);
  int DoInitEntry();
  int DoOpenEntry();
  int DoOpenEntryComplete(int result);
  int DoCreateEntry();
  int DoCreateEntryComplete(int result);
  int DoDoomEntry();
  int DoDoomEntryComplete(int result);
  int DoAddToEntry();
  int DoAddToEntryComplete(int result);
  int DoNotifyBeforeSendHeaders();
  int DoNotifyBeforeSendHeadersComplete(int result);
  int DoStartPartialCacheValidation();
  int DoCompletePartialCacheValidation(int result);
  int DoUpdateCachedResponse();
  int DoUpdateCachedResponseComplete(int result);
  int DoOverwriteCachedResponse();
  int DoTruncateCachedData();
  int DoTruncateCachedDataComplete(int result);
  int DoTruncateCachedMetadata();
  int DoTruncateCachedMetadataComplete(int result);
  int DoPartialHeadersReceived();
  int DoCacheReadResponse();
  int DoCacheReadResponseComplete(int result);
  int DoCacheWriteResponse();
  int DoCacheWriteTruncatedResponse();
  int DoCacheWriteResponseComplete(int result);
  int DoCacheReadMetadata();
  int DoCacheReadMetadataComplete(int result);
  int DoCacheQueryData();
  int DoCacheQueryDataComplete(int result);
  int DoCacheReadData();
  int DoCacheReadDataComplete(int result);
  int DoCacheWriteData(int num_bytes);
  int DoCacheWriteDataComplete(int result);

  
  void SetRequest(const BoundNetLog& net_log, const HttpRequestInfo* request);

  
  
  bool ShouldPassThrough();

  
  int BeginCacheRead();

  
  int BeginCacheValidation();

  
  
  int BeginPartialCacheValidation();

  
  
  int ValidateEntryHeadersAndContinue(bool byte_range_requested);

  
  
  
  
  int BeginExternallyConditionalizedRequest();

  
  
  int RestartNetworkRequest();

  
  
  int RestartNetworkRequestWithCertificate(X509Certificate* client_cert);

  
  
  int RestartNetworkRequestWithAuth(const string16& username,
                                    const string16& password);

  
  bool RequiresValidation();

  
  
  bool ConditionalizeRequest();

  
  
  
  bool ValidatePartialResponse(bool* partial_content);

  
  void IgnoreRangeRequest();

  
  
  void FailRangeRequest();

  
  int ReadFromNetwork(IOBuffer* data, int data_len);

  
  int ReadFromEntry(IOBuffer* data, int data_len);

  
  
  
  int WriteToEntry(int index, int offset, IOBuffer* data, int data_len,
                   CompletionCallback* callback);

  
  
  int WriteResponseInfoToEntry(bool truncated);

  
  
  int AppendResponseDataToEntry(IOBuffer* data, int data_len,
                                CompletionCallback* callback);

  
  void DoneWritingToEntry(bool success);

  
  
  void DoomPartialEntry(bool delete_object);

  
  
  int DoPartialNetworkReadCompleted(int result);

  
  
  int DoPartialCacheReadCompleted(int result);

  
  
  
  bool CanResume(bool has_data);

  
  void OnIOComplete(int result);

  State next_state_;
  const HttpRequestInfo* request_;
  BoundNetLog net_log_;
  scoped_ptr<HttpRequestInfo> custom_request_;
  
  
  ValidationHeaders external_validation_;
  base::WeakPtr<HttpCache> cache_;
  HttpCache::ActiveEntry* entry_;
  base::TimeTicks entry_lock_waiting_since_;
  HttpCache::ActiveEntry* new_entry_;
  scoped_ptr<HttpTransaction> network_trans_;
  CompletionCallback* callback_;  
  HttpResponseInfo response_;
  HttpResponseInfo auth_response_;
  const HttpResponseInfo* new_response_;
  std::string cache_key_;
  Mode mode_;
  State target_state_;
  bool reading_;  
  bool invalid_range_;  
  bool truncated_;  
  bool is_sparse_;  
  bool server_responded_206_;
  bool cache_pending_;  
  scoped_refptr<IOBuffer> read_buf_;
  int io_buf_len_;
  int read_offset_;
  int effective_load_flags_;
  int write_len_;
  scoped_ptr<PartialData> partial_;  
  uint64 final_upload_progress_;
  CompletionCallbackImpl<Transaction> io_callback_;
  scoped_refptr<CancelableCompletionCallback<Transaction> > cache_callback_;
  scoped_refptr<CancelableCompletionCallback<Transaction> >
      write_headers_callback_;

  bool report_to_stathub_;
};

}  

#endif  
