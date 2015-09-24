// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_FILE_SYSTEM_REGISTRY_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_FILE_SYSTEM_REGISTRY_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "chrome/browser/storage_monitor/removable_storage_observer.h"

class ExtensionGalleriesHost;
class MediaFileSystemContext;
class MediaGalleriesPreferences;
class Profile;

namespace content {
class RenderViewHost;
}

namespace extensions {
class Extension;
}

namespace fileapi {
class IsolatedContext;
}

struct MediaFileSystemInfo {
  MediaFileSystemInfo(const base::string16& fs_name,
                      const base::FilePath& fs_path,
                      const std::string& filesystem_id,
                      MediaGalleryPrefId pref_id,
                      const std::string& transient_device_id,
                      bool removable,
                      bool media_device);
  MediaFileSystemInfo();
  ~MediaFileSystemInfo();

  base::string16 name;
  base::FilePath path;
  std::string fsid;
  MediaGalleryPrefId pref_id;
  std::string transient_device_id;
  bool removable;
  bool media_device;
};

typedef base::Callback<void(const std::vector<MediaFileSystemInfo>&)>
    MediaFileSystemsCallback;

class MediaFileSystemRegistry
    : public RemovableStorageObserver,
      public MediaGalleriesPreferences::GalleryChangeObserver {
 public:
  MediaFileSystemRegistry();
  virtual ~MediaFileSystemRegistry();

  
  
  void GetMediaFileSystemsForExtension(
      const content::RenderViewHost* rvh,
      const extensions::Extension* extension,
      const MediaFileSystemsCallback& callback);

  
  
  
  MediaGalleriesPreferences* GetPreferences(Profile* profile);

  
  virtual void OnRemovableStorageDetached(const StorageInfo& info) OVERRIDE;

 private:
  class MediaFileSystemContextImpl;

  friend class MediaFileSystemContextImpl;
  friend class MediaFileSystemRegistryTest;
  friend class TestMediaFileSystemContext;

  
  typedef std::map<std::string ,
                   scoped_refptr<ExtensionGalleriesHost> > ExtensionHostMap;
  
  typedef std::map<Profile*, ExtensionHostMap> ExtensionGalleriesHostMap;

  virtual void OnPermissionRemoved(MediaGalleriesPreferences* pref,
                                   const std::string& extension_id,
                                   MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryRemoved(MediaGalleriesPreferences* pref,
                                MediaGalleryPrefId pref_id) OVERRIDE;

  void OnExtensionGalleriesHostEmpty(Profile* profile,
                                     const std::string& extension_id);

  
  ExtensionGalleriesHostMap extension_hosts_map_;

  scoped_ptr<MediaFileSystemContext> file_system_context_;

  DISALLOW_COPY_AND_ASSIGN(MediaFileSystemRegistry);
};

#endif  
