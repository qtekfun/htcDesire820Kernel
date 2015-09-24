// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_GTK_CHROME_LINK_BUTTON_H_
#define CHROME_BROWSER_UI_GTK_GTK_CHROME_LINK_BUTTON_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_CHROME_LINK_BUTTON        (gtk_chrome_link_button_get_type ())
#define GTK_CHROME_LINK_BUTTON(obj)        (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                            GTK_TYPE_CHROME_LINK_BUTTON, \
                                            GtkChromeLinkButton))
#define GTK_CHROME_LINK_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), \
                                             GTK_TYPE_CHROME_LINK_BUTTON, \
                                             GtkChromeLinkButtonClass))
#define GTK_IS_CHROME_LINK_BUTTON(obj)                           \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_CHROME_LINK_BUTTON))
#define GTK_IS_CHROME_LINK_BUTTON_CLASS(klass)                   \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_CHROME_LINK_BUTTON))
#define GTK_CHROME_LINK_BUTTON_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_CHROME_LINK_BUTTON, \
                             GtkChromeLinkButton))

typedef struct _GtkChromeLinkButton        GtkChromeLinkButton;
typedef struct _GtkChromeLinkButtonClass   GtkChromeLinkButtonClass;

struct _GtkChromeLinkButton {
  GtkButton button;
  GtkWidget* label;
  gchar* normal_markup;
  gchar* pressed_markup;
  gchar* insensitive_markup;
  GtkStateType label_state;
  gchar normal_color[9];
  gchar* native_markup;
  gchar* insensitive_native_markup;
  gboolean using_native_theme;
  GdkCursor* hand_cursor;
  gchar* text;
  gboolean uses_markup;
};

struct _GtkChromeLinkButtonClass {
  GtkButtonClass parent_class;
};

GtkWidget* gtk_chrome_link_button_new(const char* text);

GtkWidget* gtk_chrome_link_button_new_with_markup(const char* markup);

void gtk_chrome_link_button_set_use_gtk_theme(GtkChromeLinkButton* button,
                                              gboolean use_gtk);

void gtk_chrome_link_button_set_label(GtkChromeLinkButton* button,
                                      const char* text);

void gtk_chrome_link_button_set_normal_color(GtkChromeLinkButton* button,
                                             const GdkColor* color);

GType gtk_chrome_link_button_get_type();

G_END_DECLS

#endif  
