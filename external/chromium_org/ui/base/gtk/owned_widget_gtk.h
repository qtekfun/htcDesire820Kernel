// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_BASE_GTK_OWNED_WIDGET_GTK_H_
#define UI_BASE_GTK_OWNED_WIDGET_GTK_H_

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

typedef struct _GtkWidget GtkWidget;

namespace ui {

class UI_EXPORT OwnedWidgetGtk {
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
