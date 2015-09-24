// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_UPDATE_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_UPDATE_VIEW_H_
#pragma once

#include "views/view.h"

namespace views {
class Label;
class ProgressBar;
class Throbber;
}  

namespace chromeos {

class ScreenObserver;
class UpdateController;

class UpdateView : public views::View {
 public:
  explicit UpdateView(ScreenObserver* observer);
  virtual ~UpdateView();

  void Init();
  void Reset();
  void UpdateLocalizedStrings();

  
  void set_controller(UpdateController* controller) {
    controller_ = controller;
  }

  
  void AddProgress(int progress);

  
  void SetProgress(int progress);

  
  
  void ShowManualRebootInfo();

  
  void ShowPreparingUpdatesInfo(bool visible);

  
  
  void ShowCurtain(bool show_curtain);

  
  virtual void Layout();

 private:
  
  void InitLabel(views::Label** label);

  
  void UpdateVisibility();

  
  views::Accelerator escape_accelerator_;

  
  views::Label* installing_updates_label_;
  views::Label* preparing_updates_label_;
  views::Label* reboot_label_;
  views::Label* manual_reboot_label_;
  views::Label* escape_to_skip_label_;
  views::ProgressBar* progress_bar_;

  
  views::Label* checking_label_;
  views::Throbber* throbber_;

  
  bool show_curtain_;

  
  bool show_manual_reboot_label_;

  
  bool show_preparing_updates_label_;

  
  chromeos::ScreenObserver* observer_;
  
  chromeos::UpdateController* controller_;

  DISALLOW_COPY_AND_ASSIGN(UpdateView);
};

}  

#endif  
