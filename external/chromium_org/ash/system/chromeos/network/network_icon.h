// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_ICON_H_
#define ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_ICON_H_

#include <string>

#include "ash/ash_export.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image_skia.h"

namespace chromeos {
class NetworkState;
}

namespace ash {
namespace network_icon {

class AnimationObserver;

enum IconType {
  ICON_TYPE_TRAY,  
  ICON_TYPE_DEFAULT_VIEW,  
  ICON_TYPE_LIST,  
};

ASH_EXPORT gfx::ImageSkia GetImageForNetwork(
    const chromeos::NetworkState* network,
    IconType icon_type);

ASH_EXPORT gfx::ImageSkia GetImageForConnectedNetwork(
    IconType icon_type,
    const std::string& network_type);

ASH_EXPORT gfx::ImageSkia GetImageForConnectingNetwork(
    IconType icon_type,
    const std::string& network_type);

ASH_EXPORT gfx::ImageSkia GetImageForDisconnectedNetwork(
    IconType icon_type,
    const std::string& network_type);

ASH_EXPORT base::string16 GetLabelForNetwork(
    const chromeos::NetworkState* network,
    IconType icon_type);

ASH_EXPORT int GetCellularUninitializedMsg();

ASH_EXPORT void GetDefaultNetworkImageAndLabel(IconType icon_type,
                                               gfx::ImageSkia* image,
                                               base::string16* label,
                                               bool* animating);

ASH_EXPORT void PurgeNetworkIconCache();

}  
}  

#endif  
