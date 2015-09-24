// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ABOUT_CHROME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_ABOUT_CHROME_VIEW_H_
#pragma once

#include "views/controls/image_view.h"
#include "views/controls/label.h"
#include "views/controls/link.h"
#include "views/view.h"
#include "views/window/dialog_delegate.h"

#if defined(OS_WIN) || defined(OS_CHROMEOS)
#include "chrome/browser/google/google_update.h"
#endif
#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/version_loader.h"
#endif

namespace views {
class Textfield;
class Throbber;
class Window;
}

class Profile;

class AboutChromeView : public views::View,
                        public views::DialogDelegate,
                        public views::LinkController
#if defined(OS_WIN) || defined(OS_CHROMEOS)
                        , public GoogleUpdateStatusListener
#endif
                        {
 public:
  explicit AboutChromeView(Profile* profile);
  virtual ~AboutChromeView();

  
  void Init();

  
  virtual gfx::Size GetPreferredSize();
  virtual void Layout();
  virtual void OnPaint(gfx::Canvas* canvas);
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

  
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;
  virtual bool IsDialogButtonEnabled(
      MessageBoxFlags::DialogButton button) const;
  virtual bool IsDialogButtonVisible(
      MessageBoxFlags::DialogButton button) const;
  virtual int GetDefaultDialogButton() const;
  virtual bool CanResize() const;
  virtual bool CanMaximize() const;
  virtual bool IsAlwaysOnTop() const;
  virtual bool HasAlwaysOnTopMenu() const;
  virtual bool IsModal() const;
  virtual std::wstring GetWindowTitle() const;
  virtual bool Accept();
  virtual views::View* GetContentsView();

  
  virtual void LinkActivated(views::Link* source, int event_flags);
#if defined(OS_WIN) || defined(OS_CHROMEOS)
  
  virtual void OnReportResults(GoogleUpdateUpgradeResult result,
                               GoogleUpdateErrorCode error_code,
                               const std::wstring& version);
#endif

 private:
#if defined(OS_WIN) || defined(OS_CHROMEOS)
  
  void UpdateStatus(GoogleUpdateUpgradeResult result,
                    GoogleUpdateErrorCode error_code);
#endif

#if defined(OS_CHROMEOS)
  
  void OnOSVersion(chromeos::VersionLoader::Handle handle,
                   std::string version);
#endif


  Profile* profile_;

  
  views::ImageView* about_dlg_background_logo_;
  views::Label* about_title_label_;
  views::Textfield* version_label_;
#if defined(OS_CHROMEOS)
  views::Textfield* os_version_label_;
#endif
  views::Label* copyright_label_;
  views::Label* main_text_label_;
  int main_text_label_height_;
  views::Link* chromium_url_;
  gfx::Rect chromium_url_rect_;
  views::Link* open_source_url_;
  gfx::Rect open_source_url_rect_;
  views::Link* terms_of_service_url_;
  gfx::Rect terms_of_service_url_rect_;
  
  scoped_ptr<views::Throbber> throbber_;
  views::ImageView success_indicator_;
  views::ImageView update_available_indicator_;
  views::ImageView timeout_indicator_;
  views::Label update_label_;

  
  gfx::Size dialog_dimensions_;

  
  bool restart_button_visible_;

  
  
  
  std::wstring main_label_chunk1_;
  std::wstring main_label_chunk2_;
  std::wstring main_label_chunk3_;
  std::wstring main_label_chunk4_;
  std::wstring main_label_chunk5_;
  
  bool chromium_url_appears_first_;

#if defined(OS_WIN) || defined(OS_CHROMEOS)
  
  
  scoped_refptr<GoogleUpdate> google_updater_;
#endif

  
  std::string current_version_;

  
  std::string version_details_;

  
  std::wstring new_version_available_;

  
  bool text_direction_is_rtl_;

#if defined(OS_CHROMEOS)
  
  chromeos::VersionLoader loader_;

  
  CancelableRequestConsumer consumer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(AboutChromeView);
};

#endif  
