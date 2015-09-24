// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ESCAPE_H_
#define NET_BASE_ESCAPE_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "net/base/net_export.h"


std::string EscapePath(const std::string& path);

std::string EscapeUrlEncodedData(const std::string& path);

std::string EscapeNonASCII(const std::string& input);

std::string EscapeExternalHandlerValue(const std::string& text);

void AppendEscapedCharForHTML(char c, std::string* output);

std::string EscapeForHTML(const std::string& text);
string16 EscapeForHTML(const string16& text);


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

std::string UnescapeURLComponent(const std::string& escaped_text,
                                 UnescapeRule::Type rules);
string16 UnescapeURLComponent(const string16& escaped_text,
                              UnescapeRule::Type rules);

string16 UnescapeAndDecodeUTF8URLComponent(const std::string& text,
                                           UnescapeRule::Type rules,
                                           size_t* offset_for_adjustment);
string16 UnescapeAndDecodeUTF8URLComponentWithOffsets(
    const std::string& text,
    UnescapeRule::Type rules,
    std::vector<size_t>* offsets_for_adjustment);

string16 UnescapeForHTML(const string16& text);


NET_EXPORT std::string EscapeQueryParamValue(const std::string& text, bool use_plus);
bool EscapeQueryParamValue(const string16& text, const char* codepage,
                           bool use_plus, string16* escaped);

string16 EscapeQueryParamValueUTF8(const string16& text, bool use_plus);


struct AdjustEncodingOffset {
  typedef std::vector<size_t> Adjustments;

  explicit AdjustEncodingOffset(const Adjustments& adjustments);
  void operator()(size_t& offset);

  const Adjustments& adjustments;
};

#endif  
