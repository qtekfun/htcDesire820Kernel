// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_STREAM_PARSER_FACTORY_H_
#define MEDIA_FILTERS_STREAM_PARSER_FACTORY_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"
#include "media/base/media_log.h"

namespace media {

class StreamParser;

class MEDIA_EXPORT StreamParserFactory {
 public:
  
  
  static bool IsTypeSupported(
      const std::string& type, const std::vector<std::string>& codecs);

  
  
  
  
  
  
  
  
  
  static scoped_ptr<StreamParser> Create(
      const std::string& type, const std::vector<std::string>& codecs,
      const LogCB& log_cb, bool* has_audio, bool* has_video);
};

}  

#endif  
