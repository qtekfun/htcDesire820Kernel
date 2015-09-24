// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_UPDATE_RESPONSE_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_UPDATE_RESPONSE_H_

#include <string>
#include <vector>
#include "base/basictypes.h"
#include "url/gurl.h"

namespace component_updater {

class UpdateResponse {
 public:
  
  struct Result {
    struct Manifest {
      struct Package {
        Package();
        ~Package();

        std::string fingerprint;

        
        std::string name;
        std::string hash_sha256;
        int size;

        
        std::string namediff;
        std::string hashdiff_sha256;
        int sizediff;
      };

      Manifest();
      ~Manifest();

      std::string version;
      std::string browser_min_version;
      std::vector<Package> packages;
    };

    Result();
    ~Result();

    std::string extension_id;

    
    
    std::vector<GURL> crx_urls;
    std::vector<GURL> crx_diffurls;

    Manifest manifest;
  };

  static const int kNoDaystart = -1;
  struct Results {
    Results();
    ~Results();

    
    int daystart_elapsed_seconds;
    std::vector<Result> list;
  };

  UpdateResponse();
  ~UpdateResponse();

  
  
  
  
  bool Parse(const std::string& manifest_xml);

  const Results& results() const { return results_; }
  const std::string& errors() const { return errors_; }

 private:
  Results results_;
  std::string errors_;

  
  void ParseError(const char* details, ...);

  DISALLOW_COPY_AND_ASSIGN(UpdateResponse);
};

}  

#endif  

