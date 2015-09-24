// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_QUEUE_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_APP_MODAL_DIALOG_QUEUE_H_

#include <deque>

#include "base/basictypes.h"

class AppModalDialog;

template <typename T> struct DefaultSingletonTraits;

class AppModalDialogQueue {
 public:
  typedef std::deque<AppModalDialog*>::iterator iterator;

  
  static AppModalDialogQueue* GetInstance();

  
  
  
  
  
  
  
  void AddDialog(AppModalDialog* dialog);

  
  
  
  
  void ShowNextDialog();

  
  
  
  
  
  
  void ActivateModalDialog();

  
  bool HasActiveDialog() const;

  AppModalDialog* active_dialog() { return active_dialog_; }

  
  
  iterator begin() { return app_modal_dialog_queue_.begin(); }
  iterator end() { return app_modal_dialog_queue_.end(); }

 private:
  friend struct DefaultSingletonTraits<AppModalDialogQueue>;

  AppModalDialogQueue();
  ~AppModalDialogQueue();

  
  void ShowModalDialog(AppModalDialog* dialog);

  
  
  
  
  AppModalDialog* GetNextDialog();

  
  
  std::deque<AppModalDialog*> app_modal_dialog_queue_;

  
  
  AppModalDialog* active_dialog_;

  
  
  bool showing_modal_dialog_;

  DISALLOW_COPY_AND_ASSIGN(AppModalDialogQueue);
};

#endif  
