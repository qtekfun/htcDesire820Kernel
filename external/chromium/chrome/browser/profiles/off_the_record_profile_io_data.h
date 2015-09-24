// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_OFF_THE_RECORD_PROFILE_IO_DATA_H_
#define CHROME_BROWSER_PROFILES_OFF_THE_RECORD_PROFILE_IO_DATA_H_
#pragma once

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/profiles/profile_io_data.h"

class ChromeURLRequestContext;
class ChromeURLRequestContextGetter;
class Profile;


class OffTheRecordProfileIOData : public ProfileIOData {
 public:
  class Handle {
   public:
    explicit Handle(Profile* profile);
    ~Handle();

    const content::ResourceContext& GetResourceContext() const;
    scoped_refptr<ChromeURLRequestContextGetter>
        GetMainRequestContextGetter() const;
    scoped_refptr<ChromeURLRequestContextGetter>
        GetExtensionsRequestContextGetter() const;
    scoped_refptr<ChromeURLRequestContextGetter>
        GetIsolatedAppRequestContextGetter(
            const std::string& app_id) const;

   private:
    typedef base::hash_map<std::string,
                           scoped_refptr<ChromeURLRequestContextGetter> >
        ChromeURLRequestContextGetterMap;

    
    
    
    
    
    void LazyInitialize() const;

    
    
    
    
    
    
    mutable scoped_refptr<ChromeURLRequestContextGetter>
        main_request_context_getter_;
    mutable scoped_refptr<ChromeURLRequestContextGetter>
        extensions_request_context_getter_;
    mutable ChromeURLRequestContextGetterMap
        app_request_context_getter_map_;
    const scoped_refptr<OffTheRecordProfileIOData> io_data_;

    Profile* const profile_;

    mutable bool initialized_;

    DISALLOW_COPY_AND_ASSIGN(Handle);
  };

 private:
  friend class base::RefCountedThreadSafe<OffTheRecordProfileIOData>;

  typedef base::hash_map<std::string, net::HttpTransactionFactory* >
      HttpTransactionFactoryMap;

  OffTheRecordProfileIOData();
  ~OffTheRecordProfileIOData();

  virtual void LazyInitializeInternal(ProfileParams* profile_params) const;
  virtual scoped_refptr<RequestContext> InitializeAppRequestContext(
      scoped_refptr<ChromeURLRequestContext> main_context,
      const std::string& app_id) const;
  virtual scoped_refptr<ChromeURLRequestContext>
      AcquireMediaRequestContext() const;
  virtual scoped_refptr<ChromeURLRequestContext>
      AcquireIsolatedAppRequestContext(
          scoped_refptr<ChromeURLRequestContext> main_context,
          const std::string& app_id) const;

  mutable scoped_ptr<net::HttpTransactionFactory> main_http_factory_;

  
  mutable HttpTransactionFactoryMap app_http_factory_map_;

  DISALLOW_COPY_AND_ASSIGN(OffTheRecordProfileIOData);
};

#endif  
