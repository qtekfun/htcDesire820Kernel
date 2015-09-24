// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_SERVICE_H__
#define CHROME_BROWSER_FAVICON_SERVICE_H__
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "base/task.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/common/ref_counted_util.h"
#include "content/browser/cancelable_request.h"

class GURL;
class Profile;

class FaviconService : public CancelableRequestProvider,
                       public base::RefCountedThreadSafe<FaviconService> {
 public:
  explicit FaviconService(Profile* profile);

  
  
  
  
  
  
  
  
  
  
  typedef Callback2<Handle,                             
                    history::FaviconData>::Type  
                    FaviconDataCallback;

  typedef CancelableRequest<FaviconDataCallback> GetFaviconRequest;

  
  
  Handle GetFavicon(const GURL& icon_url,
                    history::IconType icon_type,
                    CancelableRequestConsumerBase* consumer,
                    FaviconDataCallback* callback);

  
  
  
  
  
  Handle UpdateFaviconMappingAndFetch(const GURL& page_url,
                                      const GURL& icon_url,
                                      history::IconType icon_type,
                                      CancelableRequestConsumerBase* consumer,
                                      FaviconDataCallback* callback);

  
  
  
  
  
  
  
  
  Handle GetFaviconForURL(const GURL& page_url,
                          int icon_types,
                          CancelableRequestConsumerBase* consumer,
                          FaviconDataCallback* callback);

  
  void SetFaviconOutOfDateForPage(const GURL& page_url);

  
  
  // favicons will not be overwritten.
  void SetImportedFavicons(
      const std::vector<history::ImportedFaviconUsage>& favicon_usage);

  
  void SetFavicon(const GURL& page_url,
                  const GURL& icon_url,
                  const std::vector<unsigned char>& image_data,
                  history::IconType icon_type);

 private:
  friend class base::RefCountedThreadSafe<FaviconService>;

  ~FaviconService();

  Profile* profile_;

  
  void ForwardEmptyResultAsync(GetFaviconRequest* request);

  DISALLOW_COPY_AND_ASSIGN(FaviconService);
};

#endif  
