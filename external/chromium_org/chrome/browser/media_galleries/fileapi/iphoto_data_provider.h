// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IPHOTO_DATA_PROVIDER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IPHOTO_DATA_PROVIDER_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/media_galleries/fileapi/iapps_data_provider.h"
#include "chrome/browser/media_galleries/fileapi/safe_iapps_library_parser.h"
#include "chrome/common/media_galleries/iphoto_library.h"

namespace iphoto {

class IPhotoDataProvider : public iapps::IAppsDataProvider {
 public:
  explicit IPhotoDataProvider(const base::FilePath& library_path);
  virtual ~IPhotoDataProvider();

  
  virtual void DoParseLibrary(const base::FilePath& library_path,
                              const ReadyCallback& ready_callback) OVERRIDE;

  virtual std::vector<std::string> GetAlbumNames() const;
  virtual std::map<std::string, base::FilePath> GetAlbumContents(
      const std::string& album) const;
  virtual base::FilePath GetPhotoLocationInAlbum(
      const std::string& album,
      const std::string& filename) const;

  virtual bool HasOriginals(const std::string& album) const;
  virtual std::map<std::string, base::FilePath> GetOriginals(
      const std::string& album) const;
  virtual base::FilePath GetOriginalPhotoLocation(
      const std::string& album,
      const std::string& filename) const;

 private:
  typedef base::hash_map<std::string, base::FilePath> FileIndex;
  
  typedef base::hash_map<std::string, FileIndex> DirIndex;

  void OnLibraryParsed(const ReadyCallback& ready_callback,
                       bool result,
                       const parser::Library& library);

  void BuildIndices(const parser::Library& library);

  
  DirIndex dir_index_;

  
  DirIndex originals_index_;

  scoped_refptr<iapps::SafeIAppsLibraryParser> xml_parser_;

  base::WeakPtrFactory<IPhotoDataProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(IPhotoDataProvider);
};

}  

#endif  
