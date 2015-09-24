// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_GALLERIES_DIALOG_CONTROLLER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_GALLERIES_DIALOG_CONTROLLER_H_

#include <list>
#include <map>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "chrome/browser/storage_monitor/removable_storage_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

namespace ui {
class MenuModel;
}

class GalleryContextMenuModel;
class MediaGalleriesDialogController;
class Profile;

class MediaGalleriesDialog {
 public:
  virtual ~MediaGalleriesDialog();

  
  virtual void UpdateGalleries() = 0;

  
  static MediaGalleriesDialog* Create(
      MediaGalleriesDialogController* controller);
};

class MediaGalleriesDialogController
    : public ui::SelectFileDialog::Listener,
      public RemovableStorageObserver,
      public MediaGalleriesPreferences::GalleryChangeObserver {
 public:
  struct GalleryPermission {
    GalleryPermission(const MediaGalleryPrefInfo& pref_info, bool allowed)
        : pref_info(pref_info), allowed(allowed) {}
    GalleryPermission() {}

    MediaGalleryPrefInfo pref_info;
    bool allowed;
  };

  typedef std::vector<GalleryPermission> GalleryPermissionsVector;

  
  MediaGalleriesDialogController(content::WebContents* web_contents,
                                 const extensions::Extension& extension,
                                 const base::Closure& on_finish);

  
  base::string16 GetHeader() const;

  
  base::string16 GetSubtext() const;

  
  base::string16 GetUnattachedLocationsHeader() const;

  
  bool HasPermittedGalleries() const;

  
  virtual GalleryPermissionsVector AttachedPermissions() const;

  
  virtual GalleryPermissionsVector UnattachedPermissions() const;

  
  virtual void OnAddFolderClicked();

  
  
  virtual void DidToggleGalleryId(MediaGalleryPrefId pref_id,
                                  bool enabled);
  virtual void DidToggleNewGallery(const MediaGalleryPrefInfo& gallery,
                                   bool enabled);

  
  virtual void DidForgetGallery(MediaGalleryPrefId pref_id);

  
  virtual void DialogFinished(bool accepted);

  virtual content::WebContents* web_contents();

  ui::MenuModel* GetContextMenuModel(MediaGalleryPrefId id);

 protected:
  
  explicit MediaGalleriesDialogController(
      const extensions::Extension& extension);

  virtual ~MediaGalleriesDialogController();

 private:
  
  typedef std::map<MediaGalleryPrefId, GalleryPermission>
      KnownGalleryPermissions;

  
  void OnPreferencesInitialized();

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;

  
  
  virtual void OnRemovableStorageAttached(const StorageInfo& info) OVERRIDE;
  virtual void OnRemovableStorageDetached(const StorageInfo& info) OVERRIDE;

  
  
  virtual void OnPermissionAdded(MediaGalleriesPreferences* pref,
                                 const std::string& extension_id,
                                 MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnPermissionRemoved(MediaGalleriesPreferences* pref,
                                   const std::string& extension_id,
                                   MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryAdded(MediaGalleriesPreferences* pref,
                              MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryRemoved(MediaGalleriesPreferences* pref,
                                MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryInfoUpdated(MediaGalleriesPreferences* pref,
                                    MediaGalleryPrefId pref_id) OVERRIDE;

  
  
  
  void InitializePermissions();

  
  void SavePermissions();

  
  
  
  void UpdateGalleriesOnPreferencesEvent();

  
  void UpdateGalleriesOnDeviceEvent(const std::string& device_id);

  
  
  void FillPermissions(bool attached,
                       GalleryPermissionsVector* permissions) const;

  Profile* GetProfile();

  
  content::WebContents* web_contents_;

  
  
  const extensions::Extension* extension_;

  
  
  KnownGalleryPermissions known_galleries_;

  
  MediaGalleryPrefIdSet toggled_galleries_;

  
  
  GalleryPermissionsVector new_galleries_;

  
  base::Closure on_finish_;

  
  
  MediaGalleriesPreferences* preferences_;

  
  scoped_ptr<MediaGalleriesDialog> dialog_;

  scoped_refptr<ui::SelectFileDialog> select_folder_dialog_;

  scoped_ptr<ui::MenuModel> context_menu_model_;
  scoped_ptr<GalleryContextMenuModel> gallery_menu_model_;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesDialogController);
};

#endif  
