// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_GALLERY_WATCH_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_GALLERY_WATCH_MANAGER_H_

#include <map>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"

namespace extensions {

class MediaGalleriesPrivateEventRouter;

class GalleryWatchManager {
 public:
  
  
  static GalleryWatchManager* GetForProfile(void* profile_id);

  
  
  static bool HasForProfile(void* profile_id);

  
  static void OnProfileShutdown(void* profile_id);

  
  
  
  
  
  static bool SetupGalleryWatch(
      void* profile_id,
      MediaGalleryPrefId gallery_id,
      const base::FilePath& watch_path,
      const std::string& extension_id,
      base::WeakPtr<MediaGalleriesPrivateEventRouter> event_router);

  
  
  
  static void RemoveGalleryWatch(void* profile_id,
                                 const base::FilePath& watch_path,
                                 const std::string& extension_id);

  
  static void OnExtensionUnloaded(void* profile_id,
                                  const std::string& extension_id);

 private:
  class GalleryFilePathWatcher;
  typedef std::map<base::FilePath, GalleryFilePathWatcher*> WatcherMap;

  
  GalleryWatchManager();
  ~GalleryWatchManager();

  
  
  
  
  bool StartGalleryWatch(
      MediaGalleryPrefId gallery_id,
      const base::FilePath& watch_path,
      const std::string& extension_id,
      base::WeakPtr<MediaGalleriesPrivateEventRouter> event_router);

  
  
  void StopGalleryWatch(const base::FilePath& watch_path,
                        const std::string& extension_id);

  
  void HandleExtensionUnloadedEvent(const std::string& extension_id);

  
  void DeleteAllWatchers();

  
  
  void RemoveGalleryFilePathWatcherEntry(const base::FilePath& watch_path);

  
  
  
  WatcherMap gallery_watchers_;

  DISALLOW_COPY_AND_ASSIGN(GalleryWatchManager);
};

}  

#endif  
