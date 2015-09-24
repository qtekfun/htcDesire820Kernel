// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_CUSTOM_MENU_ITEM_H_
#define CHROME_BROWSER_UI_GTK_GTK_CUSTOM_MENU_ITEM_H_
#pragma once


#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_CUSTOM_MENU_ITEM                                       \
  (gtk_custom_menu_item_get_type())
#define GTK_CUSTOM_MENU_ITEM(obj)                                       \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_CUSTOM_MENU_ITEM,         \
                              GtkCustomMenuItem))
#define GTK_CUSTOM_MENU_ITEM_CLASS(klass)                               \
  (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_CUSTOM_MENU_ITEM,          \
                           GtkCustomMenuItemClass))
#define GTK_IS_CUSTOM_MENU_ITEM(obj)                                    \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_CUSTOM_MENU_ITEM))
#define GTK_IS_CUSTOM_MENU_ITEM_CLASS(klass)                            \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_CUSTOM_MENU_ITEM))
#define GTK_CUSTOM_MENU_ITEM_GET_CLASS(obj)                             \
  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_CUSTOM_MENU_ITEM,          \
                             GtkCustomMenuItemClass))

typedef struct _GtkCustomMenuItem GtkCustomMenuItem;
typedef struct _GtkCustomMenuItemClass GtkCustomMenuItemClass;

struct _GtkCustomMenuItem {
  GtkMenuItem menu_item;

  
  GtkWidget* hbox;

  
  GtkWidget* label;

  
  
  GList* all_widgets;

  
  GList* button_widgets;

  
  GtkWidget* currently_selected_button;

  
  
  
  
  GtkWidget* previously_selected_button;
};

struct _GtkCustomMenuItemClass {
  GtkMenuItemClass parent_class;
};

GType gtk_custom_menu_item_get_type(void) G_GNUC_CONST;
GtkWidget* gtk_custom_menu_item_new(const char* title);

GtkWidget* gtk_custom_menu_item_add_button(GtkCustomMenuItem* menu_item,
                                           int command_id);

GtkWidget* gtk_custom_menu_item_add_button_label(GtkCustomMenuItem* menu_item,
                                                 int command_id);

void gtk_custom_menu_item_add_space(GtkCustomMenuItem* menu_item);

void gtk_custom_menu_item_receive_motion_event(GtkCustomMenuItem* menu_item,
                                               gdouble x, gdouble y);

gboolean gtk_custom_menu_item_handle_move(GtkCustomMenuItem* menu_item,
                                          GtkMenuDirectionType direction);

void gtk_custom_menu_item_select_item_by_direction(
    GtkCustomMenuItem* menu_item, GtkMenuDirectionType direction);

gboolean gtk_custom_menu_item_is_in_clickable_region(
    GtkCustomMenuItem* menu_item);

gboolean gtk_custom_menu_item_try_no_dismiss_command(
    GtkCustomMenuItem* menu_item);

void gtk_custom_menu_item_foreach_button(GtkCustomMenuItem* menu_item,
                                         GtkCallback callback,
                                         gpointer callback_data);

G_END_DECLS

#endif  
