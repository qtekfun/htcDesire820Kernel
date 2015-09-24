// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_DRIVE_APP_REGISTRY_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_DRIVE_APP_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "google_apis/drive/gdata_wapi_parser.h"
#include "url/gurl.h"

namespace google_apis {
class AppList;
}  

namespace drive {

class JobScheduler;

struct DriveAppInfo {
  DriveAppInfo();
  DriveAppInfo(const std::string& app_id,
               const google_apis::InstalledApp::IconList& app_icons,
               const google_apis::InstalledApp::IconList& document_icons,
               const std::string& web_store_id,
               const std::string& app_name,
               const std::string& object_type,
               bool is_primary_selector);
  ~DriveAppInfo();

  
  std::string app_id;
  
  
  google_apis::InstalledApp::IconList app_icons;
  
  
  google_apis::InstalledApp::IconList document_icons;
  
  std::string web_store_id;
  
  std::string app_name;
  
  std::string object_type;
  
  bool is_primary_selector;
};

class DriveAppRegistry {
 public:
  explicit DriveAppRegistry(JobScheduler* scheduler);
  ~DriveAppRegistry();

  
  
  void GetAppsForFile(const base::FilePath::StringType& file_extension,
                      const std::string& mime_type,
                      ScopedVector<DriveAppInfo>* apps) const;

  
  void Update();

  
  void UpdateFromAppList(const google_apis::AppList& app_list);

 private:

  
  
  typedef std::multimap<std::string, DriveAppInfo*> DriveAppFileSelectorMap;

  
  
  void UpdateAfterGetAppList(google_apis::GDataErrorCode gdata_error,
                             scoped_ptr<google_apis::AppList> app_list);

  
  
  static void AddAppSelectorList(
      const std::string& web_store_id,
      const std::string& app_name,
      const google_apis::InstalledApp::IconList& app_icons,
      const google_apis::InstalledApp::IconList& document_icons,
      const std::string& object_type,
      const std::string& app_id,
      bool is_primary_selector,
      const ScopedVector<std::string>& selectors,
      DriveAppFileSelectorMap* map);

  
  void FindAppsForSelector(const std::string& selector,
                           const DriveAppFileSelectorMap& map,
                           std::vector<DriveAppInfo*>* matched_apps) const;

  JobScheduler* scheduler_;

  
  DriveAppFileSelectorMap app_extension_map_;

  
  DriveAppFileSelectorMap app_mimetypes_map_;

  bool is_updating_;

  
  
  base::WeakPtrFactory<DriveAppRegistry> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(DriveAppRegistry);
};

namespace util {

const int kPreferredIconSize = 16;

GURL FindPreferredIcon(const google_apis::InstalledApp::IconList& icons,
                       int preferred_size);

}  

}  

#endif  
