// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEWS_DELEGATE_H_
#define UI_VIEWS_VIEWS_DELEGATE_H_

#include <string>

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "base/strings/string16.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/widget.h"

namespace base {
class TimeDelta;
}

namespace content {
class WebContents;
class BrowserContext;
class SiteInstance;
}

namespace gfx {
class ImageSkia;
class Rect;
}

namespace views {

class NativeWidget;
class NonClientFrameView;
class ViewsTouchSelectionControllerFactory;
class View;
class Widget;
namespace internal {
class NativeWidgetDelegate;
}

class VIEWS_EXPORT ViewsDelegate {
 public:
  
  static ViewsDelegate* views_delegate;

  ViewsDelegate();

  virtual ~ViewsDelegate();

  
  
  virtual void SaveWindowPlacement(const Widget* widget,
                                   const std::string& window_name,
                                   const gfx::Rect& bounds,
                                   ui::WindowShowState show_state) = 0;

  
  
  virtual bool GetSavedWindowPlacement(
      const Widget* widget,
      const std::string& window_name,
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const = 0;

  virtual void NotifyAccessibilityEvent(
      View* view,
      ui::AccessibilityTypes::Event event_type) = 0;

  
  
  virtual void NotifyMenuItemFocused(const string16& menu_name,
                                     const string16& menu_item_name,
                                     int item_index,
                                     int item_count,
                                     bool has_submenu) = 0;

#if defined(OS_WIN)
  
  virtual HICON GetDefaultWindowIcon() const = 0;
  
  
  virtual bool IsWindowInMetro(gfx::NativeWindow window) const = 0;
#elif defined(OS_LINUX) && !defined(OS_CHROMEOS)
  virtual gfx::ImageSkia* GetDefaultWindowIcon() const = 0;
#endif

  
  
  
  virtual NonClientFrameView* CreateDefaultNonClientFrameView(
      Widget* widget) = 0;

  
  
  
  virtual bool UseTransparentWindows() const = 0;

  
  
  virtual void AddRef() = 0;
  virtual void ReleaseRef() = 0;

  
  virtual content::WebContents* CreateWebContents(
      content::BrowserContext* browser_context,
      content::SiteInstance* site_instance) = 0;

  
  virtual void OnBeforeWidgetInit(Widget::InitParams* params,
                                  internal::NativeWidgetDelegate* delegate) = 0;

  
  virtual base::TimeDelta GetDefaultTextfieldObscuredRevealDuration() = 0;

 private:
  scoped_ptr<ViewsTouchSelectionControllerFactory> views_tsc_factory_;
};

}  

#endif  
