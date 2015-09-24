// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LAYOUT_LAYOUT_MANAGER_H_
#define UI_VIEWS_LAYOUT_LAYOUT_MANAGER_H_

#include "ui/views/views_export.h"

namespace gfx {
class Size;
}

namespace views {

class View;

class VIEWS_EXPORT LayoutManager {
 public:
  virtual ~LayoutManager();

  
  
  virtual void Installed(View* host);

  
  
  virtual void Uninstalled(View* host);

  
  
  
  virtual void Layout(View* host) = 0;

  
  
  virtual gfx::Size GetPreferredSize(View* host) = 0;

  
  
  virtual int GetPreferredHeightForWidth(View* host, int width);

  
  virtual void ViewAdded(View* host, View* view);

  
  virtual void ViewRemoved(View* host, View* view);
};

}  

#endif  
