// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DESKTOP_BACKGROUND_DESKTOP_BACKGROUND_CONTROLLER_H_
#define ASH_DESKTOP_BACKGROUND_DESKTOP_BACKGROUND_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/display/display_controller.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/image/image_skia.h"

typedef unsigned int SkColor;

class CommandLine;

namespace aura {
class Window;
}

namespace ash {

enum WallpaperLayout {
  
  
  WALLPAPER_LAYOUT_CENTER,
  
  
  WALLPAPER_LAYOUT_CENTER_CROPPED,
  
  
  WALLPAPER_LAYOUT_STRETCH,
  
  WALLPAPER_LAYOUT_TILE,
};

enum WallpaperResolution {
  WALLPAPER_RESOLUTION_LARGE,
  WALLPAPER_RESOLUTION_SMALL
};

const SkColor kLoginWallpaperColor = 0xFEFEFE;

ASH_EXPORT extern const int kSmallWallpaperMaxWidth;
ASH_EXPORT extern const int kSmallWallpaperMaxHeight;
ASH_EXPORT extern const int kLargeWallpaperMaxWidth;
ASH_EXPORT extern const int kLargeWallpaperMaxHeight;

ASH_EXPORT extern const int kWallpaperThumbnailWidth;
ASH_EXPORT extern const int kWallpaperThumbnailHeight;

class DesktopBackgroundControllerObserver;
class WallpaperResizer;

class ASH_EXPORT DesktopBackgroundController
    : public DisplayController::Observer {
 public:
  enum BackgroundMode {
    BACKGROUND_NONE,
    BACKGROUND_IMAGE,
  };

  DesktopBackgroundController();
  virtual ~DesktopBackgroundController();

  BackgroundMode desktop_background_mode() const {
    return desktop_background_mode_;
  }

  void set_command_line_for_testing(CommandLine* command_line) {
    command_line_for_testing_ = command_line;
  }

  
  void AddObserver(DesktopBackgroundControllerObserver* observer);
  void RemoveObserver(DesktopBackgroundControllerObserver* observer);

  
  
  gfx::ImageSkia GetWallpaper() const;

  WallpaperLayout GetWallpaperLayout() const;

  
  void OnRootWindowAdded(aura::Window* root_window);

  
  
  
  
  bool SetDefaultWallpaper(bool is_guest);

  
  
  void SetCustomWallpaper(const gfx::ImageSkia& image, WallpaperLayout layout);

  
  void CancelPendingWallpaperOperation();

  
  
  
  
  
  
  void CreateEmptyWallpaper();

  
  WallpaperResolution GetAppropriateResolution();

  
  
  bool MoveDesktopToLockedContainer();

  
  
  bool MoveDesktopToUnlockedContainer();

  
  virtual void OnDisplayConfigurationChanged() OVERRIDE;

 private:
  friend class DesktopBackgroundControllerTest;
  FRIEND_TEST_ALL_PREFIXES(DesktopBackgroundControllerTest, GetMaxDisplaySize);

  
  class WallpaperLoader;

  
  
  bool DefaultWallpaperIsAlreadyLoadingOrLoaded(
      const base::FilePath& image_file, int image_resource_id) const;

  
  
  bool CustomWallpaperIsAlreadyLoaded(const gfx::ImageSkia& image) const;

  
  
  void SetDesktopBackgroundImageMode();

  
  
  void OnDefaultWallpaperLoadCompleted(scoped_refptr<WallpaperLoader> loader);

  
  
  void InstallDesktopController(aura::Window* root_window);

  
  
  void InstallDesktopControllerForAllWindows();

  
  
  bool ReparentBackgroundWidgets(int src_container, int dst_container);

  
  int GetBackgroundContainerId(bool locked);

  
  void NotifyAnimationFinished();

  
  void UpdateWallpaper();

  void set_wallpaper_reload_delay_for_test(bool value) {
    wallpaper_reload_delay_ = value;
  }

  
  
  
  
  static gfx::Size GetMaxDisplaySizeInNative();

  
  CommandLine* command_line_for_testing_;

  
  bool locked_;

  BackgroundMode desktop_background_mode_;

  SkColor background_color_;

  ObserverList<DesktopBackgroundControllerObserver> observers_;

  
  scoped_ptr<WallpaperResizer> current_wallpaper_;

  
  
  
  base::FilePath current_default_wallpaper_path_;
  int current_default_wallpaper_resource_id_;

  gfx::Size current_max_display_size_;

  scoped_refptr<WallpaperLoader> wallpaper_loader_;

  base::WeakPtrFactory<DesktopBackgroundController> weak_ptr_factory_;

  base::OneShotTimer<DesktopBackgroundController> timer_;

  int wallpaper_reload_delay_;

  DISALLOW_COPY_AND_ASSIGN(DesktopBackgroundController);
};

}  

#endif  
