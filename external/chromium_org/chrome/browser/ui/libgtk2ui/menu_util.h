// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_MENU_UTIL_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_MENU_UTIL_H_

#include <gtk/gtk.h>

#include "ui/gfx/image/image.h"

namespace ui {
class MenuModel;
}

namespace libgtk2ui {
GtkWidget* BuildMenuItemWithImage(const std::string& label, GtkWidget* image);
GtkWidget* BuildMenuItemWithImage(const std::string& label,
                                  const gfx::Image& icon);
GtkWidget* BuildMenuItemWithLabel(const std::string& label);

ui::MenuModel* ModelForMenuItem(GtkMenuItem* menu_item);

GtkWidget* AppendMenuItemToMenu(int index,
                                ui::MenuModel* model,
                                GtkWidget* menu_item,
                                GtkWidget* menu,
                                bool connect_to_activate,
                                GCallback item_activated_cb,
                                void* this_ptr);

bool GetMenuItemID(GtkWidget* menu_item, int* menu_id);

void ExecuteCommand(ui::MenuModel* model, int id);

void BuildSubmenuFromModel(ui::MenuModel* model,
                           GtkWidget* menu,
                           GCallback item_activated_cb,
                           bool* block_activation,
                           void* this_ptr);

void SetMenuItemInfo(GtkWidget* widget, void* block_activation_ptr);

}  

#endif  
