// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_VIEW_TYPES_H_
#define CHROME_COMMON_VIEW_TYPES_H_
#pragma once

#include "base/basictypes.h"

class ViewType {
 public:
  enum Type {
    INVALID,
    BACKGROUND_CONTENTS,
    TAB_CONTENTS,
    EXTENSION_BACKGROUND_PAGE,
    EXTENSION_POPUP,
    EXTENSION_INFOBAR,
    DEV_TOOLS_UI,
    INTERSTITIAL_PAGE,
    NOTIFICATION,
  };

  
  
  static const char* kTabContents;
  static const char* kBackgroundPage;
  static const char* kPopup;
  static const char* kInfobar;
  static const char* kNotification;
  static const char* kAll;

 private:
  
  ViewType() {}

  DISALLOW_COPY_AND_ASSIGN(ViewType);
};

#endif  
