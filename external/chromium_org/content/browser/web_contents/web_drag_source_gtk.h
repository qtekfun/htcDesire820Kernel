// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_SOURCE_GTK_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_DRAG_SOURCE_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/browser/web_contents.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/vector2d.h"
#include "url/gurl.h"

class SkBitmap;

namespace content {

class RenderViewHostImpl;
class WebContentsImpl;
struct DropData;

class CONTENT_EXPORT WebDragSourceGtk :
    public base::MessageLoopForUI::Observer {
 public:
  explicit WebDragSourceGtk(WebContents* web_contents);
  virtual ~WebDragSourceGtk();

  
  
  bool StartDragging(const DropData& drop_data,
                     blink::WebDragOperationsMask allowed_ops,
                     GdkEventButton* last_mouse_down,
                     const SkBitmap& image,
                     const gfx::Vector2d& image_offset);

  
  virtual void WillProcessEvent(GdkEvent* event) OVERRIDE;
  virtual void DidProcessEvent(GdkEvent* event) OVERRIDE;

 private:
  CHROMEGTK_CALLBACK_2(WebDragSourceGtk, gboolean, OnDragFailed,
                       GdkDragContext*, GtkDragResult);
  CHROMEGTK_CALLBACK_1(WebDragSourceGtk, void, OnDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(WebDragSourceGtk, void, OnDragEnd,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_4(WebDragSourceGtk, void, OnDragDataGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);
  CHROMEGTK_CALLBACK_1(WebDragSourceGtk, gboolean, OnDragIconExpose,
                       GdkEventExpose*);

  gfx::NativeView GetContentNativeView() const;

  
  WebContentsImpl* web_contents_;

  
  
  scoped_ptr<DropData> drop_data_;

  
  
  GdkPixbuf* drag_pixbuf_;
  gfx::Vector2d image_offset_;

  
  GdkAtom drag_file_mime_type_;

  
  
  bool drag_failed_;

  
  
  
  
  GtkWidget* drag_widget_;

  
  
  GdkDragContext* drag_context_;

  
  base::string16 wide_download_mime_type_;

  
  base::FilePath download_file_name_;

  
  GURL download_url_;

  
  
  
  GtkWidget* drag_icon_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(WebDragSourceGtk);
};

}  

#endif  
