// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SHOWN_SECTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_SHOWN_SECTIONS_HANDLER_H_
#pragma once

#include "chrome/browser/prefs/pref_change_registrar.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"

class Extension;
class Value;
class PrefService;

enum Section {
  
  
  THUMB = 1 << 0,
  APPS = 1 << 6,

  
  ALL_SECTIONS_MASK = 0x0000FFFF,

  
  
  MENU_THUMB = 1 << (0 + 16),
  MENU_RECENT = 1 << (2 + 16),
  MENU_APPS = 1 << (6 + 16),
};

class ShownSectionsHandler : public WebUIMessageHandler,
                             public NotificationObserver {
 public:
  explicit ShownSectionsHandler(PrefService* pref_service);
  virtual ~ShownSectionsHandler() {}

  
  static int GetShownSections(PrefService* pref_service);

  
  
  static void SetShownSection(PrefService* prefs, Section section);

  
  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void HandleGetShownSections(const ListValue* args);

  
  void HandleSetShownSections(const ListValue* args);

  static void RegisterUserPrefs(PrefService* pref_service);

  static void MigrateUserPrefs(PrefService* pref_service,
                               int old_pref_version,
                               int new_pref_version);

  static void OnExtensionInstalled(PrefService* prefs,
                                   const Extension* extension);

 private:
  PrefService* pref_service_;
  PrefChangeRegistrar pref_registrar_;

  DISALLOW_COPY_AND_ASSIGN(ShownSectionsHandler);
};

#endif  
