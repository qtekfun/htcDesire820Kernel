// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_NAVIGATION_ENTRY_SCREENSHOT_MANAGER_H_
#define CONTENT_BROWSER_FRAME_HOST_NAVIGATION_ENTRY_SCREENSHOT_MANAGER_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/common/content_export.h"

class SkBitmap;

namespace content {

class NavigationControllerImpl;
class NavigationEntryImpl;
class RenderViewHost;
class ScreenshotData;

class CONTENT_EXPORT NavigationEntryScreenshotManager {
 public:
  explicit NavigationEntryScreenshotManager(
      NavigationControllerImpl* controller);
  virtual ~NavigationEntryScreenshotManager();

  
  void TakeScreenshot();

  
  void ClearAllScreenshots();

 protected:
  virtual void TakeScreenshotImpl(RenderViewHost* host,
                                  NavigationEntryImpl* entry);

  
  
  virtual void OnScreenshotSet(NavigationEntryImpl* entry);

  NavigationControllerImpl* owner() { return owner_; }

  void SetMinScreenshotIntervalMS(int interval_ms);

  
  
  void OnScreenshotTaken(int unique_id,
                         bool success,
                         const SkBitmap& bitmap);

  
  int GetScreenshotCount() const;

 private:
  
  
  void OnScreenshotEncodeComplete(int unique_id,
                                  scoped_refptr<ScreenshotData> data);

  
  
  bool ClearScreenshot(NavigationEntryImpl* entry);

  
  
  
  void PurgeScreenshotsIfNecessary();

  
  NavigationControllerImpl* owner_;

  
  
  
  base::WeakPtrFactory<NavigationEntryScreenshotManager> screenshot_factory_;

  base::Time last_screenshot_time_;
  int min_screenshot_interval_ms_;

  DISALLOW_COPY_AND_ASSIGN(NavigationEntryScreenshotManager);
};

}  

#endif  
