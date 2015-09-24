// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_CHROME_SHRINKABLE_HBOX_H_
#define CHROME_BROWSER_UI_GTK_GTK_CHROME_SHRINKABLE_HBOX_H_
#pragma once

#include <gdk/gdk.h>
#include <gtk/gtk.h>


G_BEGIN_DECLS

#define GTK_TYPE_CHROME_SHRINKABLE_HBOX                                 \
    (gtk_chrome_shrinkable_hbox_get_type())
#define GTK_CHROME_SHRINKABLE_HBOX(obj)                                 \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_CHROME_SHRINKABLE_HBOX, \
                                GtkChromeShrinkableHBox))
#define GTK_CHROME_SHRINKABLE_HBOX_CLASS(klass)                         \
    (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_CHROME_SHRINKABLE_HBOX,  \
                             GtkChromeShrinkableHBoxClass))
#define GTK_IS_CHROME_SHRINKABLE_HBOX(obj)                              \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_CHROME_SHRINKABLE_HBOX))
#define GTK_IS_CHROME_SHRINKABLE_HBOX_CLASS(klass)                      \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_CHROME_SHRINKABLE_HBOX))
#define GTK_CHROME_SHRINKABLE_HBOX_GET_CLASS(obj)                       \
    (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_CHROME_SHRINKABLE_HBOX,  \
                               GtkChromeShrinkableHBoxClass))

typedef struct _GtkChromeShrinkableHBox GtkChromeShrinkableHBox;
typedef struct _GtkChromeShrinkableHBoxClass GtkChromeShrinkableHBoxClass;

struct _GtkChromeShrinkableHBox {
  
  GtkHBox hbox;

  gboolean hide_child_directly;

  
  int children_width_requisition;
};

struct _GtkChromeShrinkableHBoxClass {
  GtkHBoxClass parent_class;
};

GType gtk_chrome_shrinkable_hbox_get_type() G_GNUC_CONST;

GtkWidget* gtk_chrome_shrinkable_hbox_new(gboolean hide_child_directly,
                                          gboolean homogeneous,
                                          gint spacing);

void gtk_chrome_shrinkable_hbox_set_hide_child_directly(
    GtkChromeShrinkableHBox* box, gboolean hide_child_directly);

gboolean gtk_chrome_shrinkable_hbox_get_hide_child_directly(
    GtkChromeShrinkableHBox* box);

void gtk_chrome_shrinkable_hbox_pack_start(GtkChromeShrinkableHBox* box,
                                           GtkWidget* child,
                                           guint padding);

void gtk_chrome_shrinkable_hbox_pack_end(GtkChromeShrinkableHBox* box,
                                         GtkWidget* child,
                                         guint padding);

gint gtk_chrome_shrinkable_hbox_get_visible_child_count(
    GtkChromeShrinkableHBox* box);

G_END_DECLS

#endif  
