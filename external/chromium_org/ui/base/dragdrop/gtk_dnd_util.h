// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_GTK_DND_UTIL_H_
#define UI_BASE_DRAGDROP_GTK_DND_UTIL_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

class GURL;

namespace ui {

enum {
  
  CHROME_TAB = 1 << 0,
  CHROME_BOOKMARK_ITEM = 1 << 1,
  CHROME_WEBDROP_FILE_CONTENTS = 1 << 2,
  CHROME_NAMED_URL = 1 << 3,

  
  TEXT_PLAIN = 1 << 4,
  TEXT_URI_LIST = 1 << 5,
  TEXT_HTML = 1 << 6,

  
  
  NETSCAPE_URL = 1 << 7,

  
  TEXT_PLAIN_NO_CHARSET = 1 << 8,
  DIRECT_SAVE_FILE = 1 << 9,

  
  CUSTOM_DATA = 1 << 10,

  INVALID_TARGET = 1 << 11,
};

UI_EXPORT GdkAtom GetAtomForTarget(int target);

UI_EXPORT GtkTargetList* GetTargetListFromCodeMask(int code_mask);

UI_EXPORT void SetSourceTargetListFromCodeMask(GtkWidget* source,
                                               int code_mask);

UI_EXPORT void SetDestTargetList(GtkWidget* dest, const int* target_codes);

UI_EXPORT void WriteURLWithName(GtkSelectionData* selection_data,
                                const GURL& url,
                                base::string16 title,
                                int type);

UI_EXPORT bool ExtractNamedURL(GtkSelectionData* selection_data,
                               GURL* url,
                               base::string16* title);

UI_EXPORT bool ExtractURIList(GtkSelectionData* selection_data,
                              std::vector<GURL>* urls);

UI_EXPORT bool ExtractNetscapeURL(GtkSelectionData* selection_data,
                                  GURL* url,
                                  base::string16* title);

}  

#endif  
