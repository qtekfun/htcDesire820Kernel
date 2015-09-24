// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_VIEW_CACHE_HELPER_H_
#define NET_URL_REQUEST_VIEW_CACHE_HELPER_H_
#pragma once

#include <string>

#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"

namespace disk_cache {
class Backend;
class Entry;
}  

namespace net {

class URLRequestContext;

class ViewCacheHelper {
 public:
  ViewCacheHelper();
  ~ViewCacheHelper();

  
  
  
  
  int GetEntryInfoHTML(const std::string& key,
                       URLRequestContext* context,
                       std::string* out,
                       CompletionCallback* callback);

  
  
  
  
  
  int GetContentsHTML(URLRequestContext* context,
                      const std::string& url_prefix,
                      std::string* out,
                      CompletionCallback* callback);

 private:
  enum State {
    STATE_NONE,
    STATE_GET_BACKEND,
    STATE_GET_BACKEND_COMPLETE,
    STATE_OPEN_NEXT_ENTRY,
    STATE_OPEN_NEXT_ENTRY_COMPLETE,
    STATE_OPEN_ENTRY,
    STATE_OPEN_ENTRY_COMPLETE,
    STATE_READ_RESPONSE,
    STATE_READ_RESPONSE_COMPLETE,
    STATE_READ_DATA,
    STATE_READ_DATA_COMPLETE
  };

  
  int GetInfoHTML(const std::string& key,
                  URLRequestContext* context,
                  const std::string& url_prefix,
                  std::string* out,
                  CompletionCallback* callback);

  
  
  void DoCallback(int rv);

  
  void HandleResult(int rv);

  
  int DoLoop(int result);

  
  
  
  int DoGetBackend();
  int DoGetBackendComplete(int result);
  int DoOpenNextEntry();
  int DoOpenNextEntryComplete(int result);
  int DoOpenEntry();
  int DoOpenEntryComplete(int result);
  int DoReadResponse();
  int DoReadResponseComplete(int result);
  int DoReadData();
  int DoReadDataComplete(int result);

  
  void OnIOComplete(int result);

  scoped_refptr<URLRequestContext> context_;
  disk_cache::Backend* disk_cache_;
  disk_cache::Entry* entry_;
  void* iter_;
  scoped_refptr<IOBuffer> buf_;
  int buf_len_;
  int index_;

  std::string key_;
  std::string url_prefix_;
  std::string* data_;
  CompletionCallback* callback_;

  State next_state_;

  CompletionCallbackImpl<ViewCacheHelper> cache_callback_;
  scoped_refptr<CancelableCompletionCallback<ViewCacheHelper> > entry_callback_;

  DISALLOW_COPY_AND_ASSIGN(ViewCacheHelper);
};

}  

#endif  
