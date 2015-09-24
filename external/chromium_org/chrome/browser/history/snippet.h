// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_SNIPPET_H__
#define CHROME_BROWSER_HISTORY_SNIPPET_H__

#include <vector>

#include "base/strings/string16.h"

class Snippet {
 public:
  
  
  typedef std::pair<size_t, size_t> MatchPosition;
  typedef std::vector<MatchPosition> MatchPositions;

  
  
  
  
  
  
  
  
  
  
  
  
  
  static void ExtractMatchPositions(const std::string& offsets_str,
                                    const std::string& column_num,
                                    MatchPositions* match_positions);

  
  
  static void ConvertMatchPositionsToWide(
      const std::string& utf8_string,
      Snippet::MatchPositions* match_positions);

  Snippet();
  ~Snippet();

  
  
  
  
  void ComputeSnippet(const MatchPositions& matches,
                      const std::string& document);

  const base::string16& text() const { return text_; }
  const MatchPositions& matches() const { return matches_; }

  
  void Swap(Snippet* other);

 private:
  
  base::string16 text_;

  
  MatchPositions matches_;
};

#endif  
