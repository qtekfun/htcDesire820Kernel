// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_DRIVE_API_URL_GENERATOR_H_
#define GOOGLE_APIS_DRIVE_DRIVE_API_URL_GENERATOR_H_

#include <string>

#include "url/gurl.h"

namespace google_apis {

class DriveApiUrlGenerator {
 public:
  
  
  DriveApiUrlGenerator(const GURL& base_url, const GURL& base_download_url);
  ~DriveApiUrlGenerator();

  
  static const char kBaseUrlForProduction[];

  
  static const char kBaseDownloadUrlForProduction[];

  
  GURL GetAboutGetUrl() const;

  
  GURL GetAppsListUrl() const;

  
  GURL GetFilesGetUrl(const std::string& file_id) const;

  
  GURL GetFilesInsertUrl() const;

  
  GURL GetFilesPatchUrl(const std::string& file_id,
                        bool set_modified_date,
                        bool update_viewed_date) const;

  
  GURL GetFilesCopyUrl(const std::string& file_id) const;

  
  GURL GetFilesListUrl(int max_results,
                       const std::string& page_token,
                       const std::string& q) const;

  
  GURL GetFilesDeleteUrl(const std::string& file_id) const;

  
  GURL GetFilesTrashUrl(const std::string& file_id) const;

  
  GURL GetChangesListUrl(bool include_deleted,
                         int max_results,
                         const std::string& page_token,
                         int64 start_change_id) const;

  
  GURL GetChildrenInsertUrl(const std::string& folder_id) const;

  
  
  GURL GetChildrenDeleteUrl(const std::string& child_id,
                            const std::string& folder_id) const;

  
  GURL GetInitiateUploadNewFileUrl() const;

  
  
  GURL GetInitiateUploadExistingFileUrl(const std::string& resource_id) const;

  
  GURL GenerateDownloadFileUrl(const std::string& resource_id) const;

 private:
  const GURL base_url_;
  const GURL base_download_url_;

  
};

}  

#endif  
