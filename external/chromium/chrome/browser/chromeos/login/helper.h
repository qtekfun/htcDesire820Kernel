// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_HELPER_H_
#pragma once

#include "base/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "views/controls/button/native_button.h"
#include "views/view.h"
#include "views/widget/widget_gtk.h"

class GURL;

namespace gfx {
class Rect;
class Size;
}  

namespace views {
class Label;
class MenuButton;
class Painter;
class SmoothedThrobber;
class Textfield;
class Throbber;
class Widget;
}  

namespace chromeos {

class NetworkLibrary;

class ThrobberHostView : public views::View {
 public:
  ThrobberHostView();
  virtual ~ThrobberHostView();

  
  
  
  virtual void StartThrobber();

  
  virtual void StopThrobber();

 protected:
  
  
  virtual gfx::Rect CalculateThrobberBounds(views::Throbber* throbber);

  void set_host_view(views::View* host_view) {
    host_view_ = host_view;
  }

 private:
  
  views::View* host_view_;

  
  views::Widget* throbber_widget_;

  DISALLOW_COPY_AND_ASSIGN(ThrobberHostView);
};

views::SmoothedThrobber* CreateDefaultSmoothedThrobber();

views::Throbber* CreateDefaultThrobber();

views::Painter* CreateBackgroundPainter();

gfx::Rect CalculateScreenBounds(const gfx::Size& size);

void CorrectLabelFontSize(views::Label* label);

void CorrectMenuButtonFontSize(views::MenuButton* button);

void CorrectNativeButtonFontSize(views::NativeButton* button);

void CorrectTextfieldFontSize(views::Textfield* textfield);

void SetAndCorrectTextfieldFont(views::Textfield* textfield,
                                const gfx::Font& font);

GURL GetAccountRecoveryHelpUrl();

string16 GetCurrentNetworkName(NetworkLibrary* network_library);

namespace login {

enum Command {
  SIGN_OUT,
};

const int kBorderSize = 10;

const int kUserImageSize = 256;

const SkColor kBackgroundColor = SK_ColorWHITE;

const SkColor kTextColor = SK_ColorWHITE;

const SkColor kLinkColor = 0xFF0066CC;

const int kThrobberRightMargin = 10;

const int kWizardScreenWidth = 800;
const int kWizardScreenHeight = 450;

const int kScreenCornerRadius = 10;
const int kUserCornerRadius = 6;

const int kSelectedLabelHeight = 25;
const int kUnselectedLabelHeight = 20;

const int kButtonMinWidth = 90;

class WideButton : public views::NativeButton {
 public:
  WideButton(views::ButtonListener* listener, const std::wstring& text)
      : NativeButton(listener, text) {
    CorrectNativeButtonFontSize(this);
  }

  ~WideButton() {}

 private:
  virtual gfx::Size GetPreferredSize();

  DISALLOW_COPY_AND_ASSIGN(WideButton);
};

}  

#if defined(CROS_FONTS_USING_BCI)
const int kFontSizeCorrectionDelta = 1;
const int kNetworkSelectionLabelFontDelta = 1;
const int kSelectedUsernameFontDelta = 1;
const int kUnselectedUsernameFontDelta = 1;
const int kWelcomeTitleFontDelta = 8;
const int kLoginTitleFontDelta = 3;
#else
const int kFontSizeCorrectionDelta = 2;
const int kNetworkSelectionLabelFontDelta = 1;
const int kSelectedUsernameFontDelta = 1;
const int kUnselectedUsernameFontDelta = 2;
const int kWelcomeTitleFontDelta = 9;
const int kLoginTitleFontDelta = 4;
#endif

const int kNewUserPodFullWidth = 372;
const int kNewUserPodFullHeight = 372;
const int kNewUserPodSmallWidth = 360;
const int kNewUserPodSmallHeight = 322;

}  

#endif  
