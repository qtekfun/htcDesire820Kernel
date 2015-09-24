// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_GALLERY_WATCH_STATE_TRACKER_H_
#define CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_GALLERY_WATCH_STATE_TRACKER_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace base {
class Value;
}

namespace extensions {

class GalleryWatchStateTracker
    : public content::NotificationObserver,
      public base::SupportsWeakPtr<GalleryWatchStateTracker>,
      public MediaGalleriesPreferences::GalleryChangeObserver {
 public:
  explicit GalleryWatchStateTracker(Profile* profile);
  virtual ~GalleryWatchStateTracker();

  
  
  static GalleryWatchStateTracker* GetForProfile(Profile* profile);

  
  
  void OnGalleryWatchAdded(const std::string& extension_id,
                           MediaGalleryPrefId gallery_id);

  
  
  void OnGalleryWatchRemoved(const std::string& extension_id,
                             MediaGalleryPrefId gallery_id);

  
  
  
  virtual void OnPermissionAdded(MediaGalleriesPreferences* pref,
                                 const std::string& extension_id,
                                 MediaGalleryPrefId gallery_id) OVERRIDE;

  virtual void OnPermissionRemoved(MediaGalleriesPreferences* pref,
                                   const std::string& extension_id,
                                   MediaGalleryPrefId gallery_id) OVERRIDE;

  virtual void OnGalleryRemoved(MediaGalleriesPreferences* pref,
                                MediaGalleryPrefId gallery_id) OVERRIDE;

  
  
  MediaGalleryPrefIdSet GetAllWatchedGalleryIDsForExtension(
      const std::string& extension_id) const;

  
  
  void RemoveAllGalleryWatchersForExtension(
      const std::string& extension_id,
      MediaGalleriesPreferences* preferences);

 private:
  
  
  
  
  typedef std::map<MediaGalleryPrefId, bool> WatchedGalleriesMap;

  
  
  typedef std::map<std::string, WatchedGalleriesMap> WatchedExtensionsMap;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void WriteToStorage(const std::string& extension_id);
  void ReadFromStorage(const std::string& extension_id,
                       scoped_ptr<base::Value> value);

  
  
  void SetupGalleryWatch(const std::string& extension_id,
                         MediaGalleryPrefId gallery_id,
                         MediaGalleriesPreferences* preferences);

  
  
  void RemoveGalleryWatch(const std::string& extension_id,
                          MediaGalleryPrefId gallery_id,
                          MediaGalleriesPreferences* preferences);

  
  
  bool HasGalleryWatchInfo(const std::string& extension_id,
                           MediaGalleryPrefId gallery_id,
                           bool has_active_watcher);

  
  
  
  void HandleSetupGalleryWatchResponse(const std::string& extension_id,
                                       MediaGalleryPrefId gallery_id,
                                       bool success);

  
  
  
  
  bool AddWatchedGalleryIdInfoForExtension(const std::string& extension_id,
                                           MediaGalleryPrefId gallery_id);

  
  Profile* profile_;

  
  
  content::NotificationRegistrar registrar_;

  
  WatchedExtensionsMap watched_extensions_map_;

  DISALLOW_COPY_AND_ASSIGN(GalleryWatchStateTracker);
};

}  

#endif  
