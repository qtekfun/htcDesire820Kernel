// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TEXT_ELIDER_H_
#define UI_GFX_TEXT_ELIDER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "third_party/icu/source/common/unicode/uchar.h"
#include "third_party/icu/source/i18n/unicode/coll.h"
#include "ui/gfx/gfx_export.h"

class GURL;

namespace base {
class FilePath;
}

namespace gfx {
class Font;
class FontList;

GFX_EXPORT extern const char kEllipsis[];
GFX_EXPORT extern const base::char16 kEllipsisUTF16[];

GFX_EXPORT base::string16 ElideEmail(const base::string16& email,
                                     const gfx::FontList& font_list,
                                     float available_pixel_width);

GFX_EXPORT base::string16 ElideUrl(const GURL& url,
                                   const gfx::FontList& font_list,
                                   float available_pixel_width,
                                   const std::string& languages);

enum ElideBehavior {
  
  ELIDE_AT_END,
  
  ELIDE_IN_MIDDLE,
  
  TRUNCATE_AT_END
};

GFX_EXPORT base::string16 ElideText(const base::string16& text,
                                    const gfx::FontList& font_list,
                                    float available_pixel_width,
                                    ElideBehavior elide_behavior);
GFX_EXPORT base::string16 ElideText(const base::string16& text,
                                    const gfx::Font& font,
                                    float available_pixel_width,
                                    ElideBehavior elide_behavior);

GFX_EXPORT base::string16 ElideFilename(const base::FilePath& filename,
                                        const gfx::FontList& font_list,
                                        float available_pixel_width);

class GFX_EXPORT SortedDisplayURL {
 public:
  SortedDisplayURL(const GURL& url, const std::string& languages);
  SortedDisplayURL();
  ~SortedDisplayURL();

  
  
  
  int Compare(const SortedDisplayURL& other, icu::Collator* collator) const;

  
  const base::string16& display_url() const { return display_url_; }

 private:
  
  
  base::string16 AfterHost() const;

  
  base::string16 sort_host_;

  
  size_t prefix_end_;

  base::string16 display_url_;

  DISALLOW_COPY_AND_ASSIGN(SortedDisplayURL);
};


GFX_EXPORT bool ElideString(const base::string16& input, int max_len,
                            base::string16* output);

GFX_EXPORT bool ElideRectangleString(const base::string16& input,
                                     size_t max_rows,
                                     size_t max_cols,
                                     bool strict,
                                     base::string16* output);

enum WordWrapBehavior {
  
  
  IGNORE_LONG_WORDS,

  
  
  TRUNCATE_LONG_WORDS,

  
  
  ELIDE_LONG_WORDS,

  
  
  WRAP_LONG_WORDS,
};

enum ReformattingResultFlags {
  INSUFFICIENT_SPACE_HORIZONTAL = 1 << 0,
  INSUFFICIENT_SPACE_VERTICAL = 1 << 1,
};

GFX_EXPORT int ElideRectangleText(const base::string16& text,
                                  const gfx::FontList& font_list,
                                  float available_pixel_width,
                                  int available_pixel_height,
                                  WordWrapBehavior wrap_behavior,
                                  std::vector<base::string16>* lines);

GFX_EXPORT base::string16 TruncateString(const base::string16& string,
                                         size_t length);

}  

#endif  
