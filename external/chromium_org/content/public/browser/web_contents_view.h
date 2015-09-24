// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_VIEW_H_
#define CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_VIEW_H_

#include <string>

#include "base/basictypes.h"
#include "base/process/kill.h"
#include "content/common/content_export.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

namespace content {
struct DropData;

class CONTENT_EXPORT WebContentsView {
 public:
  virtual ~WebContentsView() {}

  
  virtual gfx::NativeView GetNativeView() const = 0;

  
  
  
  virtual gfx::NativeView GetContentNativeView() const = 0;

  
  
  virtual gfx::NativeWindow GetTopLevelNativeWindow() const = 0;

  
  
  virtual void GetContainerBounds(gfx::Rect* out) const = 0;

  
  
  gfx::Size GetContainerSize() const {
    gfx::Rect rc;
    GetContainerBounds(&rc);
    return gfx::Size(rc.width(), rc.height());
  }

  
  virtual void OnTabCrashed(base::TerminationStatus status, int error_code) = 0;

  
  
  
  
  
  
  
  
  virtual void SizeContents(const gfx::Size& size) = 0;

  
  virtual void Focus() = 0;

  
  
  virtual void SetInitialFocus() = 0;

  
  virtual void StoreFocus() = 0;

  
  
  virtual void RestoreFocus() = 0;

  
  virtual DropData* GetDropData() const = 0;

  
  virtual gfx::Rect GetViewBounds() const = 0;

#if defined(OS_MACOSX)
  
  
  
  
  virtual void SetAllowOverlappingViews(bool overlapping) = 0;

  
  virtual bool GetAllowOverlappingViews() const = 0;

  
  
  
  virtual void SetOverlayView(WebContentsView* overlay,
                              const gfx::Point& offset) = 0;

  
  virtual void RemoveOverlayView() = 0;
#endif
};

}  

#endif  
