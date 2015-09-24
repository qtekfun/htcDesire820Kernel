// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_H_
#define UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_H_

#include <string>

#include "ui/gfx/native_widget_types.h"
#include "ui/views/view.h"

namespace views {

class NativeViewHostAuraTest;
class NativeViewHostWrapper;

extern const char kWidgetNativeViewHostKey[];

class VIEWS_EXPORT NativeViewHost : public View {
 public:
  
  static const char kViewClassName[];

  
  static const bool kRenderNativeControlFocus;

  NativeViewHost();
  virtual ~NativeViewHost();

  
  
  
  
  
  
  void Attach(gfx::NativeView native_view);

  
  
  
  void Detach();

  
  void SetPreferredSize(const gfx::Size& size);

  
  
  
  
  
  
  
  void set_focus_view(View* view) { focus_view_ = view; }
  View* focus_view() { return focus_view_; }

  
  
  
  
  
  
  void set_fast_resize(bool fast_resize) { fast_resize_ = fast_resize; }
  bool fast_resize() const { return fast_resize_; }

  
  bool fast_resize_at_last_layout() const {
    return fast_resize_at_last_layout_;
  }

  
  gfx::NativeView native_view() const { return native_view_; }

  void NativeViewDestroyed();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;

 protected:
  virtual bool NeedsNotificationWhenVisibleBoundsChange() const OVERRIDE;
  virtual void OnVisibleBoundsChanged() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

 private:
  friend class NativeViewHostAuraTest;

  
  
  void Detach(bool destroyed);

  
  
  
  void ClearFocus();

  
  gfx::NativeView native_view_;

  
  
  scoped_ptr<NativeViewHostWrapper> native_wrapper_;

  
  gfx::Size preferred_size_;

  
  
  bool fast_resize_;

  
  bool fast_resize_at_last_layout_;

  
  View* focus_view_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewHost);
};

}  

#endif  
