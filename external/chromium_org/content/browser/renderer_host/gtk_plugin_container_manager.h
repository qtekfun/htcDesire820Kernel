// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_GTK_PLUGIN_CONTAINER_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_GTK_PLUGIN_CONTAINER_MANAGER_H_

#include <gtk/gtk.h>
#include <map>

#include "ui/gfx/native_widget_types.h"

typedef struct _GtkWidget GtkWidget;

namespace content {
struct WebPluginGeometry;

class GtkPluginContainerManager {
 public:
  GtkPluginContainerManager();
  ~GtkPluginContainerManager();

  
  void set_host_widget(GtkWidget *widget) { host_widget_ = widget; }

  
  GtkWidget* CreatePluginContainer(gfx::PluginWindowHandle id);

  
  void DestroyPluginContainer(gfx::PluginWindowHandle id);

  
  
  void MovePluginContainer(const WebPluginGeometry& move);

 private:
  
  GtkWidget* MapIDToWidget(gfx::PluginWindowHandle id);

  
  gfx::PluginWindowHandle MapWidgetToID(GtkWidget* widget);

  
  
  static void RealizeCallback(GtkWidget *widget, void *user_data);

  
  GtkWidget* host_widget_;

  
  typedef std::map<gfx::PluginWindowHandle, GtkWidget*> PluginWindowToWidgetMap;
  PluginWindowToWidgetMap plugin_window_to_widget_map_;
};

}  

#endif  
