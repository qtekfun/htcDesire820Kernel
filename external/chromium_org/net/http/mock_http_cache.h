// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_MOCK_HTTP_CACHE_H_
#define NET_HTTP_MOCK_HTTP_CACHE_H_

#include "base/containers/hash_tables.h"
#include "net/disk_cache/disk_cache.h"
#include "net/http/http_cache.h"
#include "net/http/http_transaction_unittest.h"


class MockDiskEntry : public disk_cache::Entry,
                      public base::RefCounted<MockDiskEntry> {
 public:
  explicit MockDiskEntry(const std::string& key);

  bool is_doomed() const { return doomed_; }

  virtual void Doom() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual std::string GetKey() const OVERRIDE;
  virtual base::Time GetLastUsed() const OVERRIDE;
  virtual base::Time GetLastModified() const OVERRIDE;
  virtual int32 GetDataSize(int index) const OVERRIDE;
  virtual int ReadData(int index, int offset, net::IOBuffer* buf, int buf_len,
                       const net::CompletionCallback& callback) OVERRIDE;
  virtual int WriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
                        const net::CompletionCallback& callback,
                        bool truncate) OVERRIDE;
  virtual int ReadSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                             const net::CompletionCallback& callback) OVERRIDE;
  virtual int WriteSparseData(
      int64 offset, net::IOBuffer* buf, int buf_len,
      const net::CompletionCallback& callback) OVERRIDE;
  virtual int GetAvailableRange(
      int64 offset, int len, int64* start,
      const net::CompletionCallback& callback) OVERRIDE;
  virtual bool CouldBeSparse() const OVERRIDE;
  virtual void CancelSparseIO() OVERRIDE;
  virtual int ReadyForSparseIO(
      const net::CompletionCallback& completion_callback) OVERRIDE;

  
  void set_fail_requests() { fail_requests_ = true; }

  void set_fail_sparse_requests() { fail_sparse_requests_ = true; }

  
  
  
  static void IgnoreCallbacks(bool value);

 private:
  friend class base::RefCounted<MockDiskEntry>;
  struct CallbackInfo;

  virtual ~MockDiskEntry();

  
  
  
  void CallbackLater(const net::CompletionCallback& callback, int result);

  void RunCallback(const net::CompletionCallback& callback, int result);

  
  
  static void StoreAndDeliverCallbacks(bool store, MockDiskEntry* entry,
                                       const net::CompletionCallback& callback,
                                       int result);

  static const int kNumCacheEntryDataIndices = 3;

  std::string key_;
  std::vector<char> data_[kNumCacheEntryDataIndices];
  int test_mode_;
  bool doomed_;
  bool sparse_;
  bool fail_requests_;
  bool fail_sparse_requests_;
  bool busy_;
  bool delayed_;
  static bool cancel_;
  static bool ignore_callbacks_;
};

class MockDiskCache : public disk_cache::Backend {
 public:
  MockDiskCache();
  virtual ~MockDiskCache();

  virtual net::CacheType GetCacheType() const OVERRIDE;
  virtual int32 GetEntryCount() const OVERRIDE;
  virtual int OpenEntry(const std::string& key, disk_cache::Entry** entry,
                        const net::CompletionCallback& callback) OVERRIDE;
  virtual int CreateEntry(const std::string& key, disk_cache::Entry** entry,
                          const net::CompletionCallback& callback) OVERRIDE;
  virtual int DoomEntry(const std::string& key,
                        const net::CompletionCallback& callback) OVERRIDE;
  virtual int DoomAllEntries(const net::CompletionCallback& callback) OVERRIDE;
  virtual int DoomEntriesBetween(
      base::Time initial_time,
      base::Time end_time,
      const net::CompletionCallback& callback) OVERRIDE;
  virtual int DoomEntriesSince(
      base::Time initial_time,
      const net::CompletionCallback& callback) OVERRIDE;
  virtual int OpenNextEntry(void** iter, disk_cache::Entry** next_entry,
                            const net::CompletionCallback& callback) OVERRIDE;
  virtual void EndEnumeration(void** iter) OVERRIDE;
  virtual void GetStats(
      std::vector<std::pair<std::string, std::string> >* stats) OVERRIDE;
  virtual void OnExternalCacheHit(const std::string& key) OVERRIDE;

  
  int open_count() const { return open_count_; }

  
  int create_count() const { return create_count_; }

  
  void set_fail_requests() { fail_requests_ = true; }

  
  void set_soft_failures(bool value) { soft_failures_ = value; }

  
  void set_double_create_check(bool value) { double_create_check_ = value; }

  
  void set_fail_sparse_requests() { fail_sparse_requests_ = true; }

