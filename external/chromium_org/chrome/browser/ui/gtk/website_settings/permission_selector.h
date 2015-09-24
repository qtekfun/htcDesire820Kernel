// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_WEBSITE_SETTINGS_PERMISSION_SELECTOR_H_
#define CHROME_BROWSER_UI_GTK_WEBSITE_SETTINGS_PERMISSION_SELECTOR_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/ui/website_settings/permission_menu_model.h"
#include "chrome/browser/ui/gtk/website_settings/permission_selector_observer.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "ui/base/gtk/gtk_signal.h"

class MenuGtk;
class GtkThemeService;

class PermissionSelector : public PermissionMenuModel::Delegate {
 public:
  
  
  
  
  
  PermissionSelector(GtkThemeService* theme_service,
                     const GURL& url,
                     ContentSettingsType type,
                     ContentSetting setting,
                     ContentSetting default_setting,
                     content_settings::SettingSource source);
  virtual ~PermissionSelector();

  
  
  GtkWidget* widget() { return widget_; }

  
  void AddObserver(PermissionSelectorObserver* observer);

  
  ContentSetting GetSetting() const;

  
  ContentSettingsType GetType() const;

 private:
  
  CHROMEGTK_CALLBACK_1(PermissionSelector, gboolean, OnMenuButtonPressEvent,
                       GdkEventButton*);

  
  virtual void ExecuteCommand(int command_id) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) OVERRIDE;

  
  GtkWidget* widget_;  

  
  GtkWidget* menu_button_;  

  
  scoped_ptr<MenuGtk> menu_;

  
  scoped_ptr<PermissionMenuModel> menu_model_;

  
  
  
  GtkWidget* icon_;

  
  
  ContentSettingsType type_;

  
  ContentSetting default_setting_;

  
  ContentSetting setting_;

  ObserverList<PermissionSelectorObserver, false> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(PermissionSelector);
};

#endif  
