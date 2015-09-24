// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_FONT_FALLBACK_WIN_H_
#define UI_GFX_FONT_FALLBACK_WIN_H_

#include <string>
#include <vector>

#include "ui/gfx/font.h"

namespace gfx {

namespace internal {

void GFX_EXPORT ParseFontLinkEntry(const std::string& entry,
                                  std::string* filename,
                                  std::string* font_name);

void GFX_EXPORT ParseFontFamilyString(const std::string& family,
                                     std::vector<std::string>* font_names);

}  

class GFX_EXPORT LinkedFontsIterator {
 public:
  
  
  explicit LinkedFontsIterator(Font font);
  virtual ~LinkedFontsIterator();

  
  
  void SetNextFont(Font font);

  
  
  
  
  bool NextFont(Font* font);

 protected:
  
  
  virtual const std::vector<Font>* GetLinkedFonts() const;

 private:
  
  Font original_font_;

  
  Font next_font_;

  
  bool next_font_set_;

  
  Font current_font_;

  
  const std::vector<Font>* linked_fonts_;

  
  size_t linked_font_index_;

  DISALLOW_COPY_AND_ASSIGN(LinkedFontsIterator);
};

}  

#endif  
