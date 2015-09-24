// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_PICASA_DATA_PROVIDER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_PICASA_DATA_PROVIDER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/common/media_galleries/picasa_types.h"

namespace picasa {

class SafePicasaAlbumTableReader;
class SafePicasaAlbumsIndexer;

class PicasaDataProvider {
 public:
  typedef base::Callback<void(bool)> ReadyCallback;

  enum DataType {
    LIST_OF_ALBUMS_AND_FOLDERS_DATA,
    ALBUMS_IMAGES_DATA
  };

  explicit PicasaDataProvider(const base::FilePath& database_path);
  virtual ~PicasaDataProvider();

  
  
  void RefreshData(DataType needed_data, const ReadyCallback& ready_callback);

  
  
  scoped_ptr<AlbumMap> GetAlbums();
  scoped_ptr<AlbumMap> GetFolders();
  
  scoped_ptr<AlbumImages> FindAlbumImages(const std::string& key,
                                          base::PlatformFileError* error);

 protected:
  
  virtual void InvalidateData();

 private:
  enum State {
    STALE_DATA_STATE,
    INVALID_DATA_STATE,
    LIST_OF_ALBUMS_AND_FOLDERS_FRESH_STATE,
    ALBUMS_IMAGES_FRESH_STATE
  };

  friend class PicasaFileUtilTest;
  friend class TestPicasaDataProvider;

  
  virtual void OnTempDirWatchStarted(
      scoped_ptr<base::FilePathWatcher> temp_dir_watcher);

  
  virtual void OnTempDirChanged(const base::FilePath& temp_dir_path,
                                bool error);

  
  void DoRefreshIfNecessary();

  void OnAlbumTableReaderDone(scoped_refptr<SafePicasaAlbumTableReader> reader,
                              bool parse_success,
                              const std::vector<AlbumInfo>& albums,
                              const std::vector<AlbumInfo>& folder);

  void OnAlbumsIndexerDone(scoped_refptr<SafePicasaAlbumsIndexer> indexer,
                           bool success,
                           const picasa::AlbumImagesMap& albums_images);

  static std::string DateToPathString(const base::Time& time);
  static void UniquifyNames(const std::vector<AlbumInfo>& info_list,
                            AlbumMap* result_map);

  AlbumMap album_map_;
  AlbumMap folder_map_;
  AlbumImagesMap albums_images_;

  base::FilePath database_path_;

  State state_;

  
  std::vector<ReadyCallback> album_list_ready_callbacks_;
  std::vector<ReadyCallback> albums_index_ready_callbacks_;

  
  
  
  scoped_refptr<SafePicasaAlbumTableReader> album_table_reader_;
  scoped_refptr<SafePicasaAlbumsIndexer> albums_indexer_;

  
  
  scoped_ptr<base::FilePathWatcher> temp_dir_watcher_;

  base::WeakPtrFactory<PicasaDataProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PicasaDataProvider);
};

}  

#endif  
