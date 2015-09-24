// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_UPDATE_MANIFEST_H_
#define CHROME_COMMON_EXTENSIONS_UPDATE_MANIFEST_H_

#include <string>
#include <vector>

#include "url/gurl.h"

class UpdateManifest {
 public:

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  

  
  struct Result {
    Result();
    ~Result();

    std::string extension_id;
    std::string version;
    std::string browser_min_version;

    
    GURL crx_url;
    std::string package_hash;
    int size;
    std::string package_fingerprint;

    
    GURL diff_crx_url;
    std::string diff_package_hash;
    int diff_size;
  };

  static const int kNoDaystart = -1;
  struct Results {
    Results();
    ~Results();

    std::vector<Result> list;
    
    int daystart_elapsed_seconds;
  };

  UpdateManifest();
  ~UpdateManifest();

  
  
  
  
  bool Parse(const std::string& manifest_xml);

  const Results& results() { return results_; }
  const std::string& errors() { return errors_; }

 private:
  Results results_;
  std::string errors_;

  
  void ParseError(const char* details, ...);

  DISALLOW_COPY_AND_ASSIGN(UpdateManifest);
};

#endif  
