// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_STYLE_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_STYLE_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/size.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {


const int kNotificationButtonIconSize = 16;
const int kNotificationIconSize = 80;
const int kNotificationImageBorderSize = 10;
const int kNotificationPreferredImageWidth = 360;
const int kNotificationPreferredImageHeight = 240;
const int kSettingsIconSize = 16;

const size_t kMaxVisibleMessageCenterNotifications = 100;
const size_t kMaxVisiblePopupNotifications = 3;

const int kNotificationWidth = 360;

MESSAGE_CENTER_EXPORT extern const SkColor kMessageCenterBorderColor;
MESSAGE_CENTER_EXPORT extern const SkColor kMessageCenterShadowColor;

namespace settings {

const SkColor kEntrySeparatorColor = SkColorSetARGB(0.1 * 255, 0, 0, 0);
const int kEntryHeight = 45;
const int kEntrySeparatorHeight = 1;
const int kHorizontalMargin = 10;
const int kTopMargin = 20;
const int kTitleToDescriptionSpace = 20;
const int kEntryIconSize = 16;
const int kDescriptionToSwitcherSpace = 15;
const int kInternalHorizontalSpacing = 10;
const int kCheckboxSizeWithPadding = 24;

}  



const int kControlButtonSize = 29;  
const int kIconToTextPadding = 16;  
const int kTextTopPadding = 12;     
const int kIconBottomPadding = 16;  
                                    

const int kTitleFontSize = 14;             
const int kTitleLineHeight = 20;           
const int kMessageFontSize = 12;           
const int kMessageLineHeight = 18;         

extern const SkColor kNotificationBackgroundColor; 
extern const SkColor kImageBackgroundColor;        
extern const SkColor kIconBackgroundColor;         
                                                   
extern const SkColor kRegularTextColor;            
extern const SkColor kDimTextColor;
extern const SkColor kFocusBorderColor;  


gfx::Size GetImageSizeForWidth(int width, const gfx::Size& image_size);

extern const int kNotificationMaximumImageHeight;  
extern const size_t kNotificationMaximumItems;     

extern const int kAutocloseDefaultDelaySeconds;
extern const int kAutocloseHighPriorityDelaySeconds;

const int kButtonHeight = 38;              
const int kButtonHorizontalPadding = 16;   
const int kButtonIconTopPadding = 11;      
const int kButtonIconToTitlePadding = 16;  

#if !defined(OS_LINUX) || defined(USE_AURA)
const SkColor kButtonSeparatorColor = SkColorSetRGB(234, 234, 234);
const SkColor kHoveredButtonBackgroundColor = SkColorSetRGB(243, 243, 243);
#endif

const int kProgressBarThickness = 5;
const int kProgressBarTopPadding = 16;
const int kProgressBarCornerRadius = 3;
const SkColor kProgressBarBackgroundColor = SkColorSetRGB(216, 216, 216);
const SkColor kProgressBarSliceColor = SkColorSetRGB(120, 120, 120);

const int kTitleLineLimit = 3;
const int kExperimentalTitleLineLimit = 1;
const int kMessageCollapsedLineLimit = 2;
const int kMessageExpandedLineLimit = 7;
const int kContextMessageLineLimit = 1;


const int kMarginBetweenItems = 10;  
                                     

extern const SkColor kBackgroundLightColor;  
extern const SkColor kBackgroundDarkColor;   

extern const SkColor kShadowColor;           

extern const SkColor kMessageCenterBackgroundColor;
extern const SkColor kFooterDelimiterColor;  
extern const SkColor kFooterTextColor;       

}  

#endif  
