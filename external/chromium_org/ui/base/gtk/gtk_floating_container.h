// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_FLOATING_CONTAINER_H_
#define UI_BASE_GTK_GTK_FLOATING_CONTAINER_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "ui/base/ui_export.h"


G_BEGIN_DECLS

#define GTK_TYPE_FLOATING_CONTAINER                                 \
    (gtk_floating_container_get_type())
#define GTK_FLOATING_CONTAINER(obj)                                 \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_FLOATING_CONTAINER, \
                                GtkFloatingContainer))
#define GTK_FLOATING_CONTAINER_CLASS(klass)                         \
    (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_FLOATING_CONTAINER,  \
                             GtkFloatingContainerClass))
#define GTK_IS_FLOATING_CONTAINER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_FLOATING_CONTAINER))
#define GTK_IS_FLOATING_CONTAINER_CLASS(klass)                      \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_FLOATING_CONTAINER))
#define GTK_FLOATING_CONTAINER_GET_CLASS(obj)                       \
    (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_FLOATING_CONTAINER,  \
                               GtkFloatingContainerClass))

typedef struct _GtkFloatingContainer GtkFloatingContainer;
typedef struct _GtkFloatingContainerClass GtkFloatingContainerClass;
typedef struct _GtkFloatingContainerChild GtkFloatingContainerChild;

struct _GtkFloatingContainer {
  
  GtkBin bin;

  
  
  GList* floating_children;
};

struct _GtkFloatingContainerClass {
  GtkBinClass parent_class;
};

struct _GtkFloatingContainerChild {
  GtkWidget* widget;
  gint x;
  gint y;
};

UI_EXPORT GType      gtk_floating_container_get_type() G_GNUC_CONST;
UI_EXPORT GtkWidget* gtk_floating_container_new();
UI_EXPORT void       gtk_floating_container_add_floating(
    GtkFloatingContainer* container,
    GtkWidget* widget);

G_END_DECLS

#endif  
