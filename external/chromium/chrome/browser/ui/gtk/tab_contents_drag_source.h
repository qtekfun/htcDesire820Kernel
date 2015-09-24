// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TAB_CONTENTS_DRAG_SOURCE_H_
#define CHROME_BROWSER_UI_GTK_TAB_CONTENTS_DRAG_SOURCE_H_
#pragma once

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/message_loop.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebDragOperation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"

class SkBitmap;
class TabContents;
class TabContentsView;
struct WebDropData;

class TabContentsDragSource : public MessageLoopForUI::Observer {
 public:
  explicit TabContentsDragSource(TabContentsView* tab_contents_view);
  ~TabContentsDragSource();

  TabContents* tab_contents() const;

  
  
  void StartDragging(const WebDropData& drop_data,
                     WebKit::WebDragOperationsMask allowed_ops,
                     GdkEventButton* last_mouse_down,
                     const SkBitmap& image,
                     const gfx::Point& image_offset);

  
  virtual void WillProcessEvent(GdkEvent* event);
  virtual void DidProcessEvent(GdkEvent* event);

 private:
  CHROMEGTK_CALLBACK_2(TabContentsDragSource, gboolean, OnDragFailed,
                       GdkDragContext*, GtkDragResult);
  CHROMEGTK_CALLBACK_1(TabContentsDragSource, void, OnDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(TabContentsDragSource, void, OnDragEnd,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_4(TabContentsDragSource, void, OnDragDataGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);
  CHROMEGTK_CALLBACK_1(TabContentsDragSource, gboolean, OnDragIconExpose,
                       GdkEventExpose*);

  gfx::NativeView GetContentNativeView() const;

  
  TabContentsView* tab_contents_view_;

  
  
  scoped_ptr<WebDropData> drop_data_;

  
  
  GdkPixbuf* drag_pixbuf_;
  gfx::Point image_offset_;

  
  GdkAtom drag_file_mime_type_;

  
  
  bool drag_failed_;

  
  
  
  
  GtkWidget* drag_widget_;

  
  
  GdkDragContext* drag_context_;

  
  string16 wide_download_mime_type_;

  
  FilePath download_file_name_;

  
  GURL download_url_;

  
  
  
  GtkWidget* drag_icon_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsDragSource);
};

#endif  
