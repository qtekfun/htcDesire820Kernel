// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_PASSPHRASE_TEXTFIELD_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_PASSPHRASE_TEXTFIELD_H_

#include <string>

#include "ui/views/controls/textfield/textfield.h"

namespace chromeos {

class PassphraseTextfield : public views::Textfield {
 public:
  PassphraseTextfield();

  
  void SetShowFake(bool show_fake);

  
  
  
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  std::string GetPassphrase();

  bool show_fake() const { return show_fake_; }

 private:
  void SetFakePassphrase();
  void ClearFakePassphrase();

  bool show_fake_;
  bool changed_;
};

}  

#endif  
