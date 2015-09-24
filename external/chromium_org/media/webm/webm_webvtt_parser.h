// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_WEBM_WEBVTT_PARSER_H_
#define MEDIA_WEBM_WEBM_WEBVTT_PARSER_H_

#include <string>

#include "base/basictypes.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT WebMWebVTTParser {
 public:
  
  static void Parse(const uint8* payload, int payload_size,
                    std::string* id,
                    std::string* settings,
                    std::string* content);

 private:
  
  
  WebMWebVTTParser(const uint8* payload, int payload_size);

  
  void Parse(std::string* id, std::string* settings, std::string* content);
  
  
  bool GetByte(uint8* byte);

  
  void UngetByte();

  
  void ParseLine(std::string* line);

  
  const uint8* ptr_;
  const uint8* const ptr_end_;

  DISALLOW_COPY_AND_ASSIGN(WebMWebVTTParser);
};

}  

#endif  
