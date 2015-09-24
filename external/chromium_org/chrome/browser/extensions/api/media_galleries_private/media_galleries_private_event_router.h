// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_MEDIA_GALLERIES_PRIVATE_EVENT_ROUTER_H_
#define CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_MEDIA_GALLERIES_PRIVATE_EVENT_ROUTER_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "chrome/browser/storage_monitor/removable_storage_observer.h"

class Profile;

namespace base {
class ListValue;
}

namespace extensions {

class MediaGalleriesPrivateEventRouter
    : public RemovableStorageObserver,
      public base::SupportsWeakPtr<MediaGalleriesPrivateEventRouter> {
 public:
  explicit MediaGalleriesPrivateEventRouter(Profile* profile);
  virtual ~MediaGalleriesPrivateEventRouter();

  
  void OnGalleryChanged(MediaGalleryPrefId gallery_id,
                        const std::set<std::string>& extension_ids);

 private:
  
  virtual void OnRemovableStorageAttached(const StorageInfo& info) OVERRIDE;
  virtual void OnRemovableStorageDetached(const StorageInfo& info) OVERRIDE;

  void DispatchEvent(const std::string& event_name,
                     scoped_ptr<base::ListValue> event_args);

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesPrivateEventRouter);
};

}  

#endif  
