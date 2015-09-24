// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEW_MODEL_UTILS_H_
#define UI_VIEWS_VIEW_MODEL_UTILS_H_

#include "base/basictypes.h"
#include "ui/views/views_export.h"

namespace views {

class View;
class ViewModel;

class VIEWS_EXPORT ViewModelUtils {
 public:
  enum Alignment {
    HORIZONTAL,
    VERTICAL
  };

  
  static void SetViewBoundsToIdealBounds(const ViewModel& model);

  
  static bool IsAtIdealBounds(const ViewModel& model);

  
  static int DetermineMoveIndex(const ViewModel& model,
                                View* view,
                                Alignment alignment,
                                int x,
                                int y);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ViewModelUtils);
};

}  

#endif  
