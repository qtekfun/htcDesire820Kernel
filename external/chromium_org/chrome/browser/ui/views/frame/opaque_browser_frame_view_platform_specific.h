// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_PLATFORM_SPECIFIC_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_PLATFORM_SPECIFIC_H_

class OpaqueBrowserFrameView;
class OpaqueBrowserFrameViewLayout;

class OpaqueBrowserFrameViewPlatformSpecific {
 public:
  virtual ~OpaqueBrowserFrameViewPlatformSpecific() {}

  
  
  static OpaqueBrowserFrameViewPlatformSpecific* Create(
      OpaqueBrowserFrameView* view,
      OpaqueBrowserFrameViewLayout* layout);

  
  
  
  virtual bool ShouldShowCaptionButtons() const;
};

#endif  
