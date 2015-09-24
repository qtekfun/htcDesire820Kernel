// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_THUMBNAIL_SERVICE_FACTORY_H_
#define CHROME_BROWSER_THUMBNAILS_THUMBNAIL_SERVICE_FACTORY_H_

#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service_factory.h"

class Profile;

namespace thumbnails {
class ThumbnailService;
}

class ThumbnailServiceFactory
    : public RefcountedBrowserContextKeyedServiceFactory {
 public:
  
  
  
  
  
  
  
  static scoped_refptr<thumbnails::ThumbnailService> GetForProfile(
      Profile* profile);

  static ThumbnailServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<ThumbnailServiceFactory>;

  ThumbnailServiceFactory();
  virtual ~ThumbnailServiceFactory();

  
  virtual scoped_refptr<RefcountedBrowserContextKeyedService>
      BuildServiceInstanceFor(content::BrowserContext* profile) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ThumbnailServiceFactory);
};

#endif  
