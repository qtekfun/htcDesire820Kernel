// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_FRAME_HTML_UTILS_H_
#define CHROME_FRAME_HTML_UTILS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "net/http/http_util.h"

class HtmlUtilUnittest;

class HTMLScanner {
 public:
  typedef std::wstring::const_iterator StrPos;

  
  class StringRange {
    friend class HTMLScanner;
   public:
    StringRange();
    StringRange(StrPos start, StrPos end);

    bool LowerCaseEqualsASCII(const char* other) const;
    bool Equals(const wchar_t* other) const;

    
    std::wstring Copy() const;

    
    
    
    
    bool GetTagName(std::wstring* tag_name) const;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    bool GetTagAttribute(const wchar_t* attribute_name,
                         StringRange* attribute_value) const;

    
    
    
    bool UnQuote();
   private:
     StrPos start_;
     StrPos end_;
  };

  typedef std::vector<StringRange> StringRangeList;

  
  
  explicit HTMLScanner(const wchar_t* html_string);

  
  
  
  void GetTagsByName(const wchar_t* name, StringRangeList* tag_list,
                     const wchar_t* stop_tag_name);

 private:
  friend class HtmlUtilUnittest;
  FRIEND_TEST_ALL_PREFIXES(HtmlUtilUnittest, BasicTest);

  
  
  
  
  
  
  
  
  bool NextTag(StringRange* html_string, StringRange* tag);

  
  bool IsQuote(wchar_t c);

  
  
  
  
  bool IsHTMLCommentClose(const StringRange* html_string, StrPos pos);

  
  
  
  
  
  bool IsIEConditionalCommentClose(const StringRange* html_string, StrPos pos);

  
  const std::wstring html_string_;

  
  const std::wstring quotes_;

  DISALLOW_COPY_AND_ASSIGN(HTMLScanner);
};

namespace http_utils {

std::string AddChromeFrameToUserAgentValue(const std::string& value);

std::string RemoveChromeFrameFromUserAgentValue(const std::string& value);

std::string GetDefaultUserAgentHeaderWithCFTag();

const char* GetChromeUserAgent();

std::string GetDefaultUserAgent();

const char* GetChromeFrameUserAgent();

bool HasFrameBustingHeader(const std::string& http_headers);

std::string GetHttpHeaderFromHeaderList(const std::string& header_name,
                                        const std::string& headers);
}  

#endif  
