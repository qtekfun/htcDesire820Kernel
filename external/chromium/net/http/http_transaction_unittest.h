// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_TRANSACTION_UNITTEST_H_
#define NET_HTTP_HTTP_TRANSACTION_UNITTEST_H_
#pragma once

#include "net/http/http_transaction.h"

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "net/base/io_buffer.h"
#include "net/base/load_flags.h"
#include "net/base/net_errors.h"
#include "net/base/test_completion_callback.h"
#include "net/disk_cache/disk_cache.h"
#include "net/http/http_cache.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_response_info.h"

namespace net {
class IOBuffer;
}


enum {
  TEST_MODE_NORMAL = 0,
  TEST_MODE_SYNC_NET_START = 1 << 0,
  TEST_MODE_SYNC_NET_READ  = 1 << 1,
  TEST_MODE_SYNC_CACHE_START = 1 << 2,
  TEST_MODE_SYNC_CACHE_READ  = 1 << 3,
  TEST_MODE_SYNC_CACHE_WRITE  = 1 << 4,
  TEST_MODE_SYNC_ALL = (TEST_MODE_SYNC_NET_START | TEST_MODE_SYNC_NET_READ |
                        TEST_MODE_SYNC_CACHE_START | TEST_MODE_SYNC_CACHE_READ |
                        TEST_MODE_SYNC_CACHE_WRITE)
};

typedef void (*MockTransactionHandler)(const net::HttpRequestInfo* request,
                                       std::string* response_status,
                                       std::string* response_headers,
                                       std::string* response_data);

struct MockTransaction {
  const char* url;
  const char* method;
  
  base::Time request_time;
  const char* request_headers;
  int load_flags;
  const char* status;
  const char* response_headers;
  
  base::Time response_time;
  const char* data;
  int test_mode;
  MockTransactionHandler handler;
  int cert_status;
};

extern const MockTransaction kSimpleGET_Transaction;
extern const MockTransaction kSimplePOST_Transaction;
extern const MockTransaction kTypicalGET_Transaction;
extern const MockTransaction kETagGET_Transaction;
extern const MockTransaction kRangeGET_Transaction;

const MockTransaction* FindMockTransaction(const GURL& url);

void AddMockTransaction(const MockTransaction* trans);
void RemoveMockTransaction(const MockTransaction* trans);

struct ScopedMockTransaction : MockTransaction {
  ScopedMockTransaction() {
    AddMockTransaction(this);
  }
  explicit ScopedMockTransaction(const MockTransaction& t)
      : MockTransaction(t) {
    AddMockTransaction(this);
  }
  ~ScopedMockTransaction() {
    RemoveMockTransaction(this);
  }
};


class MockHttpRequest : public net::HttpRequestInfo {
 public:
  explicit MockHttpRequest(const MockTransaction& t);
};


class TestTransactionConsumer : public CallbackRunner< Tuple1<int> > {
 public:
  explicit TestTransactionConsumer(net::HttpTransactionFactory* factory);
  virtual ~TestTransactionConsumer();

  void Start(const net::HttpRequestInfo* request,
             const net::BoundNetLog& net_log);

  bool is_done() const { return state_ == DONE; }
  int error() const { return error_; }

  const net::HttpResponseInfo* response_info() const {
    return trans_->GetResponseInfo();
  }
  const std::string& content() const { return content_; }

 private:
  enum State {
    IDLE,
    STARTING,
    READING,
    DONE
  };

  void DidStart(int result);
  void DidRead(int result);
  void DidFinish(int result);
  void Read();

  
  virtual void RunWithParams(const Tuple1<int>& params);

  State state_;
  scoped_ptr<net::HttpTransaction> trans_;
  std::string content_;
  scoped_refptr<net::IOBuffer> read_buf_;
  int error_;

  static int quit_counter_;
};


class MockNetworkTransaction : public net::HttpTransaction {
 public:
  MockNetworkTransaction();
  virtual ~MockNetworkTransaction();

  virtual int Start(const net::HttpRequestInfo* request,
                    net::CompletionCallback* callback,
                    const net::BoundNetLog& net_log);

  virtual int RestartIgnoringLastError(net::CompletionCallback* callback);

  virtual int RestartWithCertificate(net::X509Certificate* client_cert,
                                     net::CompletionCallback* callback);

  virtual int RestartWithAuth(const string16& username,
                              const string16& password,
                              net::CompletionCallback* callback);

  virtual bool IsReadyToRestartForAuth();

  virtual int Read(net::IOBuffer* buf, int buf_len,
                   net::CompletionCallback* callback);

  virtual void StopCaching();

  virtual const net::HttpResponseInfo* GetResponseInfo() const;

  virtual net::LoadState GetLoadState() const;

  virtual uint64 GetUploadProgress() const;

 private:
  void CallbackLater(net::CompletionCallback* callback, int result);
  void RunCallback(net::CompletionCallback* callback, int result);

  ScopedRunnableMethodFactory<MockNetworkTransaction> task_factory_;
  net::HttpResponseInfo response_;
  std::string data_;
  int data_cursor_;
  int test_mode_;
};

class MockNetworkLayer : public net::HttpTransactionFactory {
 public:
  MockNetworkLayer();
  virtual ~MockNetworkLayer();

  int transaction_count() const { return transaction_count_; }

  
  virtual int CreateTransaction(scoped_ptr<net::HttpTransaction>* trans);
  virtual net::HttpCache* GetCache();
  virtual net::HttpNetworkSession* GetSession();
  virtual void Suspend(bool suspend);

 private:
  int transaction_count_;
};


int ReadTransaction(net::HttpTransaction* trans, std::string* result);

#endif  
