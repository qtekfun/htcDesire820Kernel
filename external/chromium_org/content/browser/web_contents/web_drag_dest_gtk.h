// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_DEST_GTK_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_DEST_GTK_H_

#include <gtk/gtk.h>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/public/common/drop_data.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "ui/base/gtk/gtk_signal.h"

namespace content {

class RenderViewHostImpl;
class WebContents;
class WebDragDestDelegate;

class CONTENT_EXPORT WebDragDestGtk {
 public:
  WebDragDestGtk(WebContents* web_contents, GtkWidget* widget);
  ~WebDragDestGtk();

  DropData* current_drop_data() const { return drop_data_.get(); }

  
  
  void UpdateDragStatus(blink::WebDragOperation operation);

  
  
  void DragLeave();

  WebDragDestDelegate* delegate() const { return delegate_; }
  void set_delegate(WebDragDestDelegate* delegate) { delegate_ = delegate; }

  GtkWidget* widget() const { return widget_; }

 private:
  RenderViewHostImpl* GetRenderViewHost() const;

  
  
  
  CHROMEGTK_CALLBACK_4(WebDragDestGtk, gboolean, OnDragMotion, GdkDragContext*,
                       gint, gint, guint);

  
  
  
  
  
  CHROMEGTK_CALLBACK_6(WebDragDestGtk, void, OnDragDataReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);

  
  CHROMEGTK_CALLBACK_2(WebDragDestGtk, void, OnDragLeave, GdkDragContext*,
                       guint);

  
  CHROMEGTK_CALLBACK_4(WebDragDestGtk, gboolean, OnDragDrop, GdkDragContext*,
                       gint, gint, guint);

  WebContents* web_contents_;

  
  GtkWidget* widget_;

  
  
  GdkDragContext* context_;

  
  scoped_ptr<DropData> drop_data_;

  
  
  int data_requests_;

  
  gint drag_over_time_;

  
  
  bool is_drop_target_;

  
  
  
  
  scoped_ptr<int[]> handlers_;

  
  WebDragDestDelegate* delegate_;

  
  bool canceled_;

  base::WeakPtrFactory<WebDragDestGtk> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebDragDestGtk);
};

}  

#endif  
