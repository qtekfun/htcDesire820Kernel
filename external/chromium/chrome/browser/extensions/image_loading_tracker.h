// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_IMAGE_LOADING_TRACKER_H_
#define CHROME_BROWSER_EXTENSIONS_IMAGE_LOADING_TRACKER_H_
#pragma once

#include <map>

#include "base/memory/ref_counted.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Extension;
class ExtensionResource;
class SkBitmap;

namespace gfx {
  class Size;
}

class ImageLoadingTracker : public NotificationObserver {
 public:
  enum CacheParam {
    CACHE,
    DONT_CACHE
  };

  class Observer {
   public:
    virtual ~Observer();

    
    
    
    
    
    
    
    virtual void OnImageLoaded(SkBitmap* image,
                               const ExtensionResource& resource,
                               int index) = 0;
  };

  explicit ImageLoadingTracker(Observer* observer);
  ~ImageLoadingTracker();

  
  
  
  
  void LoadImage(const Extension* extension,
                 const ExtensionResource& resource,
                 const gfx::Size& max_size,
                 CacheParam cache);

 private:
  typedef std::map<int, const Extension*> LoadMap;

  class ImageLoader;

  
  
  
  
  
  
  void OnImageLoaded(SkBitmap* image, const ExtensionResource& resource,
                     const gfx::Size& original_size, int id);

  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  Observer* observer_;

  
  int next_id_;

  
  scoped_refptr<ImageLoader> loader_;

  
  
  
  LoadMap load_map_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ImageLoadingTracker);
};

#endif  
