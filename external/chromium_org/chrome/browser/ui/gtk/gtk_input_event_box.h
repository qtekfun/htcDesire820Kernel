// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_INPUT_EVENT_BOX_H_
#define CHROME_BROWSER_UI_GTK_GTK_INPUT_EVENT_BOX_H_

#include <gtk/gtk.h>


G_BEGIN_DECLS

#define GTK_TYPE_INPUT_EVENT_BOX                                 \
    (gtk_input_event_box_get_type())
#define GTK_INPUT_EVENT_BOX(obj)                                 \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_INPUT_EVENT_BOX, \
                                GtkInputEventBox))
#define GTK_INPUT_EVENT_BOX_CLASS(klass)                         \
    (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_INPUT_EVENT_BOX,  \
                             GtkInputEventBoxClass))
#define GTK_IS_INPUT_EVENT_BOX(obj)                              \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_INPUT_EVENT_BOX))
#define GTK_IS_INPUT_EVENT_BOX_CLASS(klass)                      \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_INPUT_EVENT_BOX))
#define GTK_INPUT_EVENT_BOX_GET_CLASS(obj)                       \
    (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_INPUT_EVENT_BOX,  \
                               GtkInputEventBoxClass))

typedef struct _GtkInputEventBox GtkInputEventBox;
typedef struct _GtkInputEventBoxClass GtkInputEventBoxClass;

struct _GtkInputEventBox {
  
  GtkBin bin;
};

struct _GtkInputEventBoxClass {
  GtkBinClass parent_class;
};

GType gtk_input_event_box_get_type();
GtkWidget* gtk_input_event_box_new();

GdkWindow* gtk_input_event_box_get_window(GtkInputEventBox* widget);

G_END_DECLS

#endif  
