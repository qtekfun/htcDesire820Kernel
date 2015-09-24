// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_CONSTANTS_H_
#define UI_MESSAGE_CENTER_VIEWS_CONSTANTS_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/size.h"
#include "ui/message_center/message_center_style.h"

namespace message_center {

const SkColor kRegularTextBackgroundColor = SK_ColorWHITE;
const SkColor kDimTextBackgroundColor = SK_ColorWHITE;
const SkColor kContextTextBackgroundColor = SK_ColorWHITE;

const int kIconSize = message_center::kNotificationIconSize;
const int kLegacyIconSize = 40;
const int kTextLeftPadding = kIconSize + message_center::kIconToTextPadding;
const int kTextBottomPadding = 12;
const int kTextRightPadding = 23;
const int kItemTitleToMessagePadding = 3;
const int kButtonVecticalPadding = 0;
const int kButtonTitleTopPadding = 0;

const size_t kTitleCharacterLimit =
    message_center::kNotificationWidth * message_center::kTitleLineLimit / 4;
const size_t kMessageCharacterLimit =
    message_center::kNotificationWidth *
        message_center::kMessageExpandedLineLimit / 3;
const size_t kContextMessageCharacterLimit =
    message_center::kNotificationWidth *
    message_center::kContextMessageLineLimit / 3;

}  

#endif 
