// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_DOWNLOADER_DELEGATE_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_DOWNLOADER_DELEGATE_H_

#include <set>
#include <string>

#include "base/time/time.h"
#include "chrome/browser/extensions/updater/manifest_fetch_data.h"

class GURL;

namespace base {
class FilePath;
}

namespace extensions {

class ExtensionDownloaderDelegate {
 public:
  virtual ~ExtensionDownloaderDelegate();

  
  
  enum Error {
    
    DISABLED,

    
    MANIFEST_FETCH_FAILED,

    
    MANIFEST_INVALID,

    
    
    NO_UPDATE_AVAILABLE,

    
    
    CRX_FETCH_FAILED,
  };

  
  
  struct PingResult {
    PingResult();
    ~PingResult();

    
    bool did_ping;

    
    
    base::Time day_start;
  };

  
  
  
  
  
  
  
  
  
  
  
  

  
  
  virtual void OnExtensionDownloadFailed(const std::string& id,
                                         Error error,
                                         const PingResult& ping_result,
                                         const std::set<int>& request_ids);

  
  
  
  virtual void OnExtensionDownloadFinished(
      const std::string& id,
      const base::FilePath& path,
      const GURL& download_url,
      const std::string& version,
      const PingResult& ping_result,
      const std::set<int>& request_ids) = 0;

  
  

  
  
  
  virtual bool GetPingDataForExtension(const std::string& id,
                                       ManifestFetchData::PingData* ping);

  
  
  virtual std::string GetUpdateUrlData(const std::string& id);

  
  
  virtual bool IsExtensionPending(const std::string& id) = 0;

  
  
  virtual bool GetExtensionExistingVersion(const std::string& id,
                                           std::string* version) = 0;
};

}  

#endif  
