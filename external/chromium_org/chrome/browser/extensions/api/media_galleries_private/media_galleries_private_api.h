// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_MEDIA_GALLERIES_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_PRIVATE_MEDIA_GALLERIES_PRIVATE_API_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/api/media_galleries_private/gallery_watch_state_tracker.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "chrome/common/extensions/api/media_galleries_private.h"
#include "extensions/browser/event_router.h"

class Profile;

namespace extensions {

class MediaGalleriesPrivateEventRouter;

class MediaGalleriesPrivateAPI : public ProfileKeyedAPI,
                                 public EventRouter::Observer {
 public:
  explicit MediaGalleriesPrivateAPI(Profile* profile);
  virtual ~MediaGalleriesPrivateAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<MediaGalleriesPrivateAPI>* GetFactoryInstance();

  
  static MediaGalleriesPrivateAPI* Get(Profile* profile);

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;

  MediaGalleriesPrivateEventRouter* GetEventRouter();
  GalleryWatchStateTracker* GetGalleryWatchStateTracker();

 private:
  friend class ProfileKeyedAPIFactory<MediaGalleriesPrivateAPI>;

  void MaybeInitializeEventRouterAndTracker();

  
  static const char* service_name() {
    return "MediaGalleriesPrivateAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  
  Profile* profile_;

  
  scoped_ptr<GalleryWatchStateTracker> tracker_;

  
  scoped_ptr<MediaGalleriesPrivateEventRouter>
      media_galleries_private_event_router_;

  base::WeakPtrFactory<MediaGalleriesPrivateAPI> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesPrivateAPI);
};

class MediaGalleriesPrivateAddGalleryWatchFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleriesPrivate.addGalleryWatch",
                             MEDIAGALLERIESPRIVATE_ADDGALLERYWATCH);

 protected:
  virtual ~MediaGalleriesPrivateAddGalleryWatchFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnPreferencesInit(const std::string& pref_id);

  
  void HandleResponse(MediaGalleryPrefId gallery_id, bool success);
};

class MediaGalleriesPrivateRemoveGalleryWatchFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleriesPrivate.removeGalleryWatch",
                             MEDIAGALLERIESPRIVATE_REMOVEGALLERYWATCH);

 protected:
  virtual ~MediaGalleriesPrivateRemoveGalleryWatchFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnPreferencesInit(const std::string& pref_id);
};

class MediaGalleriesPrivateGetAllGalleryWatchFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleriesPrivate.getAllGalleryWatch",
                             MEDIAGALLERIESPRIVATE_GETALLGALLERYWATCH);
 protected:
  virtual ~MediaGalleriesPrivateGetAllGalleryWatchFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnPreferencesInit();
};

class MediaGalleriesPrivateRemoveAllGalleryWatchFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleriesPrivate.removeAllGalleryWatch",
                             MEDIAGALLERIESPRIVATE_REMOVEALLGALLERYWATCH);
 protected:
  virtual ~MediaGalleriesPrivateRemoveAllGalleryWatchFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void OnPreferencesInit();
};

class MediaGalleriesPrivateGetHandlersFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleriesPrivate.getHandlers",
                             MEDIAGALLERIESPRIVATE_GETHANDLERS);

 protected:
  virtual ~MediaGalleriesPrivateGetHandlersFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
