// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TOOLS_CONVERT_DICT_DIC_READER_H__
#define CHROME_TOOLS_CONVERT_DICT_DIC_READER_H__

#include <stdio.h>

#include <string>
#include <vector>

namespace base {
class FilePath;
}

namespace convert_dict {

class AffReader;

class DicReader {
 public:
  
  
  
  typedef std::pair<std::string, std::vector<int> > WordEntry;
  typedef std::vector<WordEntry> WordList;

  explicit DicReader(const base::FilePath& path);
  ~DicReader();

  
  
  bool Read(AffReader* aff_reader);

  
  const WordList& words() const { return words_; }

 private:
  FILE* file_;
  FILE* additional_words_file_;

  
  WordList words_;
};

}  

#endif  
