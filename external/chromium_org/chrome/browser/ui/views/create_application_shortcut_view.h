// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CREATE_APPLICATION_SHORTCUT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_CREATE_APPLICATION_SHORTCUT_VIEW_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/web_applications/web_app.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/dialog_delegate.h"

class FaviconDownloadHelper;
class GURL;
class Profile;
class SkBitmap;

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

namespace views {
class Checkbox;
class Label;
}

class CreateApplicationShortcutView : public views::DialogDelegateView,
                                      public views::ButtonListener {
 public:
  explicit CreateApplicationShortcutView(Profile* profile);
  virtual ~CreateApplicationShortcutView();

  
  void InitControls();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual bool Accept() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

 protected:
  
  views::Checkbox* AddCheckbox(const base::string16& text, bool checked);

  
  Profile* profile_;

  
  views::View* app_info_;
  views::Label* create_shortcuts_label_;
  views::Checkbox* desktop_check_box_;
  views::Checkbox* menu_check_box_;
  views::Checkbox* quick_launch_check_box_;

  
  ShellIntegration::ShortcutInfo shortcut_info_;
  
  bool create_in_chrome_apps_subdir_;

  DISALLOW_COPY_AND_ASSIGN(CreateApplicationShortcutView);
};

class CreateUrlApplicationShortcutView : public CreateApplicationShortcutView {
 public:
  explicit CreateUrlApplicationShortcutView(content::WebContents* web_contents);
  virtual ~CreateUrlApplicationShortcutView();

  virtual bool Accept() OVERRIDE;

 private:
  
  
  void FetchIcon();

  
  void DidDownloadFavicon(
      int requested_size,
      int id,
      int http_status_code,
      const GURL& image_url,
      const std::vector<SkBitmap>& bitmaps,
      const std::vector<gfx::Size>& original_bitmap_sizes);

  
  content::WebContents* web_contents_;

  
  int pending_download_id_;

  
  web_app::IconInfoList unprocessed_icons_;

  base::WeakPtrFactory<CreateUrlApplicationShortcutView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(CreateUrlApplicationShortcutView);
};

class CreateChromeApplicationShortcutView
    : public CreateApplicationShortcutView {
 public:
  CreateChromeApplicationShortcutView(
      Profile* profile,
      const extensions::Extension* app,
      const base::Closure& close_callback);
  virtual ~CreateChromeApplicationShortcutView();
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;

 private:
  void OnShortcutInfoLoaded(
      const ShellIntegration::ShortcutInfo& shortcut_info);

  const extensions::Extension* app_;
  base::Closure close_callback_;

  base::WeakPtrFactory<CreateChromeApplicationShortcutView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(CreateChromeApplicationShortcutView);
};

#endif  
