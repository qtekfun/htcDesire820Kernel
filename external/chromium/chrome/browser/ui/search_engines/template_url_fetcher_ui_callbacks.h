// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_UI_CALLBACKS_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_UI_CALLBACKS_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/search_engines/template_url_fetcher_callbacks.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class SearchEngineTabHelper;
class TabContents;

class TemplateURLFetcherUICallbacks : public TemplateURLFetcherCallbacks,
                                      public NotificationObserver {
 public:
  explicit TemplateURLFetcherUICallbacks(SearchEngineTabHelper* tab_helper,
                                         TabContents* tab_contents);
  virtual ~TemplateURLFetcherUICallbacks();

  
  virtual void ConfirmSetDefaultSearchProvider(
      TemplateURL* template_url,
      TemplateURLModel* template_url_model);
  virtual void ConfirmAddSearchProvider(
      TemplateURL* template_url,
      Profile* profile);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  
  SearchEngineTabHelper* source_;

  
  TabContents* tab_contents_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLFetcherUICallbacks);
};

#endif  
