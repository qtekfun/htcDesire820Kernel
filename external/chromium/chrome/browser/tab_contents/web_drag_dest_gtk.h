// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_WEB_DRAG_DEST_GTK_H_
#define CHROME_BROWSER_TAB_CONTENTS_WEB_DRAG_DEST_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebDragOperation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "webkit/glue/webdropdata.h"

class TabContents;

class WebDragDestGtk {
 public:
  WebDragDestGtk(TabContents* tab_contents, GtkWidget* widget);
  virtual ~WebDragDestGtk();

  
  
  void UpdateDragStatus(WebKit::WebDragOperation operation);

  
  
  void DragLeave();

 private:
  
  
  
  CHROMEGTK_CALLBACK_4(WebDragDestGtk, gboolean, OnDragMotion, GdkDragContext*,
                       gint, gint, guint);

  
  
  
  
  
  CHROMEGTK_CALLBACK_6(WebDragDestGtk, void, OnDragDataReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);

  
  CHROMEGTK_CALLBACK_2(WebDragDestGtk, void, OnDragLeave, GdkDragContext*,
                       guint);

  
  CHROMEGTK_CALLBACK_4(WebDragDestGtk, gboolean, OnDragDrop, GdkDragContext*,
                       gint, gint, guint);

  TabContents* tab_contents_;
  
  GtkWidget* widget_;
  
  
  GdkDragContext* context_;
  
  scoped_ptr<WebDropData> drop_data_;

  
  
  int data_requests_;

  
  gint drag_over_time_;

  
  
  bool is_drop_target_;

  
  
  
  
  int destroy_handler_;

  
  
  BookmarkNodeData bookmark_drag_data_;

  ScopedRunnableMethodFactory<WebDragDestGtk> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebDragDestGtk);
};

#endif  
