// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_MEDIA_GALLERIES_DIALOG_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_MEDIA_GALLERIES_DIALOG_COCOA_H_

#import <Cocoa/Cocoa.h>

#include "base/gtest_prod_util.h"
#include "chrome/browser/media_galleries/media_galleries_dialog_controller.h"
#import "chrome/browser/ui/cocoa/constrained_window/constrained_window_mac.h"

@class ConstrainedWindowAlert;
@class MediaGalleriesCocoaController;

class MediaGalleriesDialogBrowserTest;
class MediaGalleriesDialogTest;

namespace ui {
class MenuModel;
}

class MediaGalleriesDialogCocoa : public ConstrainedWindowMacDelegate,
                                  public MediaGalleriesDialog {
 public:
  MediaGalleriesDialogCocoa(
      MediaGalleriesDialogController* controller,
      MediaGalleriesCocoaController* delegate);
  virtual ~MediaGalleriesDialogCocoa();

  
  void OnAcceptClicked();
  
  void OnCancelClicked();
  
  void OnAddFolderClicked();
  
  void OnCheckboxToggled(NSButton* checkbox);

  
  virtual void UpdateGalleries() OVERRIDE;

  
  virtual void OnConstrainedWindowClosed(
      ConstrainedWindowMac* window) OVERRIDE;

  ui::MenuModel* GetContextMenuModel(MediaGalleryPrefId prefid);

 private:
  FRIEND_TEST_ALL_PREFIXES(MediaGalleriesDialogBrowserTest, Close);
  FRIEND_TEST_ALL_PREFIXES(MediaGalleriesDialogTest, InitializeCheckboxes);
  FRIEND_TEST_ALL_PREFIXES(MediaGalleriesDialogTest, ToggleCheckboxes);
  FRIEND_TEST_ALL_PREFIXES(MediaGalleriesDialogTest, UpdateAdds);
  FRIEND_TEST_ALL_PREFIXES(MediaGalleriesDialogTest, ForgetDeletes);

  void UpdateGalleryCheckbox(const MediaGalleryPrefInfo& gallery,
                             bool permitted,
                             CGFloat y_pos);

  void InitDialogControls();
  CGFloat CreateAddFolderButton();
  CGFloat CreateAttachedCheckboxes(
      CGFloat y_pos,
      const MediaGalleriesDialogController::GalleryPermissionsVector&
          permissions);
  CGFloat CreateUnattachedCheckboxes(
      CGFloat y_pos,
      const MediaGalleriesDialogController::GalleryPermissionsVector&
          permissions);
  CGFloat CreateCheckboxSeparator(CGFloat y_pos);

  MediaGalleriesDialogController* controller_;  
  scoped_ptr<ConstrainedWindowMac> window_;

  
  base::scoped_nsobject<ConstrainedWindowAlert> alert_;

  
  bool accepted_;

  
  base::scoped_nsobject<NSMutableArray> checkboxes_;

  
  base::scoped_nsobject<NSView> checkbox_container_;

  
  base::scoped_nsobject<NSBox> accessory_;

  
  base::scoped_nsobject<MediaGalleriesCocoaController> cocoa_controller_;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesDialogCocoa);
};

#endif  
