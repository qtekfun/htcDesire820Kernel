// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_FOCUS_STORE_GTK_H_
#define UI_BASE_GTK_FOCUS_STORE_GTK_H_

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

typedef struct _GtkWidget GtkWidget;

namespace ui {

class UI_EXPORT FocusStoreGtk {
 public:
  FocusStoreGtk();
  ~FocusStoreGtk();

  GtkWidget* widget() const { return widget_; }

  
  
  void Store(GtkWidget* widget);

  
  void SetWidget(GtkWidget* widget);

 private:
  
  void DisconnectDestroyHandler();

  
  GtkWidget* widget_;

  
  
  
  unsigned int destroy_handler_id_;

  DISALLOW_COPY_AND_ASSIGN(FocusStoreGtk);
};

}  

#endif  
