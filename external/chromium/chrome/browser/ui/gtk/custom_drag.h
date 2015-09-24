// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CUSTOM_DRAG_H_
#define CHROME_BROWSER_UI_GTK_CUSTOM_DRAG_H_
#pragma once

#include <gtk/gtk.h>
#include <vector>

#include "base/basictypes.h"
#include "ui/base/gtk/gtk_signal.h"

class BookmarkNode;
class DownloadItem;
class Profile;

namespace gfx {
class Image;
}

class CustomDrag {
 protected:
  explicit CustomDrag(gfx::Image* icon, int code_mask, GdkDragAction action);
  virtual ~CustomDrag();

  virtual void OnDragDataGet(GtkWidget* widget, GdkDragContext* context,
                             GtkSelectionData* selection_data,
                             guint target_type, guint time) = 0;

 private:
  CHROMEGTK_CALLBACK_1(CustomDrag, void, OnDragBegin, GdkDragContext*);
  CHROMEGTK_CALLBACK_1(CustomDrag, void, OnDragEnd, GdkDragContext*);

  
  static void OnDragDataGetThunk(GtkWidget* widget, GdkDragContext* context,
                                 GtkSelectionData* selection_data,
                                 guint target_type, guint time,
                                 CustomDrag* custom_drag) {
    return custom_drag->OnDragDataGet(widget, context, selection_data,
                                      target_type, time);
  }

  
  
  GtkWidget* drag_widget_;

  
  
  gfx::Image* image_;

  DISALLOW_COPY_AND_ASSIGN(CustomDrag);
};

class DownloadItemDrag : public CustomDrag {
 public:
  
  
  
  static void SetSource(GtkWidget* widget,
                        DownloadItem* item,
                        gfx::Image* icon);

  
  
  static void BeginDrag(const DownloadItem* item, gfx::Image* icon);

 private:
  DownloadItemDrag(const DownloadItem* item, gfx::Image* icon);
  virtual ~DownloadItemDrag();

  virtual void OnDragDataGet(GtkWidget* widget, GdkDragContext* context,
                             GtkSelectionData* selection_data,
                             guint target_type, guint time);

  const DownloadItem* download_item_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemDrag);
};

class BookmarkDrag : public CustomDrag {
 public:
  
  
  static void BeginDrag(Profile* profile,
                        const std::vector<const BookmarkNode*>& nodes);

 private:
  BookmarkDrag(Profile* profile,
               const std::vector<const BookmarkNode*>& nodes);
  virtual ~BookmarkDrag();

  virtual void OnDragDataGet(GtkWidget* widget, GdkDragContext* context,
                             GtkSelectionData* selection_data,
                             guint target_type, guint time);

  Profile* profile_;
  std::vector<const BookmarkNode*> nodes_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkDrag);
};

#endif  