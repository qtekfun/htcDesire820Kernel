// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_LAYOUT_DELEGATE_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_LAYOUT_DELEGATE_H_

namespace gfx {
class Size;
}

class OpaqueBrowserFrameViewLayoutDelegate {
 public:
  
  
  virtual bool ShouldShowWindowIcon() const = 0;
  virtual bool ShouldShowWindowTitle() const = 0;
  virtual base::string16 GetWindowTitle() const = 0;

  
  
  virtual int GetIconSize() const = 0;

  
  
  
  virtual bool ShouldLeaveOffsetNearTopBorder() const = 0;

  
  
  virtual gfx::Size GetBrowserViewMinimumSize() const = 0;

  
  
  virtual bool ShouldShowCaptionButtons() const = 0;

  
  virtual bool ShouldShowAvatar() const = 0;

  
  virtual bool IsRegularOrGuestSession() const = 0;

  
  
  virtual gfx::ImageSkia GetOTRAvatarIcon() const = 0;

  
  virtual bool IsMaximized() const = 0;
  virtual bool IsMinimized() const = 0;
  virtual bool IsFullscreen() const = 0;

  virtual bool IsTabStripVisible() const = 0;
  virtual int GetTabStripHeight() const = 0;

  
  
  
  virtual int GetAdditionalReservedSpaceInTabStrip() const = 0;

  
  
  virtual gfx::Size GetTabstripPreferredSize() const = 0;

 protected:
  virtual ~OpaqueBrowserFrameViewLayoutDelegate() {}
};

#endif  
