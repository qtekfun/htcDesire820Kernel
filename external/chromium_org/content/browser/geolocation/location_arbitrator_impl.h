// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_LOCATION_ARBITRATOR_IMPL_H_
#define CONTENT_BROWSER_GEOLOCATION_LOCATION_ARBITRATOR_IMPL_H_

#include "base/callback_forward.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "content/browser/geolocation/location_arbitrator.h"
#include "content/common/content_export.h"
#include "content/port/browser/location_provider.h"
#include "content/public/browser/access_token_store.h"
#include "content/public/common/geoposition.h"
#include "net/url_request/url_request_context_getter.h"

namespace net {
class URLRequestContextGetter;
}

namespace content {
class AccessTokenStore;
class LocationProvider;

class CONTENT_EXPORT LocationArbitratorImpl : public LocationArbitrator {
 public:
  
  
  
  static const int64 kFixStaleTimeoutMilliseconds;

  typedef base::Callback<void(const Geoposition&)> LocationUpdateCallback;

  explicit LocationArbitratorImpl(const LocationUpdateCallback& callback);
  virtual ~LocationArbitratorImpl();

  static GURL DefaultNetworkProviderURL();

  
  virtual void StartProviders(bool use_high_accuracy) OVERRIDE;
  virtual void StopProviders() OVERRIDE;
  virtual void OnPermissionGranted() OVERRIDE;
  virtual bool HasPermissionBeenGranted() const OVERRIDE;

 protected:
  AccessTokenStore* GetAccessTokenStore();

  
  
  virtual AccessTokenStore* NewAccessTokenStore();
  virtual LocationProvider* NewNetworkLocationProvider(
      AccessTokenStore* access_token_store,
      net::URLRequestContextGetter* context,
      const GURL& url,
      const base::string16& access_token);
  virtual LocationProvider* NewSystemLocationProvider();
  virtual base::Time GetTimeNow() const;

 private:
  
  
  void RegisterProvider(LocationProvider* provider);
  void OnAccessTokenStoresLoaded(
      AccessTokenStore::AccessTokenSet access_token_store,
      net::URLRequestContextGetter* context_getter);
  void DoStartProviders();

  
  void LocationUpdateAvailable(const LocationProvider* provider,
                               const Geoposition& new_position);

  
  
  
  bool IsNewPositionBetter(const Geoposition& old_position,
                           const Geoposition& new_position,
                           bool from_same_provider) const;

  scoped_refptr<AccessTokenStore> access_token_store_;
  LocationUpdateCallback callback_;
  LocationProvider::LocationProviderUpdateCallback provider_callback_;
  ScopedVector<LocationProvider> providers_;
  bool use_high_accuracy_;
  
  const LocationProvider* position_provider_;
  bool is_permission_granted_;
  
  Geoposition position_;

  
  bool is_running_;

  DISALLOW_COPY_AND_ASSIGN(LocationArbitratorImpl);
};

LocationProvider* NewSystemLocationProvider();

}  

#endif  
