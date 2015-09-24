// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_WALLPAPER_FUNCTION_BASE_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_WALLPAPER_FUNCTION_BASE_H_

#include "ash/desktop_background/desktop_background_controller.h"
#include "extensions/browser/extension_function.h"
#include "ui/gfx/image/image_skia.h"

namespace wallpaper_api_util {
ash::WallpaperLayout GetLayoutEnum(const std::string& layout);
}  

class WallpaperFunctionBase : public AsyncExtensionFunction {
 public:
  WallpaperFunctionBase();

 protected:
  virtual ~WallpaperFunctionBase();

  
  class UnsafeWallpaperDecoder;

  
  static UnsafeWallpaperDecoder* unsafe_wallpaper_decoder_;

  
  void StartDecode(const std::string& data);

  
  void OnCancel();

  
  void OnFailure(const std::string& error);

 private:
  virtual void OnWallpaperDecoded(const gfx::ImageSkia& wallpaper) = 0;
};

#endif  
