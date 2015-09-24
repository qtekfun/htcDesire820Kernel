// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_H_
#pragma once

#include "base/memory/scoped_vector.h"
#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"

class GURL;
class Profile;
class TemplateURL;
class TemplateURLFetcherCallbacks;

class TemplateURLFetcher {
 public:
  enum ProviderType {
    AUTODETECTED_PROVIDER,
    EXPLICIT_PROVIDER,  
    EXPLICIT_DEFAULT_PROVIDER  
  };

  
  explicit TemplateURLFetcher(Profile* profile);
  ~TemplateURLFetcher();

  
  
  
  void ScheduleDownload(const string16& keyword,
                        const GURL& osdd_url,
                        const GURL& favicon_url,
                        TemplateURLFetcherCallbacks* callbacks,
                        ProviderType provider_type);

  
  int requests_count() const { return requests_->size(); }

 private:
  friend class RequestDelegate;

  
  
  class RequestDelegate;

  Profile* profile() const { return profile_; }

  
  void RequestCompleted(RequestDelegate* request);

  Profile* profile_;

  
  ScopedVector<RequestDelegate> requests_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLFetcher);
};

#endif  
