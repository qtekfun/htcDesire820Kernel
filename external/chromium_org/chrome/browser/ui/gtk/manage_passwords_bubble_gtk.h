// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_MANAGE_PASSWORDS_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_MANAGE_PASSWORDS_BUBBLE_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"

typedef struct _GtkWidget GtkWidget;

class FullscreenController;

namespace content {
class WebContents;
}

class ManagePasswordsBubbleGtk {
 public:
  
  
  
  
  static void ShowBubble(content::WebContents* web_contents);

  
  static bool IsShowing();

  
  static void CloseBubble();

 private:
  ManagePasswordsBubbleGtk(GtkWidget* anchor,
                           content::WebContents* web_contents,
                           FullscreenController* fullscreen_controller);

  virtual ~ManagePasswordsBubbleGtk();

  
  void Close();

  
  CHROMEGTK_CALLBACK_0(ManagePasswordsBubbleGtk, void, OnDestroy);

  
  CHROMEGTK_CALLBACK_0(ManagePasswordsBubbleGtk, void, OnSaveButtonClicked);

  
  CHROMEGTK_CALLBACK_0(ManagePasswordsBubbleGtk, void, OnNotNowButtonClicked);

  
  
  content::WebContents* web_contents_;

  
  BubbleGtk* bubble_;

  DISALLOW_COPY_AND_ASSIGN(ManagePasswordsBubbleGtk);
};

#endif  
