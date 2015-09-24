// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCESSIBILITY_AX_NODE_DATA_H_
#define UI_ACCESSIBILITY_AX_NODE_DATA_H_

#include <map>
#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/accessibility/ax_export.h"
#include "ui/gfx/rect.h"

namespace ui {

struct AX_EXPORT AXNodeData {
  
  
  enum StringAttribute {
    
    ATTR_DOC_URL,
    ATTR_DOC_TITLE,
    ATTR_DOC_MIMETYPE,
    ATTR_DOC_DOCTYPE,

    
    ATTR_ACCESS_KEY,
    ATTR_ACTION,
    ATTR_CONTAINER_LIVE_RELEVANT,
    ATTR_CONTAINER_LIVE_STATUS,
    ATTR_DESCRIPTION,
    ATTR_DISPLAY,
    ATTR_HELP,
    ATTR_HTML_TAG,
    ATTR_NAME,
    ATTR_LIVE_RELEVANT,
    ATTR_LIVE_STATUS,
    ATTR_ROLE,
    ATTR_SHORTCUT,
    ATTR_URL,
    ATTR_VALUE,
  };

  enum IntAttribute {
    
    ATTR_SCROLL_X,
    ATTR_SCROLL_X_MIN,
    ATTR_SCROLL_X_MAX,
    ATTR_SCROLL_Y,
    ATTR_SCROLL_Y_MIN,
    ATTR_SCROLL_Y_MAX,

    
    ATTR_TEXT_SEL_START,
    ATTR_TEXT_SEL_END,

    
    ATTR_TABLE_ROW_COUNT,
    ATTR_TABLE_COLUMN_COUNT,
    ATTR_TABLE_HEADER_ID,

    
    ATTR_TABLE_ROW_INDEX,
    ATTR_TABLE_ROW_HEADER_ID,

    
    ATTR_TABLE_COLUMN_INDEX,
    ATTR_TABLE_COLUMN_HEADER_ID,

    
    ATTR_TABLE_CELL_COLUMN_INDEX,
    ATTR_TABLE_CELL_COLUMN_SPAN,
    ATTR_TABLE_CELL_ROW_INDEX,
    ATTR_TABLE_CELL_ROW_SPAN,

    
    ATTR_HIERARCHICAL_LEVEL,

    
    ATTR_TITLE_UI_ELEMENT,

    
    ATTR_COLOR_VALUE_RED,
    ATTR_COLOR_VALUE_GREEN,
    ATTR_COLOR_VALUE_BLUE,

    
    ATTR_TEXT_DIRECTION
  };

  enum FloatAttribute {
    
    ATTR_DOC_LOADING_PROGRESS,

    
    ATTR_VALUE_FOR_RANGE,
    ATTR_MIN_VALUE_FOR_RANGE,
    ATTR_MAX_VALUE_FOR_RANGE,
  };

  enum BoolAttribute {
    
    ATTR_DOC_LOADED,

    
    ATTR_BUTTON_MIXED,

    
    ATTR_CONTAINER_LIVE_ATOMIC,
    ATTR_CONTAINER_LIVE_BUSY,
    ATTR_LIVE_ATOMIC,
    ATTR_LIVE_BUSY,

    
    ATTR_ARIA_READONLY,

    
    ATTR_CAN_SET_VALUE,

    
    
    ATTR_UPDATE_LOCATION_ONLY,

    
    ATTR_CANVAS_HAS_FALLBACK,
  };

  enum IntListAttribute {
    
    
    
    
    ATTR_INDIRECT_CHILD_IDS,

    
    ATTR_LINE_BREAKS,

    
    
    
    ATTR_CELL_IDS,

    
    
    ATTR_UNIQUE_CELL_IDS,

    
    
    
    
    
    
    ATTR_CHARACTER_OFFSETS,

    
    
    ATTR_WORD_STARTS,
    ATTR_WORD_ENDS,
  };

  AXNodeData();
  virtual ~AXNodeData();

  void AddStringAttribute(StringAttribute attribute,
                          const std::string& value);
  void AddIntAttribute(IntAttribute attribute, int value);
  void AddFloatAttribute(FloatAttribute attribute, float value);
  void AddBoolAttribute(BoolAttribute attribute, bool value);
  void AddIntListAttribute(IntListAttribute attribute,
                           const std::vector<int32>& value);

  
  
  void SetName(std::string name);
  
  void SetValue(std::string value);

  
  
  int32 id;
  AXRole role;
  uint32 state;
  gfx::Rect location;
  std::vector<std::pair<StringAttribute, std::string> > string_attributes;
  std::vector<std::pair<IntAttribute, int32> > int_attributes;
  std::vector<std::pair<FloatAttribute, float> > float_attributes;
  std::vector<std::pair<BoolAttribute, bool> > bool_attributes;
  std::vector<std::pair<IntListAttribute, std::vector<int32> > >
      intlist_attributes;
  std::vector<std::pair<std::string, std::string> > html_attributes;
  std::vector<int32> child_ids;
};

}  

#endif  
