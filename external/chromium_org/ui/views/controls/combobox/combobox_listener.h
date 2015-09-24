// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_LISTENER_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_LISTENER_H_

namespace views {

class Combobox;

class ComboboxListener {
 public:
  virtual void OnSelectedIndexChanged(Combobox* combobox) = 0;

  
  
  virtual void OnComboboxTextButtonClicked(Combobox* combobox) {}

 protected:
  virtual ~ComboboxListener() {}
};

}  

#endif  