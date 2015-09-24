// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_STATUS_COLLECTOR_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_DEVICE_STATUS_COLLECTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chrome/browser/chromeos/version_loader.h"
#include "chrome/browser/idle.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "content/public/browser/geolocation_provider.h"
#include "content/public/common/geoposition.h"

namespace chromeos {
class CrosSettings;
namespace system {
class StatisticsProvider;
}
}

namespace content {
class NotificationDetails;
class NotificationSource;
}

namespace enterprise_management {
class DeviceStatusReportRequest;
}

class PrefRegistrySimple;
class PrefService;

namespace policy {

class DeviceStatusCollector : public CloudPolicyClient::StatusProvider {
 public:
  
  
  typedef base::Callback<void(
      const content::GeolocationProvider::LocationUpdateCallback& callback)>
          LocationUpdateRequester;

  DeviceStatusCollector(
      PrefService* local_state,
      chromeos::system::StatisticsProvider* provider,
      LocationUpdateRequester* location_update_requester);
  virtual ~DeviceStatusCollector();

  void GetStatus(enterprise_management::DeviceStatusReportRequest* request);

  
  virtual bool GetDeviceStatus(
      enterprise_management::DeviceStatusReportRequest* status) OVERRIDE;
  virtual bool GetSessionStatus(
      enterprise_management::SessionStatusReportRequest* status) OVERRIDE;
  virtual void OnSubmittedSuccessfully() OVERRIDE;

  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  static const unsigned int kIdlePollIntervalSeconds = 30;

 protected:
  
  virtual void CheckIdleState();

  
  virtual base::Time GetCurrentTime();

  
  void IdleStateCallback(IdleState state);

  
  
  unsigned int max_stored_past_activity_days_;

  
  
  
  unsigned int max_stored_future_activity_days_;

 private:
  
  
  class Context : public base::RefCountedThreadSafe<Context> {
   public:
    Context();

    void GetLocationUpdate(
        const content::GeolocationProvider::LocationUpdateCallback& callback);

   private:
    friend class base::RefCountedThreadSafe<Context>;

    ~Context();

    void GetLocationUpdateInternal();
    void OnLocationUpdate(const content::Geoposition& geoposition);
    void CallCollector(const content::Geoposition& geoposition);

    
    
    content::GeolocationProvider::LocationUpdateCallback our_callback_;

    
    content::GeolocationProvider::LocationUpdateCallback owner_callback_;
  };

  
  
  void PruneStoredActivityPeriods(base::Time base_time);

  
  
  
  void TrimStoredActivityPeriods(int64 min_day_key,
                                 int min_day_trim_duration,
                                 int64 max_day_key);

  void AddActivePeriod(base::Time start, base::Time end);

  
  void OnOSVersion(const std::string& version);
  void OnOSFirmware(const std::string& version);

  
  void GetActivityTimes(
      enterprise_management::DeviceStatusReportRequest* request);
  void GetVersionInfo(
      enterprise_management::DeviceStatusReportRequest* request);
  void GetBootMode(
      enterprise_management::DeviceStatusReportRequest* request);
  void GetLocation(
      enterprise_management::DeviceStatusReportRequest* request);
  void GetNetworkInterfaces(
      enterprise_management::DeviceStatusReportRequest* request);
  void GetUsers(
      enterprise_management::DeviceStatusReportRequest* request);

  
  void UpdateReportingSettings();

  void ScheduleGeolocationUpdateRequest();

  
  void ReceiveGeolocationUpdate(const content::Geoposition&);

  
  int poll_interval_seconds_;

  PrefService* local_state_;

  
  base::Time last_idle_check_;

  
  
  
  
  int64 last_reported_day_;
  int duration_for_last_reported_day_;

  
  bool geolocation_update_in_progress_;

  base::RepeatingTimer<DeviceStatusCollector> idle_poll_timer_;
  base::OneShotTimer<DeviceStatusCollector> geolocation_update_timer_;

  chromeos::VersionLoader version_loader_;
  CancelableTaskTracker tracker_;

  std::string os_version_;
  std::string firmware_version_;

  content::Geoposition position_;

  chromeos::system::StatisticsProvider* statistics_provider_;

  chromeos::CrosSettings* cros_settings_;

  base::WeakPtrFactory<DeviceStatusCollector> weak_factory_;

  
  
  LocationUpdateRequester location_update_requester_;

  
  bool report_version_info_;
  bool report_activity_times_;
  bool report_boot_mode_;
  bool report_location_;
  bool report_network_interfaces_;
  bool report_users_;

  scoped_refptr<Context> context_;

  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      version_info_subscription_;
  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      activity_times_subscription_;
  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      boot_mode_subscription_;
  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      location_subscription_;
  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      network_interfaces_subscription_;
  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      users_subscription_;

  DISALLOW_COPY_AND_ASSIGN(DeviceStatusCollector);
};

}  

#endif  
