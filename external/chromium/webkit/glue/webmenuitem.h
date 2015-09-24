// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBMENUITEM_H_
#define WEBMENUITEM_H_

#include <vector>

#include "base/string16.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebMenuItemInfo.h"

struct WebMenuItem {
  enum Type {
    OPTION    = WebKit::WebMenuItemInfo::Option,
    CHECKABLE_OPTION = WebKit::WebMenuItemInfo::CheckableOption,
    GROUP     = WebKit::WebMenuItemInfo::Group,
    SEPARATOR = WebKit::WebMenuItemInfo::Separator,
    SUBMENU  
  };

  WebMenuItem();
  WebMenuItem(const WebKit::WebMenuItemInfo& item);
  WebMenuItem(const WebMenuItem& item);
  ~WebMenuItem();

  string16 label;
  Type type;
  unsigned action;
  bool rtl;
  bool has_directional_override;
  bool enabled;
  bool checked;
  std::vector<WebMenuItem> submenu;
};

#endif  
