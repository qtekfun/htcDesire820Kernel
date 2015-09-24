// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_CONTEXT_MENU_PARAMS_H_
#define CONTENT_PUBLIC_COMMON_CONTEXT_MENU_PARAMS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/menu_item.h"
#include "content/public/common/page_state.h"
#include "content/public/common/ssl_status.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "third_party/WebKit/public/web/WebContextMenuData.h"
#include "ui/base/ui_base_types.h"
#include "url/gurl.h"

#if defined(OS_ANDROID)
#include "ui/gfx/point.h"
#endif

namespace content {

struct CONTENT_EXPORT CustomContextMenuContext {
  static const int32 kCurrentRenderWidget;

  CustomContextMenuContext();

  bool is_pepper_menu;
  int request_id;
  
  
  
  int32 render_widget_id;
};

struct CONTENT_EXPORT ContextMenuParams {
  ContextMenuParams();
  ~ContextMenuParams();

  
  blink::WebContextMenuData::MediaType media_type;

  
  
  int x;
  int y;

  
  
  GURL link_url;

  
  
  
  base::string16 link_text;

  
  
  GURL unfiltered_link_url;

  
  
  
  GURL src_url;

  
  
  bool has_image_contents;

  
  
  GURL page_url;

  
  
  GURL keyword_url;

  
  GURL frame_url;

  
  int64 frame_id;

  
  PageState frame_page_state;

  
  
  int media_flags;

  
  base::string16 selection_text;

  
  
  base::string16 misspelled_word;

  
  uint32 misspelling_hash;

  
  
  
  
  
  std::vector<base::string16> dictionary_suggestions;

  
  bool speech_input_enabled;

  
  bool spellcheck_enabled;

  
  bool is_editable;

  
  int writing_direction_default;
  int writing_direction_left_to_right;
  int writing_direction_right_to_left;

  
  
  int edit_flags;

  
  SSLStatus security_info;

  
  std::string frame_charset;

  
  blink::WebReferrerPolicy referrer_policy;

  CustomContextMenuContext custom_context;
  std::vector<MenuItem> custom_items;

  ui::MenuSourceType source_type;

#if defined(OS_ANDROID)
  
  
  gfx::Point selection_start;
  gfx::Point selection_end;
#endif
};

}  

#endif  
