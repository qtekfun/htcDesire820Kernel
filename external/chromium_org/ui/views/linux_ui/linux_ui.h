// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LINUX_UI_LINUX_UI_H_
#define UI_VIEWS_LINUX_UI_LINUX_UI_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/ime/linux/linux_input_method_context_factory.h"
#include "ui/shell_dialogs/linux_shell_dialog.h"
#include "ui/views/linux_ui/status_icon_linux.h"
#include "ui/views/views_export.h"


namespace gfx {
class Image;
}

namespace ui {
class NativeTheme;
}

namespace views {
class WindowButtonOrderObserver;

class VIEWS_EXPORT LinuxUI : public ui::LinuxInputMethodContextFactory,
                             public ui::LinuxShellDialog {
 public:
  virtual ~LinuxUI() {}

  
  static void SetInstance(LinuxUI* instance);

  
  
  
  
  
  static LinuxUI* instance();

  virtual void Initialize() = 0;

  
  virtual gfx::Image GetThemeImageNamed(int id) const = 0;
  virtual bool GetColor(int id, SkColor* color) const = 0;
  virtual bool HasCustomImage(int id) const = 0;

  
  virtual SkColor GetFocusRingColor() const = 0;
  virtual SkColor GetThumbActiveColor() const = 0;
  virtual SkColor GetThumbInactiveColor() const = 0;
  virtual SkColor GetTrackColor() const = 0;
  virtual SkColor GetActiveSelectionBgColor() const = 0;
  virtual SkColor GetActiveSelectionFgColor() const = 0;
  virtual SkColor GetInactiveSelectionBgColor() const = 0;
  virtual SkColor GetInactiveSelectionFgColor() const = 0;
  virtual double GetCursorBlinkInterval() const = 0;

  
  
  virtual ui::NativeTheme* GetNativeTheme() const = 0;

  virtual void SetUseSystemTheme(bool use_system_theme) = 0;

  
  
  virtual bool GetDefaultUsesSystemTheme() const = 0;

  
  
  virtual void SetDownloadCount(int count) const = 0;
  virtual void SetProgressFraction(float percentage) const = 0;

  
  virtual bool IsStatusIconSupported() const = 0;

  
  virtual scoped_ptr<StatusIconLinux> CreateLinuxStatusIcon(
      const gfx::ImageSkia& image,
      const string16& tool_tip) const = 0;

  
  
  
  virtual gfx::Image GetIconForContentType(
      const std::string& content_type, int size) const = 0;

  
  
  
  virtual void AddWindowButtonOrderObserver(
      WindowButtonOrderObserver* observer) = 0;

  
  virtual void RemoveWindowButtonOrderObserver(
      WindowButtonOrderObserver* observer) = 0;

  
  virtual bool UnityIsRunning() = 0;
};

}  

#endif  
