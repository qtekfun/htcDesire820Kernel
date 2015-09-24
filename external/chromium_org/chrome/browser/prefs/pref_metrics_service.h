// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_METRICS_SERVICE_H_
#define CHROME_BROWSER_PREFS_PREF_METRICS_SERVICE_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/prefs/synced_pref_change_registrar.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class PrefRegistrySimple;

class PrefMetricsService : public BrowserContextKeyedService {
 public:
  enum HashedPrefStyle {
    HASHED_PREF_STYLE_NEW,
    HASHED_PREF_STYLE_DEPRECATED,
  };

  explicit PrefMetricsService(Profile* profile);
  virtual ~PrefMetricsService();

  class Factory : public BrowserContextKeyedServiceFactory {
   public:
    static Factory* GetInstance();
    static PrefMetricsService* GetForProfile(Profile* profile);
   private:
    friend struct DefaultSingletonTraits<Factory>;

    Factory();
    virtual ~Factory();

    
    virtual BrowserContextKeyedService* BuildServiceInstanceFor(
        content::BrowserContext* profile) const OVERRIDE;
    virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;
    virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
    virtual content::BrowserContext* GetBrowserContextToUse(
        content::BrowserContext* context) const OVERRIDE;
  };

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  friend class PrefMetricsServiceTest;

  
  typedef base::Callback<void(const std::string&, const Value*)>
      LogHistogramValueCallback;

  
  PrefMetricsService(Profile* profile,
                     PrefService* local_settings,
                     const std::string& device_id,
                     const char** tracked_pref_paths,
                     int tracked_pref_path_count);

  
  void RecordLaunchPrefs();

  
  void RegisterSyncedPrefObservers();

  
  void AddPrefObserver(const std::string& path,
                       const std::string& histogram_name_prefix,
                       const LogHistogramValueCallback& callback);

  
  void OnPrefChanged(const std::string& histogram_name_prefix,
                     const LogHistogramValueCallback& callback,
                     const std::string& path,
                     bool from_sync);

  
  void LogBooleanPrefChange(const std::string& histogram_name,
                            const Value* value);

  
  void LogIntegerPrefChange(int boundary_value,
                            const std::string& histogram_name,
                            const Value* value);

  
  void GetDeviceIdCallback(const std::string& device_id);

  
  
  void CheckTrackedPreferences();

  
  
  void UpdateTrackedPreference(const char* path);

  
  
  
  std::string GetHashedPrefValue(
      const char* path,
      const base::Value* value,
      HashedPrefStyle desired_style);

  void InitializePrefObservers();

  Profile* profile_;
  PrefService* prefs_;
  PrefService* local_state_;
  std::string profile_name_;
  std::string pref_hash_seed_;
  std::string device_id_;
  const char** tracked_pref_paths_;
  const int tracked_pref_path_count_;

  
  
  bool checked_tracked_prefs_;

  PrefChangeRegistrar pref_registrar_;
  scoped_ptr<SyncedPrefChangeRegistrar> synced_pref_change_registrar_;

  base::WeakPtrFactory<PrefMetricsService> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PrefMetricsService);
};

#endif  
