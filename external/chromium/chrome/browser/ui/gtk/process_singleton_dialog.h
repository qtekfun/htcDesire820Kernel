// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_PROCESS_SINGLETON_DIALOG_H_
#define CHROME_BROWSER_UI_GTK_PROCESS_SINGLETON_DIALOG_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "ui/base/gtk/gtk_signal.h"

typedef struct _GtkWidget GtkWidget;

class ProcessSingletonDialog {
 public:
  
  static void ShowAndRun(const std::string& message);

 private:
  explicit ProcessSingletonDialog(const std::string& message);

  CHROMEGTK_CALLBACK_1(ProcessSingletonDialog, void, OnResponse, int);

  GtkWidget* dialog_;

  DISALLOW_COPY_AND_ASSIGN(ProcessSingletonDialog);
};

#endif  