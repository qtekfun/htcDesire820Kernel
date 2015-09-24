// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_DESKTOP_MEDIA_LIST_H_
#define CHROME_BROWSER_MEDIA_DESKTOP_MEDIA_LIST_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "content/public/browser/desktop_media_id.h"
#include "ui/gfx/image/image_skia.h"

class DesktopMediaListObserver;

class DesktopMediaList {
 public:
  
  struct Source {
    
    content::DesktopMediaID id;

    
    base::string16 name;

    
    gfx::ImageSkia thumbnail;
  };

  virtual ~DesktopMediaList() {}

  
  
  
  virtual void SetUpdatePeriod(base::TimeDelta period) = 0;

  
  
  
  virtual void SetThumbnailSize(const gfx::Size& thumbnail_size) = 0;

  
  
  virtual void SetViewDialogWindowId(content::DesktopMediaID::Id dialog_id) = 0;

  
  
  
  
  
  virtual void StartUpdating(DesktopMediaListObserver* observer) = 0;

  virtual int GetSourceCount() const = 0;
  virtual const Source& GetSource(int index) const = 0;
};

#endif  
