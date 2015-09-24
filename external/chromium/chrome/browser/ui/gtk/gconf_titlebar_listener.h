// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GCONF_TITLEBAR_LISTENER_H_
#define CHROME_BROWSER_UI_GTK_GCONF_TITLEBAR_LISTENER_H_
#pragma once

#include <gconf/gconf-client.h>
#include <gtk/gtk.h>

#include <set>
#include <string>

#include "base/basictypes.h"
#include "ui/base/gtk/gtk_signal.h"

class BrowserTitlebar;
template <typename T> struct DefaultSingletonTraits;

class GConfTitlebarListener {
 public:
  
  static GConfTitlebarListener* GetInstance();

  
  
  void SetTitlebarButtons(BrowserTitlebar* titlebar);

  
  
  void RemoveObserver(BrowserTitlebar* titlebar);

 protected:
  virtual ~GConfTitlebarListener();

 private:
  
  GConfTitlebarListener();

  
  CHROMEG_CALLBACK_2(GConfTitlebarListener, void, OnChangeNotification,
                     GConfClient*, guint, GConfEntry*);

  
  
  bool HandleGError(GError* error, const char* key);

  
  
  void ParseAndStoreValue(GConfValue* gconf_value);

  
  
  GConfClient* client_;

  
  std::string current_value_;

  
  std::set<BrowserTitlebar*> titlebars_;

  friend struct DefaultSingletonTraits<GConfTitlebarListener>;
  DISALLOW_COPY_AND_ASSIGN(GConfTitlebarListener);
};

#endif  
