// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_PREFIX_DELEGATE_H_
#define UI_VIEWS_CONTROLS_PREFIX_DELEGATE_H_

#include "ui/views/view.h"

namespace views {

class VIEWS_EXPORT PrefixDelegate : public View {
 public:
  
  virtual int GetRowCount() = 0;

  
  
  virtual int GetSelectedRow() = 0;

  
  virtual void SetSelectedRow(int row) = 0;

  
  virtual string16 GetTextForRow(int row) = 0;

 protected:
  virtual ~PrefixDelegate() {}
};

} 

#endif 