  void ReleaseAll();

 private:
  typedef base::hash_map<std::string, MockDiskEntry*> EntryMap;

  void CallbackLater(const net::CompletionCallback& callback, int result);

  EntryMap entries_;
  int open_count_;
  int create_count_;
  bool fail_requests_;
  bool soft_failures_;
  bool double_create_check_;
  bool fail_sparse_requests_;
};

class MockBackendFactory : public net::HttpCache::BackendFactory {
 public:
  virtual int CreateBackend(net::NetLog* net_log,
                            scoped_ptr<disk_cache::Backend>* backend,
                            const net::CompletionCallback& callback) OVERRIDE;
};

class MockHttpCache {
 public:
  MockHttpCache();
  explicit MockHttpCache(net::HttpCache::BackendFactory* disk_cache_factory);

  net::HttpCache* http_cache() { return &http_cache_; }

  MockNetworkLayer* network_layer() {
    return static_cast<MockNetworkLayer*>(http_cache_.network_layer());
  }
  MockDiskCache* disk_cache();

  
  static bool ReadResponseInfo(disk_cache::Entry* disk_entry,
                               net::HttpResponseInfo* response_info,
                               bool* response_truncated);

  
  static bool WriteResponseInfo(disk_cache::Entry* disk_entry,
                                const net::HttpResponseInfo* response_info,
                                bool skip_transient_headers,
                                bool response_truncated);

  
  bool OpenBackendEntry(const std::string& key, disk_cache::Entry** entry);

  
  bool CreateBackendEntry(const std::string& key, disk_cache::Entry** entry,
                          net::NetLog* net_log);

  
  static int GetTestMode(int test_mode);

  
  
  static void SetTestMode(int test_mode);

 private:
  net::HttpCache http_cache_;
};

class MockDiskCacheNoCB : public MockDiskCache {
  virtual int CreateEntry(const std::string& key, disk_cache::Entry** entry,
                          const net::CompletionCallback& callback) OVERRIDE;
};

class MockBackendNoCbFactory : public net::HttpCache::BackendFactory {
 public:
  virtual int CreateBackend(net::NetLog* net_log,
                            scoped_ptr<disk_cache::Backend>* backend,
                            const net::CompletionCallback& callback) OVERRIDE;
};

class MockBlockingBackendFactory : public net::HttpCache::BackendFactory {
 public:
  MockBlockingBackendFactory();
  virtual ~MockBlockingBackendFactory();

  virtual int CreateBackend(net::NetLog* net_log,
                            scoped_ptr<disk_cache::Backend>* backend,
                            const net::CompletionCallback& callback) OVERRIDE;

  
  
  void FinishCreation();

  scoped_ptr<disk_cache::Backend>* backend() { return backend_; }
  void set_fail(bool fail) { fail_ = fail; }

  const net::CompletionCallback& callback() { return callback_; }

 private:
  int Result() { return fail_ ? net::ERR_FAILED : net::OK; }

  scoped_ptr<disk_cache::Backend>* backend_;
  net::CompletionCallback callback_;
  bool block_;
  bool fail_;
};

#endif  
