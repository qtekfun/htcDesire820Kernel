// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_OFF_THE_RECORD_PROFILE_IO_DATA_H_
#define CHROME_BROWSER_PROFILES_OFF_THE_RECORD_PROFILE_IO_DATA_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/custom_handlers/protocol_handler_registry.h"
#include "chrome/browser/profiles/profile_io_data.h"
#include "chrome/browser/profiles/storage_partition_descriptor.h"

class ChromeURLRequestContext;
class ChromeURLRequestContextGetter;
class Profile;

namespace net {
class FtpTransactionFactory;
class HttpTransactionFactory;
}  


class OffTheRecordProfileIOData : public ProfileIOData {
 public:
  class Handle {
   public:
    explicit Handle(Profile* profile);
    ~Handle();

    content::ResourceContext* GetResourceContext() const;
    
    
    content::ResourceContext* GetResourceContextNoInit() const;
    scoped_refptr<ChromeURLRequestContextGetter>
        CreateMainRequestContextGetter(
            content::ProtocolHandlerMap* protocol_handlers) const;
    scoped_refptr<ChromeURLRequestContextGetter>
        GetExtensionsRequestContextGetter() const;
    scoped_refptr<ChromeURLRequestContextGetter>
        GetIsolatedAppRequestContextGetter(
            const base::FilePath& partition_path,
            bool in_memory) const;
    scoped_refptr<ChromeURLRequestContextGetter>
        CreateIsolatedAppRequestContextGetter(
            const base::FilePath& partition_path,
            bool in_memory,
            content::ProtocolHandlerMap* protocol_handlers) const;

   private:
    typedef std::map<StoragePartitionDescriptor,
                     scoped_refptr<ChromeURLRequestContextGetter>,
                     StoragePartitionDescriptorLess>
      ChromeURLRequestContextGetterMap;

    
    
    
    
    
    void LazyInitialize() const;

    
    
    
    
    
    
    mutable scoped_refptr<ChromeURLRequestContextGetter>
        main_request_context_getter_;
    mutable scoped_refptr<ChromeURLRequestContextGetter>
        extensions_request_context_getter_;
    mutable ChromeURLRequestContextGetterMap
        app_request_context_getter_map_;
    OffTheRecordProfileIOData* const io_data_;

    Profile* const profile_;

    mutable bool initialized_;

    DISALLOW_COPY_AND_ASSIGN(Handle);
  };

 private:
  friend class base::RefCountedThreadSafe<OffTheRecordProfileIOData>;

  OffTheRecordProfileIOData();
  virtual ~OffTheRecordProfileIOData();

  virtual void InitializeInternal(
      ProfileParams* profile_params,
      content::ProtocolHandlerMap* protocol_handlers) const OVERRIDE;
  virtual void InitializeExtensionsRequestContext(
      ProfileParams* profile_params) const OVERRIDE;
  virtual ChromeURLRequestContext* InitializeAppRequestContext(
      ChromeURLRequestContext* main_context,
      const StoragePartitionDescriptor& partition_descriptor,
      scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
          protocol_handler_interceptor,
      content::ProtocolHandlerMap* protocol_handlers) const OVERRIDE;
  virtual ChromeURLRequestContext* InitializeMediaRequestContext(
      ChromeURLRequestContext* original_context,
      const StoragePartitionDescriptor& partition_descriptor) const OVERRIDE;
  virtual ChromeURLRequestContext*
      AcquireMediaRequestContext() const OVERRIDE;
  virtual ChromeURLRequestContext*
      AcquireIsolatedAppRequestContext(
          ChromeURLRequestContext* main_context,
          const StoragePartitionDescriptor& partition_descriptor,
          scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
              protocol_handler_interceptor,
          content::ProtocolHandlerMap* protocol_handlers) const OVERRIDE;
  virtual ChromeURLRequestContext*
      AcquireIsolatedMediaRequestContext(
          ChromeURLRequestContext* app_context,
          const StoragePartitionDescriptor& partition_descriptor)
              const OVERRIDE;

  virtual chrome_browser_net::LoadTimeStats* GetLoadTimeStats(
      IOThread::Globals* io_thread_globals) const OVERRIDE;

  mutable scoped_ptr<net::HttpTransactionFactory> main_http_factory_;
  mutable scoped_ptr<net::FtpTransactionFactory> ftp_factory_;

  mutable scoped_ptr<net::URLRequestJobFactory> main_job_factory_;
  mutable scoped_ptr<net::URLRequestJobFactory> extensions_job_factory_;

  DISALLOW_COPY_AND_ASSIGN(OffTheRecordProfileIOData);
};

#endif  
