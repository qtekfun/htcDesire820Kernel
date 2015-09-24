// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CREATE_APPLICATION_SHORTCUT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_CREATE_APPLICATION_SHORTCUT_VIEW_H_
#pragma once

#include <string>

#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/web_applications/web_app.h"
#include "views/controls/label.h"
#include "views/view.h"
#include "views/window/dialog_delegate.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace views {
class Checkbox;
class Label;
class Window;
};  

class Extension;
class MessageLoop;
class Profile;
class TabContents;
class TabContentsWrapper;

class CreateApplicationShortcutView : public views::View,
                                      public views::DialogDelegate,
                                      public views::ButtonListener {
 public:
  explicit CreateApplicationShortcutView(Profile* profile);
  virtual ~CreateApplicationShortcutView();

  
  void InitControls();

  
  virtual gfx::Size GetPreferredSize();

  
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;
  virtual bool IsDialogButtonEnabled(
      MessageBoxFlags::DialogButton button) const;
  virtual bool CanResize() const;
  virtual bool CanMaximize() const;
  virtual bool IsAlwaysOnTop() const;
  virtual bool HasAlwaysOnTopMenu() const;
  virtual bool IsModal() const;
  virtual std::wstring GetWindowTitle() const;
  virtual bool Accept();
  virtual views::View* GetContentsView();

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

 protected:
  
  views::Checkbox* AddCheckbox(const std::wstring& text, bool checked);

  
  Profile* profile_;

  
  views::View* app_info_;
  views::Label* create_shortcuts_label_;
  views::Checkbox* desktop_check_box_;
  views::Checkbox* menu_check_box_;
  views::Checkbox* quick_launch_check_box_;

  
  ShellIntegration::ShortcutInfo shortcut_info_;

  DISALLOW_COPY_AND_ASSIGN(CreateApplicationShortcutView);
};

class CreateUrlApplicationShortcutView : public CreateApplicationShortcutView {
 public:
  explicit CreateUrlApplicationShortcutView(TabContentsWrapper* tab_contents);
  virtual ~CreateUrlApplicationShortcutView();

  virtual bool Accept();

 private:
  
  
  void FetchIcon();

  
  void OnIconDownloaded(bool errored, const SkBitmap& image);

  
  TabContentsWrapper* tab_contents_;

  
  class IconDownloadCallbackFunctor;
  IconDownloadCallbackFunctor* pending_download_;

  
  web_app::IconInfoList unprocessed_icons_;

  DISALLOW_COPY_AND_ASSIGN(CreateUrlApplicationShortcutView);
};

class CreateChromeApplicationShortcutView
   : public CreateApplicationShortcutView,
     public ImageLoadingTracker::Observer {
 public:
  explicit CreateChromeApplicationShortcutView(Profile* profile,
                                               const Extension* app);
  virtual ~CreateChromeApplicationShortcutView();

  
  
  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index);

 private:
  const Extension* app_;
  ImageLoadingTracker tracker_;

  DISALLOW_COPY_AND_ASSIGN(CreateChromeApplicationShortcutView);
};


#endif  
