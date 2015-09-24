// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ICON_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ICON_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/insets.h"

class Extension;

class ExtensionIconManager : public ImageLoadingTracker::Observer {
 public:
  ExtensionIconManager();
  virtual ~ExtensionIconManager();

  
  void LoadIcon(const Extension* extension);

  
  
  
  const SkBitmap& GetIcon(const std::string& extension_id);

  
  void RemoveIcon(const std::string& extension_id);

  
  virtual void OnImageLoaded(SkBitmap* image, const ExtensionResource& resource,
                             int index);

  void set_monochrome(bool value) { monochrome_ = value; }
  void set_padding(const gfx::Insets& value) { padding_ = value; }

 private:
  
  
  void EnsureDefaultIcon();

  
  
  SkBitmap ApplyTransforms(const SkBitmap& src);

  
  ImageLoadingTracker image_tracker_;

  
  std::map<std::string, SkBitmap> icons_;

  
  std::set<std::string> pending_icons_;

  
  SkBitmap default_icon_;

  
  bool monochrome_;

  
  gfx::Insets padding_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionIconManager);
};

#endif  
