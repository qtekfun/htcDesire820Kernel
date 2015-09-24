// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_LOCATION_LOCATION_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_API_LOCATION_LOCATION_MANAGER_H_

#include <map>
#include <string>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace content {
struct Geoposition;
}  

namespace extensions {
class LocationManager;
class LocationRequest;

namespace api {
namespace location {

struct Coordinates;

}  
}  

class LocationManager
    : public ProfileKeyedAPI,
      public content::NotificationObserver,
      public base::SupportsWeakPtr<LocationManager> {
 public:
  explicit LocationManager(Profile* profile);
  virtual ~LocationManager();

  
  
  void AddLocationRequest(
      const std::string& extension_id,
      const std::string& request_name,
      const double* distance_update_threshold_meters,
      const double* time_between_updates_ms);

  
  
  void RemoveLocationRequest(const std::string& extension_id,
                             const std::string& name);

  
  static ProfileKeyedAPIFactory<LocationManager>* GetFactoryInstance();

  
  static LocationManager* Get(Profile* profile);

 private:
  friend class LocationRequest;
  friend class ProfileKeyedAPIFactory<LocationManager>;

  typedef std::string ExtensionId;
  typedef scoped_refptr<LocationRequest> LocationRequestPointer;
  typedef std::multimap<ExtensionId, LocationRequestPointer> LocationRequestMap;
  typedef LocationRequestMap::iterator LocationRequestIterator;

  
  
  static void GeopositionToApiCoordinates(
      const content::Geoposition& position,
      api::location::Coordinates* coordinates);

  
  void SendLocationUpdate(const std::string& extension_id,
                          const std::string& request_name,
                          const content::Geoposition& position);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  static const char* service_name() { return "LocationManager"; }

  
  Profile* const profile_;

  
  
  LocationRequestMap location_requests_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(LocationManager);
};

}  

#endif  
