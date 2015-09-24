// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TAB_CONTENTS_CONTAINER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TAB_CONTENTS_CONTAINER_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/gtk/view_id_util.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/gtk/owned_widget_gtk.h"

class StatusBubbleGtk;

typedef struct _GtkFloatingContainer GtkFloatingContainer;

class TabContentsContainerGtk : protected content::WebContentsObserver,
                                public ViewIDUtil::Delegate {
 public:
  TabContentsContainerGtk(StatusBubbleGtk* status_bubble,
                          bool embed_fullscreen_widget);
  virtual ~TabContentsContainerGtk();

  
  void SetTab(content::WebContents* tab);
  content::WebContents* tab() const { return web_contents(); }

  
  void DetachTab(content::WebContents* tab);

  GtkWidget* widget() { return floating_.get(); }

  
  virtual GtkWidget* GetWidgetForViewID(ViewID id) OVERRIDE;

 private:
  
  virtual void WebContentsDestroyed(content::WebContents* contents) OVERRIDE;
  virtual void DidShowFullscreenWidget(int routing_id) OVERRIDE;
  virtual void DidDestroyFullscreenWidget(int routing_id) OVERRIDE;

  
  
  static void OnSetFloatingPosition(
      GtkFloatingContainer* container, GtkAllocation* allocation,
      TabContentsContainerGtk* tab_contents_container);

  
  void PackTab();

  
  void HideTab();

  
  StatusBubbleGtk* status_bubble_;

  
  
  
  
  ui::OwnedWidgetGtk floating_;

  
  
  
  
  GtkWidget* expanded_;

  
  
  bool should_embed_fullscreen_widgets_;

  
  
  bool is_embedding_fullscreen_widget_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsContainerGtk);
};

#endif  
