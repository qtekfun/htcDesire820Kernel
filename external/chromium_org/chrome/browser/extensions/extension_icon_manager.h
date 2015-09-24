// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ICON_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ICON_MANAGER_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/insets.h"

namespace content {
class BrowserContext;
}

namespace extensions {
class Extension;
}

namespace gfx {
class Image;
}

class ExtensionIconManager {
 public:
  ExtensionIconManager();
  virtual ~ExtensionIconManager();

  
  void LoadIcon(content::BrowserContext* context,
                const extensions::Extension* extension);

  
  
  
  const SkBitmap& GetIcon(const std::string& extension_id);

  
  void RemoveIcon(const std::string& extension_id);

  void set_monochrome(bool value) { monochrome_ = value; }
  void set_padding(const gfx::Insets& value) { padding_ = value; }

 protected:
  virtual void OnImageLoaded(const std::string& extension_id,
                             const gfx::Image& image);

 private:
  
  
  void EnsureDefaultIcon();

  
  
  SkBitmap ApplyTransforms(const SkBitmap& src);

  
  std::map<std::string, SkBitmap> icons_;

  
  std::set<std::string> pending_icons_;

  
  SkBitmap default_icon_;

  
  bool monochrome_;

  
  gfx::Insets padding_;

  base::WeakPtrFactory<ExtensionIconManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionIconManager);
};

#endif  
