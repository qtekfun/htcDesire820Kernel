// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_EXTENSION_APP_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_EXTENSION_APP_PROVIDER_H_
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class TemplateURLModel;

class ExtensionAppProvider : public AutocompleteProvider,
                             public NotificationObserver {
 public:
  ExtensionAppProvider(ACProviderListener* listener, Profile* profile);

  
  void AddExtensionAppForTesting(const std::string& app_name,
                                 const std::string url);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

 private:
  
  typedef std::pair<std::string, std::string> ExtensionApp;
  typedef std::vector<ExtensionApp> ExtensionApps;

  virtual ~ExtensionAppProvider();

  
  void RefreshAppList();

  
  void RegisterForNotifications();

  
  
  
  
  
  void HighlightMatch(const AutocompleteInput& input,
                      ACMatchClassifications* match_class,
                      std::string::const_iterator iter,
                      const std::string& match_string);

  
  int CalculateRelevance(AutocompleteInput::Type type,
                         int input_length,
                         int target_length,
                         const GURL& url);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  NotificationRegistrar registrar_;

  
  
  ExtensionApps extension_apps_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAppProvider);
};

#endif  
