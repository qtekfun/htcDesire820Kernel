// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_METRICS_LOG_H_
#define CHROME_BROWSER_METRICS_METRICS_LOG_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/common/metrics_helpers.h"
#include "content/common/page_transition_types.h"

struct AutocompleteLog;
class DictionaryValue;
class GURL;
class PrefService;

namespace webkit {
namespace npapi {
struct WebPluginInfo;
}
}

class MetricsLog : public MetricsLogBase {
 public:
  
  
  
  MetricsLog(const std::string& client_id, int session_id);
  virtual ~MetricsLog();

  static void RegisterPrefs(PrefService* prefs);

  
  
  
  
  
  
  void RecordEnvironment(
      const std::vector<webkit::npapi::WebPluginInfo>& plugin_list,
      const DictionaryValue* profile_metrics);

  
  
  void RecordOmniboxOpenedURL(const AutocompleteLog& log);

  
  
  
  
  void RecordIncrementalStabilityElements();

  
  
  static int64 GetIncrementalUptime(PrefService* pref);

  
  static std::string GetVersionString();

  virtual MetricsLog* AsMetricsLog();

 private:
  FRIEND_TEST_ALL_PREFIXES(MetricsLogTest, ChromeOSStabilityData);

  
  
  std::string GetInstallDate() const;


  
  
  void WriteStabilityElement(PrefService* pref);

  
  void WritePluginStabilityElements(PrefService* pref);

  
  void WriteRequiredStabilityAttributes(PrefService* pref);

  
  
  
  
  void WriteRealtimeStabilityAttributes(PrefService* pref);

  
  void WritePluginList(
      const std::vector<webkit::npapi::WebPluginInfo>& plugin_list);

  
  void WriteInstallElement();

  
  
  void WriteAllProfilesMetrics(const DictionaryValue& all_profiles_metrics);

  
  
  void WriteProfileMetrics(const std::string& key,
                           const DictionaryValue& profile_metrics);

  DISALLOW_COPY_AND_ASSIGN(MetricsLog);
};

#endif  
