// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_MENU_MODEL_DELEGATE_H_
#define UI_BASE_MODELS_MENU_MODEL_DELEGATE_H_

namespace ui {

class MenuModelDelegate {
 public:
  
  virtual void OnIconChanged(int index) = 0;

  
  
  virtual void OnMenuStructureChanged() {}

 protected:
  virtual ~MenuModelDelegate() {}
};

}  

#endif  
