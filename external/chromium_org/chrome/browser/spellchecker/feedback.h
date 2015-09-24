// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_FEEDBACK_H_
#define CHROME_BROWSER_SPELLCHECKER_FEEDBACK_H_

#include <map>
#include <set>
#include <vector>

#include "chrome/browser/spellchecker/misspelling.h"

namespace spellcheck {

class Feedback {
 public:
  Feedback();
  ~Feedback();

  
  
  
  Misspelling* GetMisspelling(uint32 hash);

  
  
  void FinalizeRemovedMisspellings(
      int renderer_process_id,
      const std::vector<uint32>& remaining_markers);

  
  
  bool RendererHasMisspellings(int renderer_process_id) const;

  
  
  std::vector<Misspelling> GetMisspellingsInRenderer(
      int renderer_process_id) const;

  
  
  void EraseFinalizedMisspellings(int renderer_process_id);

  
  bool HasMisspelling(uint32 hash) const;

  
  
  void AddMisspelling(int renderer_process_id, const Misspelling& misspelling);

  
  bool Empty() const;

  
  std::vector<int> GetRendersWithMisspellings() const;

  
  void FinalizeAllMisspellings();

  
  std::vector<Misspelling> GetAllMisspellings() const;

  
  void Clear();

  
  const std::set<uint32>& FindMisspellings(
      const base::string16& misspelled_text) const;

 private:
  typedef std::map<uint32, Misspelling> HashMisspellingMap;
  typedef std::set<uint32> HashCollection;
  typedef std::map<int, HashCollection> RendererHashesMap;
  typedef std::map<base::string16, HashCollection> TextHashesMap;

  
  
  const HashCollection empty_hash_collection_;

  
  
  HashMisspellingMap misspellings_;

  
  RendererHashesMap renderers_;

  
  TextHashesMap text_;

  DISALLOW_COPY_AND_ASSIGN(Feedback);
};

}  

#endif  
