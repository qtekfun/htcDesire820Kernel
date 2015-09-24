// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CHROME_URL_REQUEST_CONTEXT_H_
#define CHROME_BROWSER_NET_CHROME_URL_REQUEST_CONTEXT_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/custom_handlers/protocol_handler_registry.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_getter.h"
#include "net/url_request/url_request_job_factory.h"

class ChromeURLRequestContextFactory;
class IOThread;
class Profile;
class ProfileIOData;
struct StoragePartitionDescriptor;

namespace chrome_browser_net {
class LoadTimeStats;
}

class ChromeURLRequestContext : public net::URLRequestContext {
 public:
  enum ContextType {
    CONTEXT_TYPE_MAIN,
    CONTEXT_TYPE_MEDIA,
    CONTEXT_TYPE_EXTENSIONS,
    CONTEXT_TYPE_APP
  };
  ChromeURLRequestContext(ContextType type,
                          chrome_browser_net::LoadTimeStats* load_time_stats);
  virtual ~ChromeURLRequestContext();

  base::WeakPtr<ChromeURLRequestContext> GetWeakPtr() {
    return weak_factory_.GetWeakPtr();
  }

  
  void CopyFrom(ChromeURLRequestContext* other);

 private:
  base::WeakPtrFactory<ChromeURLRequestContext> weak_factory_;

  
  
  
  

  chrome_browser_net::LoadTimeStats* load_time_stats_;

  
  
  
  

  DISALLOW_COPY_AND_ASSIGN(ChromeURLRequestContext);
};

class ChromeURLRequestContextGetter : public net::URLRequestContextGetter {
 public:
  
  
  explicit ChromeURLRequestContextGetter(
      ChromeURLRequestContextFactory* factory);

  
  
  
  
  
  virtual ChromeURLRequestContext* GetURLRequestContext() OVERRIDE;
  virtual scoped_refptr<base::SingleThreadTaskRunner>
      GetNetworkTaskRunner() const OVERRIDE;

  
  
  static ChromeURLRequestContextGetter* Create(
      Profile* profile,
      const ProfileIOData* profile_io_data,
      content::ProtocolHandlerMap* protocol_handlers);

  
  
  
  static ChromeURLRequestContextGetter* CreateForMedia(
      Profile* profile, const ProfileIOData* profile_io_data);

  
  
  static ChromeURLRequestContextGetter* CreateForExtensions(
      Profile* profile, const ProfileIOData* profile_io_data);

  
  
  static ChromeURLRequestContextGetter* CreateForIsolatedApp(
      Profile* profile,
      const ProfileIOData* profile_io_data,
      const StoragePartitionDescriptor& partition_descriptor,
      scoped_ptr<ProtocolHandlerRegistry::JobInterceptorFactory>
          protocol_handler_interceptor,
      content::ProtocolHandlerMap* protocol_handlers);

  
  
  static ChromeURLRequestContextGetter* CreateForIsolatedMedia(
      Profile* profile,
      ChromeURLRequestContextGetter* app_context,
      const ProfileIOData* profile_io_data,
      const StoragePartitionDescriptor& partition_descriptor);

 private:
  virtual ~ChromeURLRequestContextGetter();

  
  
  scoped_ptr<ChromeURLRequestContextFactory> factory_;

  
  
  
  base::WeakPtr<ChromeURLRequestContext> url_request_context_;

  DISALLOW_COPY_AND_ASSIGN(ChromeURLRequestContextGetter);
};

#endif  
