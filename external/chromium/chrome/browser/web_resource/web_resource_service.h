// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_WEB_RESOURCE_SERVICE_H_
#define CHROME_BROWSER_WEB_RESOURCE_WEB_RESOURCE_SERVICE_H_
#pragma once

#include <string>

#include "chrome/browser/utility_process_host.h"
#include "content/common/notification_type.h"

class PrefService;
class Profile;
class ResourceDispatcherHost;

class WebResourceService
    : public UtilityProcessHost::Client {
 public:
  
  
  WebResourceService(Profile* profile,
                     PrefService* prefs,
                     const char* web_resource_server,
                     bool apply_locale_to_url_,
                     NotificationType::Type notification_type,
                     const char* last_update_time_pref_name,
                     int start_fetch_delay,
                     int cache_update_delay);

  
  
  void StartAfterDelay();

  
  
  void UpdateResourceCache(const std::string& json_data);

 protected:
  virtual ~WebResourceService();

  virtual void Unpack(const DictionaryValue& parsed_json) = 0;

  
  
  
  void PostNotification(int64 delay_ms);

  
  
  PrefService* prefs_;

  Profile* profile_;

 private:
  class WebResourceFetcher;
  friend class WebResourceFetcher;

  class UnpackerClient;

  
  void EndFetch();

  
  void OnWebResourceUnpacked(const DictionaryValue& parsed_json);

  
  void WebResourceStateChange();

  scoped_ptr<WebResourceFetcher> web_resource_fetcher_;

  ResourceDispatcherHost* resource_dispatcher_host_;

  
  
  
  ScopedRunnableMethodFactory<WebResourceService> service_factory_;

  
  
  
  bool in_fetch_;

  
  const char* web_resource_server_;

  
  bool apply_locale_to_url_;

  
  NotificationType::Type notification_type_;

  
  const char* last_update_time_pref_name_;

  
  int start_fetch_delay_;

  
  
  int cache_update_delay_;

  
  
  bool web_resource_update_scheduled_;

  DISALLOW_COPY_AND_ASSIGN(WebResourceService);
};

#endif  
