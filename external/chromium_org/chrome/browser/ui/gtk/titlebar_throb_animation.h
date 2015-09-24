// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TITLEBAR_THROB_ANIMATION_H_
#define CHROME_BROWSER_UI_GTK_TITLEBAR_THROB_ANIMATION_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/gtk_util.h"

class TitlebarThrobAnimation {
 public:
  TitlebarThrobAnimation();

  
  
  
  GdkPixbuf* GetNextFrame(bool is_waiting);

  
  void Reset();

 private:
  
  static void InitFrames();

  int current_frame_;
  int current_waiting_frame_;

  DISALLOW_COPY_AND_ASSIGN(TitlebarThrobAnimation);
};

#endif  
