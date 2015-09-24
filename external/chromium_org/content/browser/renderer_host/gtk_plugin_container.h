// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_GTK_PLUGIN_CONTAINER_H_
#define CONTENT_BROWSER_RENDERER_HOST_GTK_PLUGIN_CONTAINER_H_


typedef struct _GtkWidget GtkWidget;

namespace content {

GtkWidget* gtk_plugin_container_new();

void gtk_plugin_container_set_size(GtkWidget *widget, int width, int height);

}  

#endif  
