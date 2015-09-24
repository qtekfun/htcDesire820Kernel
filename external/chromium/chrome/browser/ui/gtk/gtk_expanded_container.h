// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_EXPANDED_CONTAINER_H_
#define CHROME_BROWSER_UI_GTK_GTK_EXPANDED_CONTAINER_H_
#pragma once

#include <gdk/gdk.h>
#include <gtk/gtk.h>


G_BEGIN_DECLS

#define GTK_TYPE_EXPANDED_CONTAINER                                 \
    (gtk_expanded_container_get_type())
#define GTK_EXPANDED_CONTAINER(obj)                                 \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_EXPANDED_CONTAINER, \
                                GtkExpandedContainer))
#define GTK_EXPANDED_CONTAINER_CLASS(klass)                         \
    (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_EXPANDED_CONTAINER,  \
                             GtkExpandedContainerClass))
#define GTK_IS_EXPANDED_CONTAINER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_EXPANDED_CONTAINER))
#define GTK_IS_EXPANDED_CONTAINER_CLASS(klass)                      \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_EXPANDED_CONTAINER))
#define GTK_EXPANDED_CONTAINER_GET_CLASS(obj)                       \
    (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_EXPANDED_CONTAINER,  \
                               GtkExpandedContainerClass))

typedef struct _GtkExpandedContainer GtkExpandedContainer;
typedef struct _GtkExpandedContainerClass GtkExpandedContainerClass;

struct _GtkExpandedContainer {
  
  GtkFixed fixed;
};

struct _GtkExpandedContainerClass {
  GtkFixedClass parent_class;
};

GType gtk_expanded_container_get_type() G_GNUC_CONST;
GtkWidget* gtk_expanded_container_new();
void gtk_expanded_container_put(GtkExpandedContainer* container,
                                GtkWidget* widget, gint x, gint y);
void gtk_expanded_container_move(GtkExpandedContainer* container,
                                 GtkWidget* widget, gint x, gint y);
void gtk_expanded_container_set_has_window(GtkExpandedContainer* container,
                                           gboolean has_window);
gboolean gtk_expanded_container_get_has_window(GtkExpandedContainer* container);

G_END_DECLS

#endif  
