// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_UTIL_H_
#define NET_HTTP_HTTP_UTIL_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/string_tokenizer.h"
#include "googleurl/src/gurl.h"
#include "net/http/http_byte_range.h"

#define HTTP_LWS " \t"

namespace net {

class UploadDataStream;

class HttpUtil {
 public:
  
  
  static std::string PathForRequest(const GURL& url);

  
  
  
  static std::string SpecForRequest(const GURL& url);

  
  
  
  
  static size_t FindDelimiter(const std::string& line,
                              size_t search_start,
                              char delimiter);

  
  
  
  
  static void ParseContentType(const std::string& content_type_str,
                               std::string* mime_type,
                               std::string* charset,
                               bool *had_charset);

  
  
  
  
  
  
  static bool ParseRanges(const std::string& headers,
                          std::vector<HttpByteRange>* ranges);

  
  
  static bool ParseRangeHeader(const std::string& range_specifier,
                               std::vector<HttpByteRange>* ranges);

  
  
  
  static bool HasHeader(const std::string& headers, const char* name);

  
  
  
  
  static std::string StripHeaders(const std::string& headers,
                                  const char* const headers_to_remove[],
                                  size_t headers_to_remove_len);

  
  
  
  static bool IsNonCoalescingHeader(std::string::const_iterator name_begin,
                                    std::string::const_iterator name_end);
  static bool IsNonCoalescingHeader(const std::string& name) {
    return IsNonCoalescingHeader(name.begin(), name.end());
  }

  
  
  
  static bool IsLWS(char c);

  
  static void TrimLWS(std::string::const_iterator* begin,
                      std::string::const_iterator* end);

  
  static bool IsQuote(char c);

  
  
  
  
  
  static std::string Unquote(std::string::const_iterator begin,
                             std::string::const_iterator end);

  
  static std::string Unquote(const std::string& str);

  
  static std::string Quote(const std::string& str);

  
  
  
  static int LocateStartOfStatusLine(const char* buf, int buf_len);

  
  
  // servers only send back LFs (e.g., Unix-based CGI scripts written using the
  
  
  
  static int LocateEndOfHeaders(const char* buf, int buf_len, int i = 0);

  
  
  
  
  
  static std::string AssembleRawHeaders(const char* buf, int buf_len);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static std::string GenerateAcceptLanguageHeader(
      const std::string& raw_language_list);

  
  
  
  static std::string GenerateAcceptCharsetHeader(const std::string& charset);

  
  
  static void AppendHeaderIfMissing(const char* header_name,
                                    const std::string& header_value,
                                    std::string* headers);

  
  
  
  
  class HeadersIterator {
   public:
    HeadersIterator(std::string::const_iterator headers_begin,
                    std::string::const_iterator headers_end,
                    const std::string& line_delimiter);
    ~HeadersIterator();

    
    
    
    bool GetNext();

    
    
    
    
    
    
    bool AdvanceTo(const char* lowercase_name);

    void Reset() {
      lines_.Reset();
    }

    std::string::const_iterator name_begin() const {
      return name_begin_;
    }
    std::string::const_iterator name_end() const {
      return name_end_;
    }
    std::string name() const {
      return std::string(name_begin_, name_end_);
    }

    std::string::const_iterator values_begin() const {
      return values_begin_;
    }
    std::string::const_iterator values_end() const {
      return values_end_;
    }
    std::string values() const {
      return std::string(values_begin_, values_end_);
    }

   private:
    StringTokenizer lines_;
    std::string::const_iterator name_begin_;
    std::string::const_iterator name_end_;
    std::string::const_iterator values_begin_;
    std::string::const_iterator values_end_;
  };

  
  
  
  
  
  
  
  
  
  
  
  class ValuesIterator {
   public:
    ValuesIterator(std::string::const_iterator values_begin,
                   std::string::const_iterator values_end,
                   char delimiter);
    ~ValuesIterator();

    
    
    bool GetNext();

    std::string::const_iterator value_begin() const {
      return value_begin_;
    }
    std::string::const_iterator value_end() const {
      return value_end_;
    }
    std::string value() const {
      return std::string(value_begin_, value_end_);
    }

   private:
    StringTokenizer values_;
    std::string::const_iterator value_begin_;
    std::string::const_iterator value_end_;
  };

  
  
  
  
  
  
  
  class NameValuePairsIterator {
   public:
    NameValuePairsIterator(std::string::const_iterator begin,
                           std::string::const_iterator end,
                           char delimiter);
    ~NameValuePairsIterator();

    
    
    
    bool GetNext();

    
    bool valid() const { return valid_; }

    
    std::string::const_iterator name_begin() const { return name_begin_; }
    std::string::const_iterator name_end() const { return name_end_; }
    std::string name() const { return std::string(name_begin_, name_end_); }

    
    std::string::const_iterator value_begin() const {
      return value_is_quoted_ ? unquoted_value_.begin() : value_begin_;
    }
    std::string::const_iterator value_end() const {
      return value_is_quoted_ ? unquoted_value_.end() : value_end_;
    }
    std::string value() const {
      return value_is_quoted_ ? unquoted_value_ : std::string(value_begin_,
                                                               value_end_);
    }

   private:
    HttpUtil::ValuesIterator props_;
    bool valid_;

    std::string::const_iterator begin_;
    std::string::const_iterator end_;

    std::string::const_iterator name_begin_;
    std::string::const_iterator name_end_;

    std::string::const_iterator value_begin_;
    std::string::const_iterator value_end_;

    
    
    
    std::string unquoted_value_;

    bool value_is_quoted_;
  };
};

}  

#endif  
