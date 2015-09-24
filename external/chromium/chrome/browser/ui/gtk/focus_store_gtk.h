// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_FOCUS_STORE_GTK_H_
#define CHROME_BROWSER_UI_GTK_FOCUS_STORE_GTK_H_
#pragma once

#include "base/basictypes.h"

typedef struct _GtkWidget GtkWidget;

class FocusStoreGtk {
 public:
  FocusStoreGtk();

  virtual ~FocusStoreGtk();

  GtkWidget* widget() const { return widget_; }

  
  
  void Store(GtkWidget* widget);

  
  void SetWidget(GtkWidget* widget);

 private:
  
  void DisconnectDestroyHandler();

  
  GtkWidget* widget_;

  
  
  
  unsigned int destroy_handler_id_;

  DISALLOW_COPY_AND_ASSIGN(FocusStoreGtk);
};

#endif  
