// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_CONTEXT_MENU_H_
#define WEBKIT_GLUE_CONTEXT_MENU_H_

#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/webmenuitem.h"

#include "third_party/WebKit/Source/WebKit/chromium/public/WebContextMenuData.h"

namespace webkit_glue {

struct CustomContextMenuContext {
  bool is_pepper_menu;
  int request_id;
  
  
  
  int32 render_widget_id;
  static const int32 kCurrentRenderWidget;

  CustomContextMenuContext();
};

}  

struct ContextMenuParams {
  
  WebKit::WebContextMenuData::MediaType media_type;

  
  
  int x;
  int y;

  
  
  GURL link_url;

  
  
  GURL unfiltered_link_url;

  
  
  
  GURL src_url;

  
  bool is_image_blocked;

  
  
  GURL page_url;

  
  GURL frame_url;

  
  
  std::string frame_content_state;

  
  
  int media_flags;

  
  string16 selection_text;

  
  
  string16 misspelled_word;

  
  
  
  
  
  std::vector<string16> dictionary_suggestions;

  
  bool spellcheck_enabled;

  
  bool is_editable;

#if defined(OS_MACOSX)
  
  
  int writing_direction_default;
  int writing_direction_left_to_right;
  int writing_direction_right_to_left;
#endif  

  
  
  int edit_flags;

  
  std::string security_info;

  
  std::string frame_charset;

  webkit_glue::CustomContextMenuContext custom_context;
  std::vector<WebMenuItem> custom_items;

  ContextMenuParams();
  ContextMenuParams(const WebKit::WebContextMenuData& data);
  ~ContextMenuParams();
};

#endif  
