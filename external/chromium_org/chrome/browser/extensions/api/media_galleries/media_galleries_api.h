// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_MEDIA_GALLERIES_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_MEDIA_GALLERIES_MEDIA_GALLERIES_API_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/media_galleries/media_file_system_registry.h"
#include "chrome/browser/storage_monitor/media_storage_util.h"
#include "chrome/common/extensions/api/media_galleries.h"

namespace MediaGalleries = extensions::api::media_galleries;

namespace extensions {

class MediaGalleriesGetMediaFileSystemsFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleries.getMediaFileSystems",
                             MEDIAGALLERIES_GETMEDIAFILESYSTEMS)

 protected:
  virtual ~MediaGalleriesGetMediaFileSystemsFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void OnPreferencesInit(
    MediaGalleries::GetMediaFileSystemsInteractivity interactive);

  
  void AlwaysShowDialog(const std::vector<MediaFileSystemInfo>& filesystems);

  
  void ShowDialogIfNoGalleries(
      const std::vector<MediaFileSystemInfo>& filesystems);

  
  
  void GetAndReturnGalleries();

  
  void ReturnGalleries(const std::vector<MediaFileSystemInfo>& filesystems);

  
  void ShowDialog();

  
  
  void GetMediaFileSystemsForExtension(const MediaFileSystemsCallback& cb);
};

class MediaGalleriesGetAllMediaFileSystemMetadataFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleries.getAllMediaFileSystemMetadata",
                             MEDIAGALLERIES_GETALLMEDIAFILESYSTEMMETADATA)

 protected:
  virtual ~MediaGalleriesGetAllMediaFileSystemMetadataFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  
  
  void OnPreferencesInit();

  
  
  void OnGetGalleries(const MediaGalleryPrefIdSet& permitted_gallery_ids,
                      const MediaStorageUtil::DeviceIdSet* available_devices);
};

class MediaGalleriesGetMetadataFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("mediaGalleries.getMetadata",
                             MEDIAGALLERIES_GETMETADATA)

 protected:
  virtual ~MediaGalleriesGetMetadataFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  
  void OnPreferencesInit(bool mime_type_only, const std::string& blob_uuid);

  void SniffMimeType(bool mime_type_only, scoped_ptr<std::string> blob_header);
};

}  

#endif  
