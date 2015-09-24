// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_WALLPAPER_API_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_WALLPAPER_API_H_

#include "base/threading/sequenced_worker_pool.h"
#include "chrome/browser/chromeos/extensions/wallpaper_function_base.h"
#include "chrome/common/extensions/api/wallpaper.h"
#include "net/url_request/url_request_status.h"

class WallpaperSetWallpaperFunction : public WallpaperFunctionBase {
 public:
  DECLARE_EXTENSION_FUNCTION("wallpaper.setWallpaper",
                             WALLPAPER_SETWALLPAPER)

  WallpaperSetWallpaperFunction();

 protected:
  virtual ~WallpaperSetWallpaperFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  virtual void OnWallpaperDecoded(const gfx::ImageSkia& wallpaper) OVERRIDE;

  
  
  void GenerateThumbnail(const base::FilePath& thumbnail_path,
                         scoped_ptr<gfx::ImageSkia> image);

  
  void ThumbnailGenerated(base::RefCountedBytes* data);

  
  void OnWallpaperFetched(net::URLRequestStatus::Status status,
                          const std::string& response);

  scoped_ptr<extensions::api::wallpaper::SetWallpaper::Params> params_;

  
  std::string file_name_;

  
  std::string email_;

  
  std::string user_id_hash_;

  
  
  base::SequencedWorkerPool::SequenceToken sequence_token_;
};

#endif  

