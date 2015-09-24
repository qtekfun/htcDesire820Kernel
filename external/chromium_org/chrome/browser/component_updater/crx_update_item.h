// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_CRX_UPDATE_ITEM_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_CRX_UPDATE_ITEM_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/version.h"
#include "chrome/browser/component_updater/component_updater_service.h"
#include "chrome/browser/component_updater/crx_downloader.h"

class CUResourceThrottle;

namespace component_updater {

struct CrxUpdateItem {
  enum Status {
    kNew,
    kChecking,
    kCanUpdate,
    kDownloadingDiff,
    kDownloading,
    kUpdatingDiff,
    kUpdating,
    kUpdated,
    kUpToDate,
    kNoUpdate,
    kLastStatus
  };

  
  
  Status status;

  std::string id;
  CrxComponent component;

  base::Time last_check;

  
  std::vector<GURL> crx_urls;
  std::vector<GURL> crx_diffurls;

  
  Version previous_version;
  Version next_version;
  std::string previous_fp;
  std::string next_fp;

  
  bool on_demand;

  
  bool diff_update_failed;

  
  
  
  
  
  
  int error_category;
  int error_code;
  int extra_code1;
  int diff_error_category;
  int diff_error_code;
  int diff_extra_code1;

  std::vector<CrxDownloader::DownloadMetrics> download_metrics;

  std::vector<base::WeakPtr<CUResourceThrottle> > throttles;

  CrxUpdateItem();
  ~CrxUpdateItem();

  
  class FindById {
   public:
    explicit FindById(const std::string& id) : id_(id) {}

    bool operator() (CrxUpdateItem* item) const {
      return (item->id == id_);
    }
   private:
    const std::string& id_;
  };
};

}  

#endif  
