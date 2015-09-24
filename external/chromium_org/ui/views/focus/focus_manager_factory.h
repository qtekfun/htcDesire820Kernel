// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_FOCUS_MANAGER_FACTORY_H_
#define UI_VIEWS_FOCUS_FOCUS_MANAGER_FACTORY_H_

#include "base/basictypes.h"
#include "ui/views/views_export.h"

namespace views {

class FocusManager;
class Widget;

class VIEWS_EXPORT FocusManagerFactory {
 public:
  
  static FocusManager* Create(Widget* widget, bool desktop_widget);

  
  
  static void Install(FocusManagerFactory* factory);

 protected:
  FocusManagerFactory();
  virtual ~FocusManagerFactory();

  
  
  
  virtual FocusManager* CreateFocusManager(Widget* widget,
                                           bool desktop_widget) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(FocusManagerFactory);
};

}  

#endif  
