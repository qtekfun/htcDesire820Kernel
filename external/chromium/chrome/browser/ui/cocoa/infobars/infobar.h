// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_H_
#pragma once

#include "base/logging.h"  

@class InfoBarController;


class InfoBar {
 public:
  InfoBar(InfoBarController* controller) {
    DCHECK(controller);
    controller_ = controller;
  }

  InfoBarController* controller() {
    return controller_;
  }

 private:
  
  InfoBarController* controller_;  

  DISALLOW_COPY_AND_ASSIGN(InfoBar);
};

#endif  
