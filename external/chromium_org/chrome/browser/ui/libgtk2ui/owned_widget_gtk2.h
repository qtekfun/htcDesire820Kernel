// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_LIBGTK2UI_OWNED_WIDGET_GTK2_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_OWNED_WIDGET_GTK2_H_

#include "base/basictypes.h"

typedef struct _GtkWidget GtkWidget;

namespace libgtk2ui {

class OwnedWidgetGtk {
 public:
  
  OwnedWidgetGtk() : widget_(NULL) { }
  
  explicit OwnedWidgetGtk(GtkWidget* widget) : widget_(NULL) { Own(widget); }

  ~OwnedWidgetGtk();

  
  GtkWidget* get() const { return widget_; }
  GtkWidget* operator->() const { return widget_; }

  
  
  
  
  
  void Own(GtkWidget* widget);

  
  
  
  
  
  
  void Destroy();

 private:
  GtkWidget* widget_;

  DISALLOW_COPY_AND_ASSIGN(OwnedWidgetGtk);
};

}  

#endif  
