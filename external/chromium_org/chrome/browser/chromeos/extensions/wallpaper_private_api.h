// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_WALLPAPER_PRIVATE_API_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_WALLPAPER_PRIVATE_API_H_

#include "base/threading/sequenced_worker_pool.h"
#include "chrome/browser/chromeos/extensions/wallpaper_function_base.h"
#include "chrome/common/extensions/api/wallpaper_private.h"
#include "net/url_request/url_fetcher_delegate.h"

namespace chromeos {
class UserImage;
}  

class WallpaperPrivateGetStringsFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.getStrings",
                             WALLPAPERPRIVATE_GETSTRINGS)

 protected:
  virtual ~WallpaperPrivateGetStringsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class WallpaperPrivateSetWallpaperIfExistsFunction
    : public WallpaperFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.setWallpaperIfExists",
                             WALLPAPERPRIVATE_SETWALLPAPERIFEXISTS)

  WallpaperPrivateSetWallpaperIfExistsFunction();

 protected:
  virtual ~WallpaperPrivateSetWallpaperIfExistsFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual void OnWallpaperDecoded(const gfx::ImageSkia& wallpaper) OVERRIDE;

  
  void OnFileNotExists(const std::string& error);

  
  
  void ReadFileAndInitiateStartDecode(const base::FilePath& file_path,
                                      const base::FilePath& fallback_path);

  scoped_ptr<extensions::api::wallpaper_private::SetWallpaperIfExists::Params>
      params;

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

class WallpaperPrivateSetWallpaperFunction : public WallpaperFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.setWallpaper",
                             WALLPAPERPRIVATE_SETWALLPAPER)

  WallpaperPrivateSetWallpaperFunction();

 protected:
  virtual ~WallpaperPrivateSetWallpaperFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual void OnWallpaperDecoded(const gfx::ImageSkia& wallpaper) OVERRIDE;

  
  void SaveToFile();

  
  void SetDecodedWallpaper(scoped_ptr<gfx::ImageSkia> wallpaper);

  scoped_ptr<extensions::api::wallpaper_private::SetWallpaper::Params> params;

  
  
  gfx::ImageSkia wallpaper_;

  
  std::string email_;

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

class WallpaperPrivateResetWallpaperFunction
    : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.resetWallpaper",
                             WALLPAPERPRIVATE_RESETWALLPAPER)

  WallpaperPrivateResetWallpaperFunction();

 protected:
  virtual ~WallpaperPrivateResetWallpaperFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class WallpaperPrivateSetCustomWallpaperFunction
    : public WallpaperFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.setCustomWallpaper",
                             WALLPAPERPRIVATE_SETCUSTOMWALLPAPER)

  WallpaperPrivateSetCustomWallpaperFunction();

 protected:
  virtual ~WallpaperPrivateSetCustomWallpaperFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual void OnWallpaperDecoded(const gfx::ImageSkia& wallpaper) OVERRIDE;

  
  
  void GenerateThumbnail(const base::FilePath& thumbnail_path,
                         scoped_ptr<gfx::ImageSkia> image);

  
  void ThumbnailGenerated(base::RefCountedBytes* data);

  scoped_ptr<extensions::api::wallpaper_private::SetCustomWallpaper::Params>
      params;

  
  std::string email_;

  
  std::string user_id_hash_;

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

class WallpaperPrivateSetCustomWallpaperLayoutFunction
    : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.setCustomWallpaperLayout",
                             WALLPAPERPRIVATE_SETCUSTOMWALLPAPERLAYOUT)

  WallpaperPrivateSetCustomWallpaperLayoutFunction();

 protected:
  virtual ~WallpaperPrivateSetCustomWallpaperLayoutFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class WallpaperPrivateMinimizeInactiveWindowsFunction
    : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.minimizeInactiveWindows",
                             WALLPAPERPRIVATE_MINIMIZEINACTIVEWINDOWS)

  WallpaperPrivateMinimizeInactiveWindowsFunction();

 protected:
  virtual ~WallpaperPrivateMinimizeInactiveWindowsFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class WallpaperPrivateRestoreMinimizedWindowsFunction
    : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.restoreMinimizedWindows",
                             WALLPAPERPRIVATE_RESTOREMINIMIZEDWINDOWS)

  WallpaperPrivateRestoreMinimizedWindowsFunction();

 protected:
  virtual ~WallpaperPrivateRestoreMinimizedWindowsFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class WallpaperPrivateGetThumbnailFunction : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.getThumbnail",
                             WALLPAPERPRIVATE_GETTHUMBNAIL)

  WallpaperPrivateGetThumbnailFunction();

 protected:
  virtual ~WallpaperPrivateGetThumbnailFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void Failure(const std::string& file_name);

  
  
  void FileNotLoaded();

  
  
  void FileLoaded(const std::string& data);

  
  void Get(const base::FilePath& path);

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

class WallpaperPrivateSaveThumbnailFunction : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.saveThumbnail",
                             WALLPAPERPRIVATE_SAVETHUMBNAIL)

  WallpaperPrivateSaveThumbnailFunction();

 protected:
  virtual ~WallpaperPrivateSaveThumbnailFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void Failure(const std::string& file_name);

  
  void Success();

  
  void Save(const std::string& data, const std::string& file_name);

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

class WallpaperPrivateGetOfflineWallpaperListFunction
    : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaperPrivate.getOfflineWallpaperList",
                             WALLPAPERPRIVATE_GETOFFLINEWALLPAPERLIST)
  WallpaperPrivateGetOfflineWallpaperListFunction();

 protected:
  virtual ~WallpaperPrivateGetOfflineWallpaperListFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void GetList();

  
  
  void OnComplete(const std::vector<std::string>& file_list);

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

#endif  
