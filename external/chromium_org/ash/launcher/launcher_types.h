// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_LAUNCHER_LAUNCHER_TYPES_H_
#define ASH_LAUNCHER_LAUNCHER_TYPES_H_

#include <vector>

#include "ash/ash_export.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image_skia.h"

namespace ash {

typedef int LauncherID;

ASH_EXPORT extern const int kLauncherPreferredSize;

ASH_EXPORT extern const int kLauncherBackgroundAlpha;

extern const int kInvalidImageResourceID;

extern const int kInvalidLauncherID;

ASH_EXPORT extern const int kTimeToSwitchBackgroundMs;

enum LauncherItemType {
  
  TYPE_APP_PANEL,

  
  TYPE_APP_SHORTCUT,

  
  TYPE_APP_LIST,

  
  TYPE_BROWSER_SHORTCUT,

  
  TYPE_PLATFORM_APP,

  
  TYPE_WINDOWED_APP,

  
  TYPE_UNDEFINED,
};

enum LauncherItemStatus {
  
  STATUS_CLOSED,
  
  STATUS_RUNNING,
  
  STATUS_ACTIVE,
  
  STATUS_ATTENTION,
};

struct ASH_EXPORT LauncherItem {
  LauncherItem();
  ~LauncherItem();

  LauncherItemType type;

  
  gfx::ImageSkia image;

  
  LauncherID id;

  
  LauncherItemStatus status;
};

typedef std::vector<LauncherItem> LauncherItems;

enum CycleDirection {
  CYCLE_FORWARD,
  CYCLE_BACKWARD
};

struct ASH_EXPORT LauncherItemDetails {
  LauncherItemDetails();
  ~LauncherItemDetails();

  LauncherItemType type;

  
  int image_resource_id;

  
  base::string16 title;
};

}  

#endif  
