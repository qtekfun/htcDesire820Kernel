// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_H_

#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "ui/gfx/native_widget_types.h"

class GURL;
class Profile;
class TemplateURL;
class TemplateURLFetcherCallbacks;

namespace content {
class WebContents;
}

class TemplateURLFetcher : public BrowserContextKeyedService {
 public:
  enum ProviderType {
    AUTODETECTED_PROVIDER,
    EXPLICIT_PROVIDER  
  };

  
  explicit TemplateURLFetcher(Profile* profile);
  virtual ~TemplateURLFetcher();

  
  
  
  
  
  
  
  
  
  
  
  
  void ScheduleDownload(const base::string16& keyword,
                        const GURL& osdd_url,
                        const GURL& favicon_url,
                        content::WebContents* web_contents,
                        TemplateURLFetcherCallbacks* callbacks,
                        ProviderType provider_type);

  
  int requests_count() const { return requests_.size(); }

 private:
  
  
  class RequestDelegate;
  friend class RequestDelegate;

  typedef ScopedVector<RequestDelegate> Requests;

  Profile* profile() const { return profile_; }

  
  void RequestCompleted(RequestDelegate* request);

  Profile* profile_;

  
  Requests requests_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLFetcher);
};

#endif  
