// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_NETWORK_LOCATION_PROVIDER_H_
#define CONTENT_BROWSER_GEOLOCATION_NETWORK_LOCATION_PROVIDER_H_

#include <list>
#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/thread.h"
#include "content/browser/geolocation/location_provider_base.h"
#include "content/browser/geolocation/network_location_request.h"
#include "content/browser/geolocation/wifi_data_provider.h"
#include "content/common/content_export.h"
#include "content/public/common/geoposition.h"

namespace content {
class AccessTokenStore;


class NetworkLocationProvider
    : public base::NonThreadSafe,
      public LocationProviderBase {
 public:
  
  class CONTENT_EXPORT PositionCache {
   public:
    
    static const size_t kMaximumSize;

    PositionCache();
    ~PositionCache();

    
    
    
    
    bool CachePosition(const WifiData& wifi_data,
                       const Geoposition& position);

    
    
    
    const Geoposition* FindPosition(const WifiData& wifi_data);

   private:
    
    
    static bool MakeKey(const WifiData& wifi_data,
                        base::string16* key);

    
    
    
    typedef std::map<base::string16, Geoposition> CacheMap;
    CacheMap cache_;
    typedef std::list<CacheMap::iterator> CacheAgeList;
    CacheAgeList cache_age_list_;  
  };

  NetworkLocationProvider(AccessTokenStore* access_token_store,
                          net::URLRequestContextGetter* context,
                          const GURL& url,
                          const base::string16& access_token);
  virtual ~NetworkLocationProvider();

  
  virtual bool StartProvider(bool high_accuracy) OVERRIDE;
  virtual void StopProvider() OVERRIDE;
  virtual void GetPosition(Geoposition *position) OVERRIDE;
  virtual void RequestRefresh() OVERRIDE;
  virtual void OnPermissionGranted() OVERRIDE;

 private:
  
  void RequestPosition();

  
  void WifiDataUpdateAvailable(WifiDataProvider* provider);

  
  void OnWifiDataUpdated();

  bool IsStarted() const;

  void LocationResponseAvailable(const Geoposition& position,
                                 bool server_error,
                                 const base::string16& access_token,
                                 const WifiData& wifi_data);

  scoped_refptr<AccessTokenStore> access_token_store_;

  
  WifiDataProvider* wifi_data_provider_;

  WifiDataProvider::WifiDataUpdateCallback wifi_data_update_callback_;

  
  WifiData wifi_data_;
  bool is_wifi_data_complete_;

  
  base::Time wifi_data_updated_timestamp_;

  
  
  base::string16 access_token_;

  
  Geoposition position_;

  
  bool is_permission_granted_;

  bool is_new_data_available_;

  
  scoped_ptr<NetworkLocationRequest> request_;

  
  scoped_ptr<PositionCache> position_cache_;

  base::WeakPtrFactory<NetworkLocationProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkLocationProvider);
};

CONTENT_EXPORT LocationProviderBase* NewNetworkLocationProvider(
    AccessTokenStore* access_token_store,
    net::URLRequestContextGetter* context,
    const GURL& url,
    const base::string16& access_token);

}  

#endif  
