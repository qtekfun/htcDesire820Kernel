// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_SYSTEM_NOTIFIER_H_
#define ASH_SYSTEM_SYSTEM_NOTIFIER_H_

#include <string>

#include "ash/ash_export.h"
#include "ui/message_center/notifier_settings.h"

namespace ash {
namespace system_notifier {

ASH_EXPORT extern const char kNotifierDisplay[];
ASH_EXPORT extern const char kNotifierDisplayResolutionChange[];
ASH_EXPORT extern const char kNotifierDisplayError[];
ASH_EXPORT extern const char kNotifierInputMethod[];
ASH_EXPORT extern const char kNotifierLocale[];
ASH_EXPORT extern const char kNotifierLocallyManagedUser[];
ASH_EXPORT extern const char kNotifierMultiProfileFirstRun[];
ASH_EXPORT extern const char kNotifierNetwork[];
ASH_EXPORT extern const char kNotifierNetworkError[];
ASH_EXPORT extern const char kNotifierScreenshot[];
ASH_EXPORT extern const char kNotifierScreenCapture[];
ASH_EXPORT extern const char kNotifierScreenShare[];
ASH_EXPORT extern const char kNotifierSessionLengthTimeout[];
ASH_EXPORT extern const char kNotifierPower[];

ASH_EXPORT bool ShouldAlwaysShowPopups(
    const message_center::NotifierId& notifier_id);

ASH_EXPORT bool IsAshSystemNotifier(
    const message_center::NotifierId& notifier_id);

}  
}  

#endif  
