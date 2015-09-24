// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_GEOLOCATION_PROVIDER_IMPL_H_
#define CONTENT_BROWSER_GEOLOCATION_GEOLOCATION_PROVIDER_IMPL_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "content/public/browser/geolocation_provider.h"
#include "content/public/common/geoposition.h"

template<typename Type> struct DefaultSingletonTraits;

namespace content {
class LocationArbitrator;

class CONTENT_EXPORT GeolocationProviderImpl
    : public NON_EXPORTED_BASE(GeolocationProvider),
      public base::Thread {
 public:
  
  virtual void AddLocationUpdateCallback(const LocationUpdateCallback& callback,
                                         bool use_high_accuracy) OVERRIDE;
  virtual bool RemoveLocationUpdateCallback(
      const LocationUpdateCallback& callback) OVERRIDE;
  virtual void UserDidOptIntoLocationServices() OVERRIDE;

  bool LocationServicesOptedIn() const;

  
  
  
  void OverrideLocationForTesting(const Geoposition& override_position);

  
  void OnLocationUpdate(const Geoposition& position);

  
  
  
  
  static GeolocationProviderImpl* GetInstance();

 protected:
  friend struct DefaultSingletonTraits<GeolocationProviderImpl>;
  GeolocationProviderImpl();
  virtual ~GeolocationProviderImpl();

  
  virtual LocationArbitrator* CreateArbitrator();

 private:
  typedef std::pair<LocationUpdateCallback, bool> LocationUpdateInfo;
  typedef std::list<LocationUpdateInfo> CallbackList;

  bool OnGeolocationThread() const;

  
  void OnClientsChanged();

  
  
  
  void StopProviders();

  
  
  void StartProviders(bool use_high_accuracy);

  
  void InformProvidersPermissionGranted();

  
  void NotifyClients(const Geoposition& position);

  
  virtual void Init() OVERRIDE;
  virtual void CleanUp() OVERRIDE;

  
  CallbackList callbacks_;
  bool user_did_opt_into_location_services_;
  Geoposition position_;

  
  bool ignore_location_updates_;

  
  LocationArbitrator* arbitrator_;

  DISALLOW_COPY_AND_ASSIGN(GeolocationProviderImpl);
};

}  

#endif  
