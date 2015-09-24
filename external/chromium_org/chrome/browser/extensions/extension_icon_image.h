// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ICON_IMAGE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ICON_IMAGE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/extensions/extension_icon_set.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/layout.h"
#include "ui/gfx/image/image_skia.h"

namespace content {
class BrowserContext;
}

namespace extensions {
class Extension;
}

namespace gfx {
class Size;
class Image;
}

namespace extensions {

class IconImage : public content::NotificationObserver {
 public:
  class Observer {
   public:
    
    
    virtual void OnExtensionIconImageChanged(IconImage* image) = 0;

   protected:
    virtual ~Observer() {}
  };

  
  
  
  IconImage(content::BrowserContext* context,
            const Extension* extension,
            const ExtensionIconSet& icon_set,
            int resource_size_in_dip,
            const gfx::ImageSkia& default_icon,
            Observer* observer);
  virtual ~IconImage();

  const gfx::ImageSkia& image_skia() const { return image_skia_; }

 private:
  class Source;

  
  
  
  
  
  
  gfx::ImageSkiaRep LoadImageForScaleFactor(ui::ScaleFactor scale_factor);

  void OnImageLoaded(float scale_factor, const gfx::Image& image);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::BrowserContext* browser_context_;
  const Extension* extension_;
  const ExtensionIconSet& icon_set_;
  const int resource_size_in_dip_;

  Observer* observer_;

  Source* source_;  
  gfx::ImageSkia image_skia_;
  
  
  gfx::ImageSkia default_icon_;

  content::NotificationRegistrar registrar_;

  base::WeakPtrFactory<IconImage> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(IconImage);
};

}  

#endif  
