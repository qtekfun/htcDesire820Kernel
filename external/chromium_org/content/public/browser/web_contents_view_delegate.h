// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_VIEW_WIN_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_VIEW_WIN_DELEGATE_H_

#if defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#elif defined(OS_MACOSX)
#import <Cocoa/Cocoa.h>
#endif

#include "content/common/content_export.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_MACOSX)
@protocol RenderWidgetHostViewMacDelegate;
#endif

namespace gfx {
class Size;
}

namespace ui {
class FocusStoreGtk;
}

namespace content {

class RenderWidgetHost;
class WebDragDestDelegate;
struct ContextMenuParams;

class CONTENT_EXPORT WebContentsViewDelegate {
 public:
  virtual ~WebContentsViewDelegate() {}

  
  virtual WebDragDestDelegate* GetDragDestDelegate() = 0;

  
  virtual void ShowContextMenu(const content::ContextMenuParams& params) = 0;

#if defined(OS_WIN) || defined(USE_AURA)
  
  
  
  virtual void StoreFocus() = 0;
  virtual void RestoreFocus() = 0;
  virtual bool Focus() = 0;
  virtual void TakeFocus(bool reverse) = 0;
  virtual void SizeChanged(const gfx::Size& size) = 0;
#elif defined(TOOLKIT_GTK)
  
  virtual void Initialize(GtkWidget* expanded_container,
                          ui::FocusStoreGtk* focus_store) = 0;

  
  
  
  virtual gfx::NativeView GetNativeView() const = 0;

  
  virtual void Focus() = 0;

  
  
  
  virtual gboolean OnNativeViewFocusEvent(GtkWidget* widget,
                                          GtkDirectionType type,
                                          gboolean* return_value) = 0;
#elif defined(OS_MACOSX)
  
  
  virtual NSObject<RenderWidgetHostViewMacDelegate>*
      CreateRenderWidgetHostViewDelegate(
          RenderWidgetHost* render_widget_host) = 0;
#endif
};

}  

#endif  
