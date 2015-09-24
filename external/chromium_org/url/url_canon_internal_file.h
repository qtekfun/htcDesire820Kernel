// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_CANON_INTERNAL_FILE_H_
#define URL_URL_CANON_INTERNAL_FILE_H_



#include "url/url_file.h"
#include "url/url_parse_internal.h"

using namespace url_canon;

template<typename CHAR>
static int FileDoDriveSpec(const CHAR* spec, int begin, int end,
                           CanonOutput* output) {
  
  
  int num_slashes = CountConsecutiveSlashes(spec, begin, end);
  int after_slashes = begin + num_slashes;

  if (!DoesBeginWindowsDriveSpec(spec, after_slashes, end))
    return begin;  

  
  

  
  if (spec[after_slashes] >= 'a' && spec[after_slashes] <= 'z')
    output->push_back(spec[after_slashes] - 'a' + 'A');
  else
    output->push_back(static_cast<char>(spec[after_slashes]));

  
  output->push_back(':');
  output->push_back('/');
  return after_slashes + 2;
}

template<typename CHAR, typename UCHAR>
static void FileDoPath(const CHAR* spec, int begin, int end,
                       CanonOutput* output) {
  
  
  
  int num_slashes = CountConsecutiveSlashes(spec, begin, end);
  int after_slashes = begin + num_slashes;

  
  
  
  
  ParsedURL::Component sub_path(after_slashes, end - after_slashes);
  if (sub_path.len > 0) {
    
    
    ParsedURL::Component fake_output_path;
    URLCanonInternal<CHAR, UCHAR>::DoPath(
        spec, sub_path, output, &fake_output_path);
  }
}

template<typename CHAR, typename UCHAR>
static bool DoCanonicalizeFileURL(const URLComponentSource<CHAR>& source,
                                  const ParsedURL& parsed,
                                  CanonOutput* output,
                                  ParsedURL* new_parsed) {
  
  new_parsed->username = ParsedURL::Component(0, -1);
  new_parsed->password = ParsedURL::Component(0, -1);
  new_parsed->port = ParsedURL::Component(0, -1);

  
  
  new_parsed->scheme.begin = output->length();
  output->push_back('f');
  output->push_back('i');
  output->push_back('l');
  output->push_back('e');
  new_parsed->scheme.len = output->length() - new_parsed->scheme.begin;
  output->push_back(':');

  
  output->push_back('/');
  output->push_back('/');

  
  
  
  
  
  bool success = URLCanonInternal<CHAR, UCHAR>::DoHost(
      source.host, parsed.host, output, &new_parsed->host);

  
  
  new_parsed->path.begin = output->length();
  output->push_back('/');

  
  int after_drive = FileDoDriveSpec(source.path, parsed.path.begin,
                                    parsed.path.end(), output);

  
  FileDoPath<CHAR, UCHAR>(source.path, after_drive, parsed.path.end(), output);
  new_parsed->path.len = output->length() - new_parsed->path.begin;

  
  success &= URLCanonInternal<CHAR, UCHAR>::DoQuery(
      source.query, parsed.query, output, &new_parsed->query);
  success &= URLCanonInternal<CHAR, UCHAR>::DoRef(
      source.ref, parsed.ref, output, &new_parsed->ref);

  return success;
}

#endif  
