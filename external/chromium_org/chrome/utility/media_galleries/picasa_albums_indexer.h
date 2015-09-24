// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_MEDIA_GALLERIES_PICASA_ALBUMS_INDEXER_H_
#define CHROME_UTILITY_MEDIA_GALLERIES_PICASA_ALBUMS_INDEXER_H_

#include <map>
#include <set>
#include <string>

#include "base/files/file_path.h"
#include "chrome/common/media_galleries/picasa_types.h"

namespace picasa {

class PicasaAlbumsIndexer {
 public:
  explicit PicasaAlbumsIndexer(const AlbumUIDSet& album_uids);
  ~PicasaAlbumsIndexer();

  
  void ParseFolderINI(
      const std::vector<picasa::FolderINIContents>& folders_inis);

  const AlbumImagesMap& albums_images() const { return albums_images_; }

 private:
  AlbumImagesMap albums_images_;

  DISALLOW_COPY_AND_ASSIGN(PicasaAlbumsIndexer);
};

}  

#endif  
