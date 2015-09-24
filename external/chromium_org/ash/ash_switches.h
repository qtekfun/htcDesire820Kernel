// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ASH_SWITCHES_H_
#define ASH_ASH_SWITCHES_H_

#include "ash/ash_export.h"

#include "build/build_config.h"

namespace ash {
namespace switches {


ASH_EXPORT extern const char kAshAnimateFromBootSplashScreen[];
ASH_EXPORT extern const char kAshConstrainPointerToRoot[];
ASH_EXPORT extern const char kAshCopyHostBackgroundAtBoot[];
ASH_EXPORT extern const char kAshDebugShortcuts[];
ASH_EXPORT extern const char kAshDebugShowPreferredNetworks[];
ASH_EXPORT extern const char kAshDefaultWallpaperIsOem[];
ASH_EXPORT extern const char kAshDefaultWallpaperLarge[];
ASH_EXPORT extern const char kAshDefaultWallpaperSmall[];
ASH_EXPORT extern const char kAshDisableAlternateShelfLayout[];
#if defined(OS_CHROMEOS)
ASH_EXPORT extern const char kAshDisableAudioDeviceMenu[];
#endif
ASH_EXPORT extern const char kAshDisableAlternateFrameCaptionButtonStyle[];
ASH_EXPORT extern const char kAshDisableAutoMaximizing[];
ASH_EXPORT extern const char kAshDisableDisplayChangeLimiter[];
ASH_EXPORT extern const char kAshDisableDragOffShelf[];
ASH_EXPORT extern const char kAshDisableOverviewMode[];
ASH_EXPORT extern const char kAshDisableDragAndDropAppListToLauncher[];
#if defined(OS_CHROMEOS)
ASH_EXPORT extern const char kAshDisableUsbChargerNotification[];
ASH_EXPORT extern const char kAshEnableAudioDeviceMenu[];
#endif
ASH_EXPORT extern const char kAshEnableAdvancedGestures[];
ASH_EXPORT extern const char kAshEnableAlternateFrameCaptionButtonStyle[];
ASH_EXPORT extern const char kAshEnableBrightnessControl[];
ASH_EXPORT extern const char kAshEnableDockedWindows[];
ASH_EXPORT extern const char kAshEnableImmersiveFullscreenForAllWindows[];
#if defined(OS_CHROMEOS)
ASH_EXPORT extern const char kAshEnableFullMultiProfileMode[];
#endif
#if defined(OS_LINUX)
ASH_EXPORT extern const char kAshEnableMemoryMonitor[];
#endif
#if defined(OS_CHROMEOS)
ASH_EXPORT extern const char kAshEnableMultiUserTray[];
#endif
ASH_EXPORT extern const char kAshEnableOak[];
ASH_EXPORT extern const char kAshEnableSoftwareMirroring[];
ASH_EXPORT extern const char kAshEnableStickyEdges[];
ASH_EXPORT extern const char kAshEnableTrayDragging[];
ASH_EXPORT extern const char kAshForceMirrorMode[];
ASH_EXPORT extern const char kAshGuestWallpaperLarge[];
ASH_EXPORT extern const char kAshGuestWallpaperSmall[];
ASH_EXPORT extern const char kAshHideNotificationsForFactory[];
ASH_EXPORT extern const char kAshHostWindowBounds[];
ASH_EXPORT extern const char kAshOverviewDelayOnAltTab[];
ASH_EXPORT extern const char kAshSecondaryDisplayLayout[];
ASH_EXPORT extern const char kAshMultipleSnapWindowWidths[];
ASH_EXPORT extern const char kAshTouchHud[];
ASH_EXPORT extern const char kAshUseAlternateShelfLayout[];
ASH_EXPORT extern const char kAshUseFirstDisplayAsInternal[];
ASH_EXPORT extern const char kAuraLegacyPowerButton[];
#if defined(OS_WIN)
ASH_EXPORT extern const char kForceAshToDesktop[];
#endif
ASH_EXPORT extern const char kShowShelfAlignmentMenu[];
ASH_EXPORT extern const char kHideShelfAlignmentMenu[];

ASH_EXPORT bool UseAlternateFrameCaptionButtonStyle();

ASH_EXPORT bool UseAlternateShelfLayout();

ASH_EXPORT bool UseDragOffShelf();

ASH_EXPORT bool ShowShelfAlignmentMenu();

ASH_EXPORT bool UseFullMultiProfileMode();

ASH_EXPORT bool UseMultiUserTray();

ASH_EXPORT bool UseOverviewMode();

ASH_EXPORT bool UseDockedWindows();

#if defined(OS_CHROMEOS)
ASH_EXPORT bool UseNewAudioHandler();

ASH_EXPORT bool ShowAudioDeviceMenu();

ASH_EXPORT bool UseUsbChargerNotification();
#endif

}  
}  

#endif  
