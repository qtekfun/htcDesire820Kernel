// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_MANIFEST_FETCH_DATA_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_MANIFEST_FETCH_DATA_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "url/gurl.h"

namespace extensions {

class ManifestFetchData {
 public:
  static const int kNeverPinged = -1;

  
  enum PingType {
    
    ROLLCALL,

    
    
    ACTIVE,
  };

  struct PingData {
    
    
    
    int rollcall_days;
    int active_days;
    
    bool is_enabled;

    PingData() : rollcall_days(0), active_days(0), is_enabled(true) {}
    PingData(int rollcall, int active, bool enabled)
        : rollcall_days(rollcall), active_days(active), is_enabled(enabled) {}
  };

  ManifestFetchData(const GURL& update_url, int request_id);
  ~ManifestFetchData();

  
  
  
  bool AddExtension(std::string id, std::string version,
                    const PingData* ping_data,
                    const std::string& update_url_data,
                    const std::string& install_source);

  const GURL& base_url() const { return base_url_; }
  const GURL& full_url() const { return full_url_; }
  const std::set<std::string>& extension_ids() const { return extension_ids_; }
  const std::set<int>& request_ids() const { return request_ids_; }

  
  bool Includes(const std::string& extension_id) const;

  
  
  bool DidPing(std::string extension_id, PingType type) const;

  
  
  
  
  void Merge(const ManifestFetchData& other);

 private:
  
  std::set<std::string> extension_ids_;

  
  std::map<std::string, PingData> pings_;

  
  GURL base_url_;

  
  
  GURL full_url_;

  
  
  
  
  std::set<int> request_ids_;

  DISALLOW_COPY_AND_ASSIGN(ManifestFetchData);
};

}  

#endif  
