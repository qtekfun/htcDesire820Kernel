// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_CALLBACKS_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_CALLBACKS_H_
#pragma once

class Profile;
class TemplateURL;
class TemplateURLModel;

class TemplateURLFetcherCallbacks {
 public:
  TemplateURLFetcherCallbacks() {}
  virtual ~TemplateURLFetcherCallbacks() {}

  
  
  virtual void ConfirmSetDefaultSearchProvider(
      TemplateURL* template_url,
      TemplateURLModel* template_url_model) = 0;

  
  
  virtual void ConfirmAddSearchProvider(
      TemplateURL* template_url,
      Profile* profile) = 0;
};

#endif  
