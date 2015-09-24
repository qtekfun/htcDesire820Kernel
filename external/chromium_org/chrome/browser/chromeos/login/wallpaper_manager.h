// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WALLPAPER_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WALLPAPER_MANAGER_H_

#include <string>

#include "ash/desktop_background/desktop_background_controller.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted_memory.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/login/user.h"
#include "chrome/browser/chromeos/login/user_image.h"
#include "chrome/browser/chromeos/login/user_image_loader.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/icu/source/i18n/unicode/timezone.h"
#include "ui/gfx/image/image_skia.h"

class CommandLine;
class PrefRegistrySimple;

namespace base {
class SequencedTaskRunner;
}

namespace chromeos {

struct WallpaperInfo {
  
  std::string file;
  ash::WallpaperLayout layout;
  User::WallpaperType type;
  base::Time date;
  bool operator==(const WallpaperInfo& other) {
    return (file == other.file) && (layout == other.layout) &&
        (type == other.type);
  }
};

class WallpaperManagerBrowserTest;
class UserImage;

extern const char kWallpaperSequenceTokenName[];

extern const char kSmallWallpaperSuffix[];
extern const char kLargeWallpaperSuffix[];

extern const char kSmallWallpaperSubDir[];
extern const char kLargeWallpaperSubDir[];
extern const char kOriginalWallpaperSubDir[];
extern const char kThumbnailWallpaperSubDir[];

class WallpaperManager: public content::NotificationObserver {
 public:
  
  class TestApi {
   public:
    explicit TestApi(WallpaperManager* wallpaper_manager);
    virtual ~TestApi();

    base::FilePath current_wallpaper_path();

   private:
    WallpaperManager* wallpaper_manager_;  

    DISALLOW_COPY_AND_ASSIGN(TestApi);
  };

  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnWallpaperAnimationFinished(const std::string& email) = 0;
  };

  static WallpaperManager* Get();

  WallpaperManager();
  virtual ~WallpaperManager();

  void set_command_line_for_testing(CommandLine* command_line) {
    command_line_for_testing_ = command_line;
  }

  
  
  void Shutdown();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  void AddObservers();

  
  
  void EnsureLoggedInUserWallpaperLoaded();

  
  void ClearWallpaperCache();

  
  
  base::FilePath GetCustomWallpaperPath(const char* sub_dir,
                                        const std::string& user_id_hash,
                                        const std::string& file);

  
  bool GetWallpaperFromCache(const std::string& email,
                             gfx::ImageSkia* wallpaper);

  
  base::FilePath GetOriginalWallpaperPathForUser(const std::string& username);

  
  bool GetLoggedInUserWallpaperInfo(WallpaperInfo* info);

  
  
  
  void InitializeWallpaper();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void RemoveUserWallpaperInfo(const std::string& email);

  
  
  bool ResizeWallpaper(const UserImage& wallpaper,
                       ash::WallpaperLayout layout,
                       int preferred_width,
                       int preferred_height,
                       scoped_refptr<base::RefCountedBytes>* output);

  
  
  
  void ResizeAndSaveWallpaper(const UserImage& wallpaper,
                              const base::FilePath& path,
                              ash::WallpaperLayout layout,
                              int preferred_width,
                              int preferred_height);

  
  
  void SetCustomWallpaper(const std::string& username,
                          const std::string& user_id_hash,
                          const std::string& file,
                          ash::WallpaperLayout layout,
                          User::WallpaperType type,
                          const UserImage& wallpaper);

  
  void SetDefaultWallpaper();

  
  
  void SetInitialUserWallpaper(const std::string& username, bool is_persistent);

  
  
  void SetUserWallpaperInfo(const std::string& username,
                            const WallpaperInfo& info,
                            bool is_persistent);

  
  void SetLastSelectedUser(const std::string& last_selected_user);

  
  void SetUserWallpaper(const std::string& email);

  
  void SetWallpaperFromImageSkia(const gfx::ImageSkia& wallpaper,
                                 ash::WallpaperLayout layout);

  
  
  void UpdateWallpaper();

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

 private:
  friend class TestApi;
  friend class WallpaperManagerBrowserTest;
  typedef std::map<std::string, gfx::ImageSkia> CustomWallpaperMap;

  
  int loaded_wallpapers() const { return loaded_wallpapers_; }

  
  
  
  
  
  
  
  
  
  
  void CacheUsersWallpapers();

  
  void CacheUserWallpaper(const std::string& email);

  
  void ClearObsoleteWallpaperPrefs();

  
  void DeleteAllExcept(const base::FilePath& path);

  
  void DeleteWallpaperInList(const std::vector<base::FilePath>& file_list);

  
  void DeleteUserWallpapers(const std::string& email,
                            const std::string& path_to_file);

  
  
  void EnsureCustomWallpaperDirectories(const std::string& user_id_hash);

  
  CommandLine* GetComandLine();

  
  
  void InitializeRegisteredDeviceWallpaper();

  
  
  void LoadWallpaper(const std::string& email,
                     const WallpaperInfo& info,
                     bool update_wallpaper);

  
  
  void MoveCustomWallpapersOnWorker(const std::string& email,
                                    const std::string& user_id_hash);

  
  
  void MoveCustomWallpapersSuccess(const std::string& email,
                                   const std::string& user_id_hash);

  
  
  
  
  void MoveLoggedInUserCustomWallpaper();

  
  
  
  void GetCustomWallpaperInternal(const std::string& email,
                                  const WallpaperInfo& info,
                                  const base::FilePath& wallpaper_path,
                                  bool update_wallpaper);

  
  
  bool GetUserWallpaperInfo(const std::string& email, WallpaperInfo* info);

  
  
  void OnWallpaperDecoded(const std::string& email,
                          ash::WallpaperLayout layout,
                          bool update_wallpaper,
                          const UserImage& wallpaper);

  
  
  
  void ProcessCustomWallpaper(const std::string& user_id_hash,
                              bool persistent,
                              const WallpaperInfo& info,
                              scoped_ptr<gfx::ImageSkia> image,
                              const UserImage::RawImage& raw_image);

  
  void RecordUma(User::WallpaperType type, int index);

  
  
  void SaveCustomWallpaper(const std::string& user_id_hash,
                           const base::FilePath& path,
                           ash::WallpaperLayout layout,
                           const UserImage& wallpaper);

  
  void SaveWallpaperInternal(const base::FilePath& path, const char* data,
                             int size);

  
  
  void StartLoad(const std::string& email,
                 const WallpaperInfo& info,
                 bool update_wallpaper,
                 const base::FilePath& wallpaper_path);

  
  void NotifyAnimationFinished();

  
  int loaded_wallpapers_;

  
  base::SequencedWorkerPool::SequenceToken sequence_token_;

  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  
  base::FilePath current_wallpaper_path_;

  
  scoped_refptr<UserImageLoader> wallpaper_loader_;

  
  WallpaperInfo current_user_wallpaper_info_;

  
  CommandLine* command_line_for_testing_;

  
  CustomWallpaperMap wallpaper_cache_;

  
  std::string last_selected_user_;

  bool should_cache_wallpaper_;

  scoped_ptr<CrosSettings::ObserverSubscription>
      show_user_name_on_signin_subscription_;

  base::WeakPtrFactory<WallpaperManager> weak_factory_;

  content::NotificationRegistrar registrar_;

  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(WallpaperManager);
};

}  

#endif  
