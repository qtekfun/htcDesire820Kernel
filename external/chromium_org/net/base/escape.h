// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ESCAPE_H_
#define NET_BASE_ESCAPE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "net/base/net_export.h"

namespace net {


NET_EXPORT std::string EscapeQueryParamValue(const std::string& text,
                                             bool use_plus);

NET_EXPORT std::string EscapePath(const std::string& path);

NET_EXPORT std::string EscapeUrlEncodedData(const std::string& path,
                                            bool use_plus);

NET_EXPORT std::string EscapeNonASCII(const std::string& input);

NET_EXPORT std::string EscapeExternalHandlerValue(const std::string& text);

NET_EXPORT void AppendEscapedCharForHTML(char c, std::string* output);

NET_EXPORT std::string EscapeForHTML(const std::string& text);
NET_EXPORT base::string16 EscapeForHTML(const base::string16& text);


class UnescapeRule {
 public:
  
  
  typedef uint32 Type;

  enum {
    
    NONE = 0,

    
    
    
    
    
    NORMAL = 1,

    
    
    
    
    SPACES = 2,

    
    
    
    
    
    URL_SPECIAL_CHARS = 4,

    
    
    
    CONTROL_CHARS = 8,

    
    REPLACE_PLUS_WITH_SPACE = 16,
  };
};

NET_EXPORT std::string UnescapeURLComponent(const std::string& escaped_text,
                                            UnescapeRule::Type rules);
NET_EXPORT base::string16 UnescapeURLComponent(
    const base::string16& escaped_text,
    UnescapeRule::Type rules);

NET_EXPORT base::string16 UnescapeAndDecodeUTF8URLComponent(
    const std::string& text,
    UnescapeRule::Type rules,
    size_t* offset_for_adjustment);
NET_EXPORT base::string16 UnescapeAndDecodeUTF8URLComponentWithOffsets(
    const std::string& text,
    UnescapeRule::Type rules,
    std::vector<size_t>* offsets_for_adjustment);

NET_EXPORT base::string16 UnescapeForHTML(const base::string16& text);

namespace internal {


struct NET_EXPORT_PRIVATE AdjustEncodingOffset {
  typedef std::vector<size_t> Adjustments;

  explicit AdjustEncodingOffset(const Adjustments& adjustments);
  void operator()(size_t& offset);

  const Adjustments& adjustments;
};

}  

}  

#endif  
