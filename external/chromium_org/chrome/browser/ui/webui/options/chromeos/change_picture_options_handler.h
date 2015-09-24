// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_CHANGE_PICTURE_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_CHANGE_PICTURE_OPTIONS_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/image_decoder.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace chromeos {

class User;

namespace options {

class ChangePictureOptionsHandler : public ::options::OptionsPageUIHandler,
                                    public ui::SelectFileDialog::Listener,
                                    public ImageDecoder::Delegate {
 public:
  ChangePictureOptionsHandler();
  virtual ~ChangePictureOptionsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void SendDefaultImages();

  
  void SendSelectedImage();

  
  
  void SendProfileImage(const gfx::ImageSkia& image, bool should_select);

  
  
  void UpdateProfileImage();

  
  void SendOldImage(const std::string& image_url);

  
  void CheckCameraPresence();

  
  void SetCameraPresent(bool present);

  
  void HandleChooseFile(const base::ListValue* args);

  
  void HandlePhotoTaken(const base::ListValue* args);

  
  void HandleCheckCameraPresence(const base::ListValue* args);

  
  void HandleGetAvailableImages(const base::ListValue* args);

  
  void HandlePageInitialized(const base::ListValue* args);

  
  void HandlePageShown(const base::ListValue* args);

  
  void HandleSelectImage(const base::ListValue* args);

  
  virtual void FileSelected(
      const base::FilePath& path,
      int index, void* params) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnCameraPresenceCheckDone();

  
  void SetImageFromCamera(const gfx::ImageSkia& photo);

  
  gfx::NativeWindow GetBrowserWindow() const;

  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image) OVERRIDE;
  virtual void OnDecodeImageFailed(const ImageDecoder* decoder) OVERRIDE;

  
  
  User* GetUser() const;

  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;

  
  gfx::ImageSkia previous_image_;
  std::string previous_image_url_;

  
  int previous_image_index_;

  
  gfx::ImageSkia user_photo_;

  
  std::string user_photo_data_url_;

  content::NotificationRegistrar registrar_;

  base::WeakPtrFactory<ChangePictureOptionsHandler> weak_factory_;

  
  
  scoped_refptr<ImageDecoder> image_decoder_;

 private:
  
  bool was_camera_present_;

  DISALLOW_COPY_AND_ASSIGN(ChangePictureOptionsHandler);
};

}  
}  

#endif  
