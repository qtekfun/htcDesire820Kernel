// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_GCONF_TITLEBAR_LISTENER_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_GCONF_TITLEBAR_LISTENER_H_

#include <gconf/gconf-client.h>
#include <gtk/gtk.h>

#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/ui/libgtk2ui/gtk2_signal.h"

namespace libgtk2ui {
class Gtk2UI;

class GConfTitlebarListener {
 public:
  
  explicit GConfTitlebarListener(Gtk2UI* delegate);
  ~GConfTitlebarListener();

 private:
  
  CHROMEG_CALLBACK_2(GConfTitlebarListener, void, OnChangeNotification,
                     GConfClient*, guint, GConfEntry*);

  
  
  bool HandleGError(GError* error, const char* key);

  
  
  void ParseAndStoreValue(GConfValue* gconf_value);

  Gtk2UI* delegate_;

  
  
  GConfClient* client_;

  DISALLOW_COPY_AND_ASSIGN(GConfTitlebarListener);
};

}  

#endif  
