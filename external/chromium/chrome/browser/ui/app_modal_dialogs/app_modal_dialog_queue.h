// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_QUEUE_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_QUEUE_H_
#pragma once

#include <queue>

#include "chrome/browser/ui/app_modal_dialogs/app_modal_dialog.h"

template <typename T> struct DefaultSingletonTraits;

class AppModalDialogQueue {
 public:
  
  static AppModalDialogQueue* GetInstance();

  
  
  
  
  
  
  
  
  
  
  void AddDialog(AppModalDialog* dialog);

  
  
  
  
  void ShowNextDialog();

  
  
  
  
  
  
  void ActivateModalDialog();

  
  bool HasActiveDialog() {
    return active_dialog_ != NULL;
  }

  
  AppModalDialog* active_dialog() {
    return active_dialog_;
  }

 private:
  friend struct DefaultSingletonTraits<AppModalDialogQueue>;

  AppModalDialogQueue();
  ~AppModalDialogQueue();

  
  void ShowModalDialog(AppModalDialog* dialog);

  
  
  
  
  AppModalDialog* GetNextDialog();

  
  
  std::queue<AppModalDialog*> app_modal_dialog_queue_;

  
  
  AppModalDialog* active_dialog_;

  
  
  bool showing_modal_dialog_;

  DISALLOW_COPY_AND_ASSIGN(AppModalDialogQueue);
};

#endif  
