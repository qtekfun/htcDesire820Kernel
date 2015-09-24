// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_SAFE_PICASA_ALBUMS_INDEXER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_SAFE_PICASA_ALBUMS_INDEXER_H_

#include <queue>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/media_galleries/picasa_types.h"
#include "content/public/browser/utility_process_host_client.h"

namespace base {
class FilePath;
}

namespace IPC {
class Message;
}

namespace picasa {

class SafePicasaAlbumsIndexer : public content::UtilityProcessHostClient {
 public:
  typedef base::Callback<
      void(bool parse_success, const picasa::AlbumImagesMap&)>
      DoneCallback;

  SafePicasaAlbumsIndexer(const AlbumMap& albums, const AlbumMap& folders);

  void Start(const DoneCallback& callback);

 private:
  enum ParserState {
    INITIAL_STATE,
    STARTED_PARSING_STATE,
    FINISHED_PARSING_STATE,
  };

  
  virtual ~SafePicasaAlbumsIndexer();

  
  void ProcessFoldersBatch();

  
  void StartWorkOnIOThread();

  
  
  
  void OnIndexPicasaAlbumsContentsFinished(const AlbumImagesMap& albums_images);

  
  
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  AlbumUIDSet album_uids_;

  
  std::queue<base::FilePath> folders_queue_;

  std::vector<picasa::FolderINIContents> folders_inis_;

  
  DoneCallback callback_;

  
  
  ParserState parser_state_;

  DISALLOW_COPY_AND_ASSIGN(SafePicasaAlbumsIndexer);
};

}  

#endif  
