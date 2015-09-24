// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GTK_PRESERVE_WINDOW_H_
#define UI_GFX_GTK_PRESERVE_WINDOW_H_

#include <atk/atk.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "ui/gfx/gfx_export.h"


G_BEGIN_DECLS

#define GTK_TYPE_PRESERVE_WINDOW                                 \
    (gtk_preserve_window_get_type())
#define GTK_PRESERVE_WINDOW(obj)                                 \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_PRESERVE_WINDOW, \
                                GtkPreserveWindow))
#define GTK_PRESERVE_WINDOW_CLASS(klass)                         \
    (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_PRESERVE_WINDOW,  \
                             GtkPreserveWindowClass))
#define GTK_IS_PRESERVE_WINDOW(obj)                              \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_PRESERVE_WINDOW))
#define GTK_IS_PRESERVE_WINDOW_CLASS(klass)                      \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_PRESERVE_WINDOW))
#define GTK_PRESERVE_WINDOW_GET_CLASS(obj)                       \
    (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_PRESERVE_WINDOW,  \
                               GtkPreserveWindowClass))

typedef struct _GtkPreserveWindow GtkPreserveWindow;
typedef struct _GtkPreserveWindowClass GtkPreserveWindowClass;

struct _GtkPreserveWindow {
  
  GtkFixed fixed;
};

struct _GtkPreserveWindowClass {
  GtkFixedClass parent_class;
};

GFX_EXPORT GType gtk_preserve_window_get_type() G_GNUC_CONST;
GFX_EXPORT GtkWidget* gtk_preserve_window_new();

GFX_EXPORT gboolean gtk_preserve_window_get_preserve(GtkPreserveWindow* widget);
GFX_EXPORT void gtk_preserve_window_set_preserve(GtkPreserveWindow* widget,
                                                 gboolean value);

GFX_EXPORT void gtk_preserve_window_delegate_resize(GtkPreserveWindow* widget,
                                                    gboolean delegate);

GFX_EXPORT void gtk_preserve_window_set_accessible_factory(
    GtkPreserveWindow* widget,
    AtkObject* (*factory)(void* userdata),
    gpointer userdata);

G_END_DECLS

#endif  
