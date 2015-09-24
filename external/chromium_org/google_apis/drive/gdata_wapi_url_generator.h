// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_GDATA_WAPI_URL_GENERATOR_H_
#define GOOGLE_APIS_DRIVE_GDATA_WAPI_URL_GENERATOR_H_

#include <string>

#include "url/gurl.h"

namespace google_apis {

class GDataWapiUrlGenerator {
 public:
  
  GDataWapiUrlGenerator(const GURL& base_url, const GURL& base_download_url);
  ~GDataWapiUrlGenerator();

  
  static const char kBaseUrlForProduction[];

  
  static const char kBaseDownloadUrlForProduction[];

  
  
  static GURL AddStandardUrlParams(const GURL& url);

  
  
  static GURL AddInitiateUploadUrlParams(const GURL& url);

  
  
  static GURL AddFeedUrlParams(const GURL& url,
                               int num_items_to_fetch);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL GenerateResourceListUrl(
      const GURL& override_url,
      int64 start_changestamp,
      const std::string& search_string,
      const std::string& directory_resource_id) const;

  
  
  
  
  GURL GenerateSearchByTitleUrl(
      const std::string& title,
      const std::string& directory_resource_id) const;

  
  
  GURL GenerateEditUrl(const std::string& resource_id) const;

  
  
  
  
  
  
  
  GURL GenerateEditUrlWithoutParams(const std::string& resource_id) const;

  
  
  
  
  GURL GenerateEditUrlWithEmbedOrigin(const std::string& resource_id,
                                      const GURL& embed_origin) const;

  
  
  GURL GenerateContentUrl(const std::string& resource_id) const;

  
  
  GURL GenerateResourceUrlForRemoval(const std::string& parent_resource_id,
                                     const std::string& resource_id) const;

  
  
  GURL GenerateInitiateUploadNewFileUrl(
      const std::string& parent_resource_id) const;

  
  
  GURL GenerateInitiateUploadExistingFileUrl(
      const std::string& resource_id) const;

  
  
  
  GURL GenerateResourceListRootUrl() const;

  
  
  
  GURL GenerateAccountMetadataUrl(bool include_installed_apps) const;

  
  GURL GenerateDownloadFileUrl(const std::string& resource_id) const;

 private:
  const GURL base_url_;
  const GURL base_download_url_;
};

}  

#endif  
