// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_RESIZE_AREA_DELEGATE_H_
#define UI_VIEWS_CONTROLS_RESIZE_AREA_DELEGATE_H_

namespace views {

class ResizeAreaDelegate {
 public:
  
  
  
  
  
  
  virtual void OnResize(int resize_amount, bool done_resizing) = 0;

 protected:
  virtual ~ResizeAreaDelegate() {}
};

}  

#endif  
