// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_UI_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_UI_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/common/extensions/url_pattern.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/native_widget_types.h"

class Extension;
class MessageLoop;
class Profile;
class InfoBarDelegate;
class TabContents;

class ExtensionInstallUI : public ImageLoadingTracker::Observer {
 public:
  enum PromptType {
    UNSET_PROMPT_TYPE = -1,
    INSTALL_PROMPT = 0,
    RE_ENABLE_PROMPT,
    NUM_PROMPT_TYPES
  };

  
  static const int kTitleIds[NUM_PROMPT_TYPES];
  static const int kHeadingIds[NUM_PROMPT_TYPES];
  static const int kButtonIds[NUM_PROMPT_TYPES];
  static const int kWarningIds[NUM_PROMPT_TYPES];

  class Delegate {
   public:
    
    virtual void InstallUIProceed() = 0;

    
    virtual void InstallUIAbort() = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit ExtensionInstallUI(Profile* profile);
  virtual ~ExtensionInstallUI();

  
  
  
  
  virtual void ConfirmInstall(Delegate* delegate, const Extension* extension);

  
  
  
  
  virtual void ConfirmReEnable(Delegate* delegate, const Extension* extension);

  
  virtual void OnInstallSuccess(const Extension* extension, SkBitmap* icon);

  
  virtual void OnInstallFailure(const std::string& error);

  
  virtual void OnImageLoaded(
      SkBitmap* image, const ExtensionResource& resource, int index);

  
  
  
  
  
  
  static void ShowThemeInfoBar(
      const std::string& previous_theme_id, bool previous_use_system_theme,
      const Extension* new_theme, Profile* profile);

 private:
  
  
  void SetIcon(SkBitmap* icon);

  
  
  
  void ShowConfirmation(PromptType prompt_type);

  
  
  static InfoBarDelegate* GetNewThemeInstalledInfoBarDelegate(
      TabContents* tab_contents,
      const Extension* new_theme,
      const std::string& previous_theme_id,
      bool previous_use_system_theme);

  Profile* profile_;
  MessageLoop* ui_loop_;

  
  std::string previous_theme_id_;
  bool previous_use_system_theme_;

  
  SkBitmap icon_;

  
  const Extension* extension_;

  
  Delegate* delegate_;

  
  PromptType prompt_type_;

  
  
  ImageLoadingTracker tracker_;
};

#endif  
