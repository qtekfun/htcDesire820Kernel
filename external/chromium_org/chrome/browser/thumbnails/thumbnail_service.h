// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_THUMBNAIL_SERVICE_H_
#define CHROME_BROWSER_THUMBNAILS_THUMBNAIL_SERVICE_H_

#include "chrome/common/thumbnail_score.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"
#include "ui/gfx/image/image.h"

class GURL;

namespace base {
class RefCountedMemory;
}

namespace thumbnails {

class ThumbnailingAlgorithm;
struct ThumbnailingContext;

class ThumbnailService : public RefcountedBrowserContextKeyedService {
 public:
  
  
  
  virtual bool SetPageThumbnail(const ThumbnailingContext& context,
                                const gfx::Image& thumbnail) = 0;

  
  
  
  
  
  virtual ThumbnailingAlgorithm* GetThumbnailingAlgorithm() const = 0;

  
  
  
  
  
  
  
  virtual bool GetPageThumbnail(
      const GURL& url,
      bool prefix_match,
      scoped_refptr<base::RefCountedMemory>* bytes) = 0;

  
  
  virtual void AddForcedURL(const GURL& url) = 0;

  
  virtual bool ShouldAcquirePageThumbnail(const GURL& url) = 0;

 protected:
  virtual ~ThumbnailService() {}
};

}  

#endif  
