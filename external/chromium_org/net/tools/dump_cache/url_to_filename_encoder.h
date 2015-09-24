// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef NET_TOOLS_DUMP_CACHE_URL_TO_FILENAME_ENCODER_H_
#define NET_TOOLS_DUMP_CACHE_URL_TO_FILENAME_ENCODER_H_

#include <string>

#include "base/strings/string_util.h"
#include "net/tools/dump_cache/url_utilities.h"

namespace net {

class UrlToFilenameEncoder {
 public:
  
  
  
  
  
  static std::string Encode(const std::string& url, std::string base_path,
                            bool legacy_escape) {
    std::string filename;
    if (!legacy_escape) {
      std::string url_no_scheme = UrlUtilities::GetUrlHostPath(url);
      EncodeSegment(base_path, url_no_scheme, '/', &filename);
#ifdef WIN32
      ReplaceAll(&filename, "/", "\\");
#endif
    } else {
      std::string clean_url(url);
      if (clean_url.length() && clean_url[clean_url.length()-1] == '/')
        clean_url.append("index.html");

      std::string host = UrlUtilities::GetUrlHost(clean_url);
      filename.append(base_path);
      filename.append(host);
#ifdef WIN32
      filename.append("\\");
#else
      filename.append("/");
#endif

      std::string url_filename = UrlUtilities::GetUrlPath(clean_url);
      
      if (url_filename[0] == '/')
        url_filename = url_filename.substr(1);

      
      ConvertToSlashes(&url_filename);

      
      StripDoubleSlashes(&url_filename);

      
      url_filename = LegacyEscape(url_filename);
      filename.append(url_filename);

#ifndef WIN32
      
      const std::string slash("/");
      const std::string backslash("\\");
      ReplaceAll(&filename, backslash, slash);
#endif
    }

    return filename;
  }

  
  
  
  
  
  
  
  static void EncodeSegment(
      const std::string& filename_prefix,
      const std::string& escaped_ending,
      char dir_separator,
      std::string* encoded_filename);

  
  
  static bool Decode(const std::string& encoded_filename,
                     char dir_separator,
                     std::string* decoded_url);

  static const char kEscapeChar;
  static const char kTruncationChar;
  static const size_t kMaximumSubdirectoryLength;

  friend class UrlToFilenameEncoderTest;

 private:
  
  
  
  
  
  
  
  static void AppendSegment(std::string* segment, std::string* dest);

  
  static std::string LegacyEscape(const std::string& path);

  
  static void ReplaceAll(std::string* str, const std::string& from,
                         const std::string& to) {
    std::string::size_type pos(0);
    while ((pos = str->find(from, pos)) != std::string::npos) {
      str->replace(pos, from.size(), to);
      pos += from.size();
    }
  }

  
  static void ConvertToSlashes(std::string* path) {
    const std::string slash("/");
    const std::string backslash("\\");
    ReplaceAll(path, slash, backslash);
  }

  
  static void StripDoubleSlashes(std::string* path) {
    const std::string doubleslash("\\\\");
    const std::string escaped_doubleslash("%5C%5C");
    ReplaceAll(path, doubleslash, escaped_doubleslash);
  }
};

}  

#endif  
