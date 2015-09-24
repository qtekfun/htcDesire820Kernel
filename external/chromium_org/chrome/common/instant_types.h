// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_INSTANT_TYPES_H_
#define CHROME_COMMON_INSTANT_TYPES_H_

#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/common/autocomplete_match_type.h"
#include "content/public/common/page_transition_types.h"
#include "url/gurl.h"

typedef int InstantRestrictedID;

struct InstantSuggestion {
  InstantSuggestion();
  InstantSuggestion(const base::string16& in_text,
                    const std::string& in_metadata);
  ~InstantSuggestion();

  
  base::string16 text;

  
  std::string metadata;
};

struct InstantAutocompleteResult {
  InstantAutocompleteResult();
  ~InstantAutocompleteResult();

  
  base::string16 provider;

  
  AutocompleteMatchType::Type type;

  
  base::string16 description;

  
  base::string16 destination_url;

  
  
  base::string16 search_query;

  
  
  content::PageTransition transition;

  
  int relevance;

  
  
  
  
  size_t autocomplete_match_index;
};

typedef std::pair<InstantRestrictedID, InstantAutocompleteResult>
    InstantAutocompleteResultIDPair;

enum ThemeBackgroundImageAlignment {
  THEME_BKGRND_IMAGE_ALIGN_CENTER,
  THEME_BKGRND_IMAGE_ALIGN_LEFT,
  THEME_BKGRND_IMAGE_ALIGN_TOP,
  THEME_BKGRND_IMAGE_ALIGN_RIGHT,
  THEME_BKGRND_IMAGE_ALIGN_BOTTOM,
};

enum ThemeBackgroundImageTiling {
  THEME_BKGRND_IMAGE_NO_REPEAT,
  THEME_BKGRND_IMAGE_REPEAT_X,
  THEME_BKGRND_IMAGE_REPEAT_Y,
  THEME_BKGRND_IMAGE_REPEAT,
};

struct RGBAColor {
  RGBAColor();
  ~RGBAColor();

  bool operator==(const RGBAColor& rhs) const;

  
  
  uint8 r;
  uint8 g;
  uint8 b;
  uint8 a;
};

struct ThemeBackgroundInfo {
  ThemeBackgroundInfo();
  ~ThemeBackgroundInfo();

  bool operator==(const ThemeBackgroundInfo& rhs) const;

  
  bool using_default_theme;

  
  RGBAColor background_color;

  
  RGBAColor text_color;

  
  RGBAColor link_color;

  
  RGBAColor text_color_light;

  
  RGBAColor header_color;

  
  RGBAColor section_border_color;

  
  
  std::string theme_id;

  
  
  ThemeBackgroundImageAlignment image_horizontal_alignment;

  
  
  ThemeBackgroundImageAlignment image_vertical_alignment;

  
  ThemeBackgroundImageTiling image_tiling;

  
  
  uint16 image_height;

  
  
  bool has_attribution;

  
  bool logo_alternate;
};

struct InstantMostVisitedItem {
  
  GURL url;

  
  
  base::string16 title;
};

typedef std::pair<InstantRestrictedID, InstantMostVisitedItem>
    InstantMostVisitedItemIDPair;

#endif  
