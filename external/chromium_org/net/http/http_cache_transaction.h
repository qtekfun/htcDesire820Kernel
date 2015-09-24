// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_HTTP_CACHE_TRANSACTION_H_
#define NET_HTTP_HTTP_CACHE_TRANSACTION_H_

#include <string>

#include "base/time/time.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_cache.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_info.h"
#include "net/http/http_transaction.h"

namespace net {

class PartialData;
struct HttpRequestInfo;
class HttpTransactionDelegate;
struct LoadTimingInfo;

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

  Transaction(RequestPriority priority,
              HttpCache* cache,
              HttpTransactionDelegate* transaction_delegate);
  virtual ~Transaction();

  Mode mode() const { return mode_; }

  const std::string& key() const { return cache_key_; }

  
  
  // Returns the number of bytes actually written, or a net error code. If the
  
  
  
  
  
  // meta-data will be overwritten with the provided data, and subsequent
  
  
  
  
  
  
  int WriteMetadata(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback);

  
  
  
  
  bool AddTruncatedFlag();

  HttpCache::ActiveEntry* entry() { return entry_; }

  
  
  
  
  LoadState GetWriterLoadState() const;

  const CompletionCallback& io_callback() { return io_callback_; }

  const BoundNetLog& net_log() const;

  
  virtual int Start(const HttpRequestInfo* request_info,
                    const CompletionCallback& callback,
                    const BoundNetLog& net_log) OVERRIDE;
  virtual int RestartIgnoringLastError(
      const CompletionCallback& callback) OVERRIDE;
  virtual int RestartWithCertificate(
      X509Certificate* client_cert,
      const CompletionCallback& callback) OVERRIDE;
  virtual int RestartWithAuth(const AuthCredentials& credentials,
                              const CompletionCallback& callback) OVERRIDE;
  virtual bool IsReadyToRestartForAuth() OVERRIDE;
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual void StopCaching() OVERRIDE;
  virtual bool GetFullRequestHeaders(
      HttpRequestHeaders* headers) const OVERRIDE;
  virtual void DoneReading() OVERRIDE;
  virtual const HttpResponseInfo* GetResponseInfo() const OVERRIDE;
  virtual LoadState GetLoadState() const OVERRIDE;
  virtual UploadProgress GetUploadProgress(void) const OVERRIDE;
  virtual bool GetLoadTimingInfo(
      LoadTimingInfo* load_timing_info) const OVERRIDE;
  virtual void SetPriority(RequestPriority priority) OVERRIDE;
  virtual void SetWebSocketHandshakeStreamCreateHelper(
      net::WebSocketHandshakeStreamBase::CreateHelper* create_helper) OVERRIDE;

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

  
  
  
  
  
  
  enum TransactionPattern {
    PATTERN_UNDEFINED,
    PATTERN_NOT_COVERED,
    PATTERN_ENTRY_NOT_CACHED,
    PATTERN_ENTRY_USED,
    PATTERN_ENTRY_VALIDATED,
    PATTERN_ENTRY_UPDATED,
    PATTERN_ENTRY_CANT_CONDITIONALIZE,
    PATTERN_MAX,
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

  
  
  int ValidateEntryHeadersAndContinue();

  
  
  
  
  int BeginExternallyConditionalizedRequest();

  
  
  int RestartNetworkRequest();

  
  
  int RestartNetworkRequestWithCertificate(X509Certificate* client_cert);

  
  
  int RestartNetworkRequestWithAuth(const AuthCredentials& credentials);

  
  bool RequiresValidation();

  
  
  bool ConditionalizeRequest();

  
  
  
  bool ValidatePartialResponse();

  
  void IgnoreRangeRequest();

  
  
  void FailRangeRequest();

  
  int SetupEntryForRead();

  
  int ReadFromNetwork(IOBuffer* data, int data_len);

  
  int ReadFromEntry(IOBuffer* data, int data_len);

  
  
  
  int WriteToEntry(int index, int offset, IOBuffer* data, int data_len,
                   const CompletionCallback& callback);

  
  
  int WriteResponseInfoToEntry(bool truncated);

  
  
  int AppendResponseDataToEntry(IOBuffer* data, int data_len,
                                const CompletionCallback& callback);

  
  void DoneWritingToEntry(bool success);

  
  
  
  int OnCacheReadError(int result, bool restart);

  
  
  void DoomPartialEntry(bool delete_object);

  
  
  int DoPartialNetworkReadCompleted(int result);

  
  
  int DoPartialCacheReadCompleted(int result);

  
  
  
  int DoRestartPartialRequest();

  
  
  void ResetNetworkTransaction();

  
  
  
  bool CanResume(bool has_data);

  
  void OnIOComplete(int result);

  void ReportCacheActionStart();
  void ReportCacheActionFinish();
  void ReportNetworkActionStart();
  void ReportNetworkActionFinish();
  void UpdateTransactionPattern(TransactionPattern new_transaction_pattern);
  void RecordHistograms();

  State next_state_;
  const HttpRequestInfo* request_;
  RequestPriority priority_;
  BoundNetLog net_log_;
  scoped_ptr<HttpRequestInfo> custom_request_;
  HttpRequestHeaders request_headers_copy_;
  
  
  ValidationHeaders external_validation_;
  base::WeakPtr<HttpCache> cache_;
  HttpCache::ActiveEntry* entry_;
  HttpCache::ActiveEntry* new_entry_;
  scoped_ptr<HttpTransaction> network_trans_;
  CompletionCallback callback_;  
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
  bool range_requested_;  
  bool handling_206_;  
  bool cache_pending_;  
  bool done_reading_;
  bool vary_mismatch_;  
  bool couldnt_conditionalize_request_;
  scoped_refptr<IOBuffer> read_buf_;
  int io_buf_len_;
  int read_offset_;
  int effective_load_flags_;
  int write_len_;
  scoped_ptr<PartialData> partial_;  
  UploadProgress final_upload_progress_;
  base::WeakPtrFactory<Transaction> weak_factory_;
  CompletionCallback io_callback_;

  
  TransactionPattern transaction_pattern_;
  base::TimeTicks entry_lock_waiting_since_;
  base::TimeTicks first_cache_access_since_;
  base::TimeTicks send_request_since_;

  HttpTransactionDelegate* transaction_delegate_;

  
  
  
  scoped_ptr<LoadTimingInfo> old_network_trans_load_timing_;

  
  
  
  
  WebSocketHandshakeStreamBase::CreateHelper*
      websocket_handshake_stream_base_create_helper_;

  DISALLOW_COPY_AND_ASSIGN(Transaction);
};

}  

#endif  
