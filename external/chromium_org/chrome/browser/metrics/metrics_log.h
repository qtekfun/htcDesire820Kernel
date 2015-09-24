// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_METRICS_LOG_H_
#define CHROME_BROWSER_METRICS_METRICS_LOG_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/metrics/metrics_network_observer.h"
#include "chrome/common/metrics/metrics_log_base.h"
#include "chrome/common/metrics/variations/variations_util.h"
#include "chrome/installer/util/google_update_settings.h"
#include "ui/gfx/size.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/metrics/perf_provider_chromeos.h"
#endif

class MetricsNetworkObserver;
struct OmniboxLog;
class PrefService;
class PrefRegistrySimple;

namespace base {
class DictionaryValue;
}

namespace content {
struct WebPluginInfo;
}

namespace device {
class BluetoothAdapter;
}

namespace tracked_objects {
struct ProcessDataSnapshot;
}

namespace chrome_variations {
struct ActiveGroupId;
}

struct GoogleUpdateMetrics {
    GoogleUpdateMetrics();
    ~GoogleUpdateMetrics();

    
    bool is_system_install;
    
    base::Time last_started_au;
    
    
    base::Time last_checked;
    
    GoogleUpdateSettings::ProductData google_update_data;
    
    GoogleUpdateSettings::ProductData product_data;
};

class MetricsLog : public MetricsLogBase {
 public:
  
  
  
  MetricsLog(const std::string& client_id, int session_id);
  virtual ~MetricsLog();

  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  static std::string GetVersionString();

  
  
  static void set_version_extension(const std::string& extension);
  static const std::string& version_extension();

  
  
  
  
  
  
  void RecordEnvironment(
      const std::vector<content::WebPluginInfo>& plugin_list,
      const GoogleUpdateMetrics& google_update_metrics,
      const std::vector<chrome_variations::ActiveGroupId>& synthetic_trials);

  
  
  
  bool LoadSavedEnvironmentFromPrefs();

  
  
  void RecordOmniboxOpenedURL(const OmniboxLog& log);

  
  
  void RecordProfilerData(
      const tracked_objects::ProcessDataSnapshot& process_data,
      int process_type);

  
  
  
  
  
  
  
  void RecordStabilityMetrics(
      base::TimeDelta incremental_uptime,
      LogType log_type);

  const base::TimeTicks& creation_time() const {
    return creation_time_;
  }

 protected:
  

  
  virtual PrefService* GetPrefService();

  
  virtual gfx::Size GetScreenSize() const;

  
  virtual float GetScreenDeviceScaleFactor() const;

  
  virtual int GetScreenCount() const;

  
  
  virtual void GetFieldTrialIds(
      std::vector<chrome_variations::ActiveGroupId>* field_trial_ids) const;

 private:
  FRIEND_TEST_ALL_PREFIXES(MetricsLogTest, ChromeOSStabilityData);

  
  
  bool HasEnvironment() const;

  
  
  bool HasStabilityMetrics() const;

  
  void WritePluginStabilityElements(PrefService* pref);

  
  void WriteRequiredStabilityAttributes(PrefService* pref);

  
  
  
  
  void WriteRealtimeStabilityAttributes(PrefService* pref,
                                        base::TimeDelta incremental_uptime);

  
  void WritePluginList(const std::vector<content::WebPluginInfo>& plugin_list);

  
  
  void WriteGoogleUpdateProto(const GoogleUpdateMetrics& google_update_metrics);

  
  
  void SetBluetoothAdapter(scoped_refptr<device::BluetoothAdapter> adapter);

  
  
  void WriteBluetoothProto(metrics::SystemProfileProto::Hardware* hardware);

#if defined(OS_CHROMEOS)
  
  
  
  void UpdateMultiProfileUserCount();
#endif

  
  MetricsNetworkObserver network_observer_;

#if defined(OS_CHROMEOS)
  metrics::PerfProvider perf_provider_;
#endif

  
  scoped_refptr<device::BluetoothAdapter> adapter_;

  
  const base::TimeTicks creation_time_;

  DISALLOW_COPY_AND_ASSIGN(MetricsLog);
};

#endif  
