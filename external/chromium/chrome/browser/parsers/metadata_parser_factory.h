// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PARSERS_METADATA_PARSER_FACTORY_H_
#define CHROME_BROWSER_PARSERS_METADATA_PARSER_FACTORY_H_
#pragma once

#include "chrome/browser/parsers/metadata_parser.h"

class FilePath;

class MetadataParserFactory {
 public:
  MetadataParserFactory() {}
  virtual ~MetadataParserFactory() {}

  
  
  virtual bool CanParse(const FilePath& path,
                        char* bytes,
                        int bytes_size) = 0;

  
  
  virtual MetadataParser* CreateParser(const FilePath& path) = 0;
};

#endif  
