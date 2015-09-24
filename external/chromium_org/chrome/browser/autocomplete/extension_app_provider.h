// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_EXTENSION_APP_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_EXTENSION_APP_PROVIDER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/window_open_disposition.h"

class ExtensionAppProvider : public AutocompleteProvider,
                             public content::NotificationObserver {
 public:
  ExtensionAppProvider(AutocompleteProviderListener* listener,
                       Profile* profile);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

  
  
  
  
  static void LaunchAppFromOmnibox(const AutocompleteMatch& match,
                                   Profile* profile,
                                   WindowOpenDisposition disposition);

 private:
  friend class ExtensionAppProviderTest;
  FRIEND_TEST_ALL_PREFIXES(ExtensionAppProviderTest, CreateMatchSanitize);

  
  
  struct ExtensionApp {
    
    base::string16 name;
    
    
    base::string16 launch_url;
    
    
    
    bool should_match_against_launch_url;
  };
  typedef std::vector<ExtensionApp> ExtensionApps;

  virtual ~ExtensionAppProvider();

  void AddExtensionAppForTesting(const ExtensionApp& extension_app);

  
  AutocompleteMatch CreateAutocompleteMatch(const AutocompleteInput& input,
                                            const ExtensionApp& app,
                                            size_t name_match_index,
                                            size_t url_match_index);

  
  void RefreshAppList();

  
  int CalculateRelevance(AutocompleteInput::Type type,
                         int input_length,
                         int target_length,
                         const GURL& url);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  
  
  ExtensionApps extension_apps_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAppProvider);
};

#endif  
