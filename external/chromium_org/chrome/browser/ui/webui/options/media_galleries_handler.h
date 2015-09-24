// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_MEDIA_GALLERIES_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_MEDIA_GALLERIES_HANDLER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "content/public/browser/notification_observer.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace options {

class MediaGalleriesHandler
    : public OptionsPageUIHandler,
      public ui::SelectFileDialog::Listener,
      public MediaGalleriesPreferences::GalleryChangeObserver {
 public:
  MediaGalleriesHandler();
  virtual ~MediaGalleriesHandler();

  
  virtual void GetLocalizedValues(base::DictionaryValue* values) OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;

  
  virtual void OnGalleryAdded(MediaGalleriesPreferences* pref,
                              MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryRemoved(MediaGalleriesPreferences* pref,
                                MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryInfoUpdated(MediaGalleriesPreferences* pref,
                                    MediaGalleryPrefId pref_id) OVERRIDE;

 private:
  
  void HandleAddNewGallery(const base::ListValue* args);

  
  void HandleInitializeMediaGalleries(const base::ListValue* args);

  
  
  void HandleForgetGallery(const base::ListValue* args);

  
  void OnGalleriesChanged(MediaGalleriesPreferences* pref);

  
  void PreferencesInitialized();

  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;

  base::WeakPtrFactory<MediaGalleriesHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesHandler);
};

}  

#endif  
