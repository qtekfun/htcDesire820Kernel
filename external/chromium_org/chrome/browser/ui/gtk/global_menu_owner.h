// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GLOBAL_MENU_OWNER_H_
#define CHROME_BROWSER_UI_GTK_GLOBAL_MENU_OWNER_H_

typedef struct _GtkWidget GtkWidget;

class GlobalMenuOwner {
 public:
  virtual ~GlobalMenuOwner() {}

  
  
  
  virtual void Init(GtkWidget* menu, GtkWidget* menu_item) = 0;
};

#endif  
