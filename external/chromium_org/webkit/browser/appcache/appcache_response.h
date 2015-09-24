// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_APPCACHE_APPCACHE_RESPONSE_H_
#define WEBKIT_BROWSER_APPCACHE_APPCACHE_RESPONSE_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/http/http_response_info.h"
#include "url/gurl.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/appcache/appcache_interfaces.h"

namespace net {
class IOBuffer;
}

namespace appcache {

class AppCacheStorage;

static const int kUnkownResponseDataSize = -1;

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheResponseInfo
    : public base::RefCounted<AppCacheResponseInfo> {
 public:
  
  AppCacheResponseInfo(AppCacheStorage* storage, const GURL& manifest_url,
                       int64 response_id, net::HttpResponseInfo* http_info,
                       int64 response_data_size);

  const GURL& manifest_url() const { return manifest_url_; }
  int64 response_id() const { return response_id_; }
  const net::HttpResponseInfo* http_response_info() const {
    return http_response_info_.get();
  }
  int64 response_data_size() const { return response_data_size_; }

 private:
  friend class base::RefCounted<AppCacheResponseInfo>;
  virtual ~AppCacheResponseInfo();

  const GURL manifest_url_;
  const int64 response_id_;
  const scoped_ptr<net::HttpResponseInfo> http_response_info_;
  const int64 response_data_size_;
  AppCacheStorage* storage_;
};

struct WEBKIT_STORAGE_BROWSER_EXPORT HttpResponseInfoIOBuffer
    : public base::RefCountedThreadSafe<HttpResponseInfoIOBuffer> {
  scoped_ptr<net::HttpResponseInfo> http_info;
  int response_data_size;

  HttpResponseInfoIOBuffer();
  explicit HttpResponseInfoIOBuffer(net::HttpResponseInfo* info);

 private:
  friend class base::RefCountedThreadSafe<HttpResponseInfoIOBuffer>;
  virtual ~HttpResponseInfoIOBuffer();
};

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheDiskCacheInterface {
 public:
  class Entry {
   public:
    virtual int Read(int index, int64 offset, net::IOBuffer* buf, int buf_len,
                     const net::CompletionCallback& callback) = 0;
    virtual int Write(int index, int64 offset, net::IOBuffer* buf, int buf_len,
                      const net::CompletionCallback& callback) = 0;
    virtual int64 GetSize(int index) = 0;
    virtual void Close() = 0;
   protected:
    virtual ~Entry() {}
  };

  virtual int CreateEntry(int64 key, Entry** entry,
                          const net::CompletionCallback& callback) = 0;
  virtual int OpenEntry(int64 key, Entry** entry,
                        const net::CompletionCallback& callback) = 0;
  virtual int DoomEntry(int64 key, const net::CompletionCallback& callback) = 0;

 protected:
  friend class base::RefCounted<AppCacheDiskCacheInterface>;
  virtual ~AppCacheDiskCacheInterface() {}
};

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheResponseIO {
 public:
  virtual ~AppCacheResponseIO();
  int64 response_id() const { return response_id_; }

 protected:
  AppCacheResponseIO(int64 response_id,
                     int64 group_id,
                     AppCacheDiskCacheInterface* disk_cache);

  virtual void OnIOComplete(int result) = 0;

  bool IsIOPending() { return !callback_.is_null(); }
  void ScheduleIOCompletionCallback(int result);
  void InvokeUserCompletionCallback(int result);
  void ReadRaw(int index, int offset, net::IOBuffer* buf, int buf_len);
  void WriteRaw(int index, int offset, net::IOBuffer* buf, int buf_len);

  const int64 response_id_;
  const int64 group_id_;
  AppCacheDiskCacheInterface* disk_cache_;
  AppCacheDiskCacheInterface::Entry* entry_;
  scoped_refptr<HttpResponseInfoIOBuffer> info_buffer_;
  scoped_refptr<net::IOBuffer> buffer_;
  int buffer_len_;
  net::CompletionCallback callback_;
  base::WeakPtrFactory<AppCacheResponseIO> weak_factory_;

 private:
  void OnRawIOComplete(int result);
};

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheResponseReader
    : public AppCacheResponseIO {
 public:
  virtual ~AppCacheResponseReader();

  
  
  
  
  
  
  
  
  
  
  virtual void ReadInfo(HttpResponseInfoIOBuffer* info_buf,
                        const net::CompletionCallback& callback);

  
  
  
  
  
  
  
  
  virtual void ReadData(net::IOBuffer* buf, int buf_len,
                        const net::CompletionCallback& callback);

  
  bool IsReadPending() { return IsIOPending(); }

  
  
  
  void SetReadRange(int offset, int length);

 protected:
  friend class AppCacheStorageImpl;
  friend class MockAppCacheStorage;

  
  AppCacheResponseReader(int64 response_id,
                         int64 group_id,
                         AppCacheDiskCacheInterface* disk_cache);

  virtual void OnIOComplete(int result) OVERRIDE;
  void ContinueReadInfo();
  void ContinueReadData();
  void OpenEntryIfNeededAndContinue();
  void OnOpenEntryComplete(AppCacheDiskCacheInterface::Entry** entry, int rv);

  int range_offset_;
  int range_length_;
  int read_position_;
  net::CompletionCallback open_callback_;
  base::WeakPtrFactory<AppCacheResponseReader> weak_factory_;
};

class WEBKIT_STORAGE_BROWSER_EXPORT AppCacheResponseWriter
    : public AppCacheResponseIO {
 public:
  virtual ~AppCacheResponseWriter();

  
  // asynchronously through the 'callback'. Returns the number of bytes written
  
  
  // negative error code or the number of bytes written. The 'callback' is a
  
  
  void WriteInfo(HttpResponseInfoIOBuffer* info_buf,
                 const net::CompletionCallback& callback);

  
  // asynchronously through the 'callback'. Returns the number of bytes written
  
  
  
  // the number of bytes written. The 'callback' is a required parameter.
  
  
  void WriteData(net::IOBuffer* buf, int buf_len,
                 const net::CompletionCallback& callback);

  
  bool IsWritePending() { return IsIOPending(); }

  // Returns the amount written, info and data.
  int64 amount_written() { return info_size_ + write_position_; }

 private:
  friend class AppCacheStorageImpl;
  friend class MockAppCacheStorage;

  enum CreationPhase {
    NO_ATTEMPT,
    INITIAL_ATTEMPT,
    DOOM_EXISTING,
    SECOND_ATTEMPT
  };

  
  AppCacheResponseWriter(int64 response_id,
                         int64 group_id,
                         AppCacheDiskCacheInterface* disk_cache);

  virtual void OnIOComplete(int result) OVERRIDE;
  void ContinueWriteInfo();
  void ContinueWriteData();
  void CreateEntryIfNeededAndContinue();
  void OnCreateEntryComplete(AppCacheDiskCacheInterface::Entry** entry, int rv);

  int info_size_;
  int write_position_;
  int write_amount_;
  CreationPhase creation_phase_;
  net::CompletionCallback create_callback_;
  base::WeakPtrFactory<AppCacheResponseWriter> weak_factory_;
};

}  

#endif  
