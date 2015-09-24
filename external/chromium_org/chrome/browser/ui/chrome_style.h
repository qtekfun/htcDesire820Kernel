// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_CHROME_STYLE_H_
#define CHROME_BROWSER_UI_CHROME_STYLE_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/resource/resource_bundle.h"



namespace chrome_style {

int GetCloseButtonSize();  
SkColor GetBackgroundColor();  
SkColor GetLinkColor();  

const int kTitleTopPadding = 15; 
const int kHorizontalPadding = 20; 
const int kClientBottomPadding = 20; 
const int kCloseButtonPadding = 7; 
const int kBorderRadius = 2; 
const int kRowPadding = 20; 

const ui::ResourceBundle::FontStyle kTextFontStyle =
    ui::ResourceBundle::BaseFont;
const ui::ResourceBundle::FontStyle kTitleFontStyle =
    ui::ResourceBundle::MediumFont;

}  

#endif  
