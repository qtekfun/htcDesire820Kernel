// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_NATIVE_BROWSER_FRAME_FACTORY_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_NATIVE_BROWSER_FRAME_FACTORY_H_

#include "base/basictypes.h"
#include "chrome/browser/ui/host_desktop.h"

class BrowserFrame;
class BrowserView;
class NativeBrowserFrame;

class NativeBrowserFrameFactory {
 public:
  
  static NativeBrowserFrame* CreateNativeBrowserFrame(
      BrowserFrame* browser_frame,
      BrowserView* browser_view);

  
  
  static void Set(NativeBrowserFrameFactory* new_factory);

  
  
  static chrome::HostDesktopType AdjustHostDesktopType(
      chrome::HostDesktopType desktop_type);

  virtual NativeBrowserFrame* Create(BrowserFrame* browser_frame,
                                     BrowserView* browser_view);

 protected:
  NativeBrowserFrameFactory() {}
  virtual ~NativeBrowserFrameFactory() {}

 private:
  
  
  static bool ShouldCreateForAshDesktop(BrowserView* browser_view);

  DISALLOW_COPY_AND_ASSIGN(NativeBrowserFrameFactory);
};

#endif  
