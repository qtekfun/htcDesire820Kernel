// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PARSERS_METADATA_PARSER_MANAGER_H_
#define CHROME_BROWSER_PARSERS_METADATA_PARSER_MANAGER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"

class MetadataParserFactory;
class MetadataParser;

namespace base {
class FilePath;
}

class MetadataParserManager {
 public:
  
  MetadataParserManager();
  ~MetadataParserManager();

  
  static MetadataParserManager* GetInstance();

  
  
  bool RegisterParserFactory(MetadataParserFactory* parser);

  
  MetadataParser* GetParserForFile(const base::FilePath& path);

 private:
  ScopedVector<MetadataParserFactory> factories_;

  DISALLOW_COPY_AND_ASSIGN(MetadataParserManager);
};

#endif  
