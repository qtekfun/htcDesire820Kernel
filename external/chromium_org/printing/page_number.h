// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PAGE_NUMBER_H_
#define PRINTING_PAGE_NUMBER_H_

#include <ostream>

#include "printing/page_range.h"

namespace printing {

class PrintSettings;

class PRINTING_EXPORT PageNumber {
 public:
  
  PageNumber(const PrintSettings& settings, int document_page_count);

  PageNumber();

  void operator=(const PageNumber& other);

  
  
  void Init(const PrintSettings& settings, int document_page_count);

  
  int ToInt() const {
    return page_number_;
  }

  
  int operator++();

  
  static const PageNumber npos() {
    return PageNumber();
  }

  
  
  bool operator==(const PageNumber& other) const;
  bool operator!=(const PageNumber& other) const;

 private:
  
  const PageRanges* ranges_;

  
  int page_number_;

  
  
  int page_range_index_;

  
  int document_page_count_;
};

template<class E, class T>
inline typename std::basic_ostream<E,T>& operator<<(
    typename std::basic_ostream<E,T>& ss, const PageNumber& page) {
  return ss << page.ToInt();
}

}  

#endif  
