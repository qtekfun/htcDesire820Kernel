// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_HIDABLE_AREA_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_HIDABLE_AREA_H_

#include "base/memory/scoped_ptr.h"
#include "ui/views/widget/widget.h"

namespace chromeos {
namespace input_method {

class HidableArea : public views::View {
 public:
  HidableArea();
  virtual ~HidableArea();

  
  void SetContents(views::View* contents);

  
  void Show();

  
  void Hide();

  
  bool IsShown() const;

  
  views::View* contents() {
    return contents_.get();
  }

 private:
  scoped_ptr<views::View> contents_;
  scoped_ptr<views::View> place_holder_;

  DISALLOW_COPY_AND_ASSIGN(HidableArea);
};

}  
}  

#endif  
