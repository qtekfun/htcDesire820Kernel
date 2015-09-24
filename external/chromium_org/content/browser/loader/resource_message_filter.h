// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_RESOURCE_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_MESSAGE_FILTER_H_

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_message_filter.h"
#include "webkit/common/resource_type.h"

struct ResourceHostMsg_Request;

namespace fileapi {
class FileSystemContext;
}  

namespace net {
class URLRequestContext;
}  


namespace content {
class ChromeAppCacheService;
class ChromeBlobStorageContext;
class ResourceContext;

class CONTENT_EXPORT ResourceMessageFilter : public BrowserMessageFilter {
 public:
  typedef base::Callback<void(const ResourceHostMsg_Request&,
                              ResourceContext**,
                              net::URLRequestContext**)> GetContextsCallback;

  
  
  ResourceMessageFilter(
      int child_id,
      int process_type,
      ChromeAppCacheService* appcache_service,
      ChromeBlobStorageContext* blob_storage_context,
      fileapi::FileSystemContext* file_system_context,
      const GetContextsCallback& get_contexts_callback);

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  void GetContexts(const ResourceHostMsg_Request& request,
                   ResourceContext** resource_context,
                   net::URLRequestContext** request_context);

  
  net::URLRequestContext* GetURLRequestContext(
      ResourceType::Type request_type);

  ChromeAppCacheService* appcache_service() const {
    return appcache_service_.get();
  }

  ChromeBlobStorageContext* blob_storage_context() const {
    return blob_storage_context_.get();
  }

  fileapi::FileSystemContext* file_system_context() const {
    return file_system_context_.get();
  }

  int child_id() const { return child_id_; }
  int process_type() const { return process_type_; }

  base::WeakPtr<ResourceMessageFilter> GetWeakPtr();

 protected:
  
  virtual ~ResourceMessageFilter();

 private:
  
  int child_id_;

  int process_type_;

  scoped_refptr<ChromeAppCacheService> appcache_service_;
  scoped_refptr<ChromeBlobStorageContext> blob_storage_context_;
  scoped_refptr<fileapi::FileSystemContext> file_system_context_;

  GetContextsCallback get_contexts_callback_;

  
  base::WeakPtrFactory<ResourceMessageFilter> weak_ptr_factory_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ResourceMessageFilter);
};

}  

#endif  
