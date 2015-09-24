// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_SAFE_PICASA_ALBUM_TABLE_READER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_SAFE_PICASA_ALBUM_TABLE_READER_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/media_galleries/picasa_types.h"
#include "content/public/browser/utility_process_host.h"
#include "content/public/browser/utility_process_host_client.h"

namespace base {
class FilePath;
}

namespace IPC {
class Message;
}

namespace picasa {

class SafePicasaAlbumTableReader : public content::UtilityProcessHostClient {
 public:
  typedef base::Callback<void(bool parse_success,
                              const std::vector<AlbumInfo>&,
                              const std::vector<AlbumInfo>&)>
      ParserCallback;

  
  explicit SafePicasaAlbumTableReader(const AlbumTableFiles& album_table_files);

  void Start(const ParserCallback& callback);

 private:
  enum ParserState {
    INITIAL_STATE,
    PINGED_UTILITY_PROCESS_STATE,
    STARTED_PARSING_STATE,
    FINISHED_PARSING_STATE,
  };

  
  virtual ~SafePicasaAlbumTableReader();

  
  void StartWorkOnIOThread();

  
  
  
  void OnProcessStarted();

  
  
  
  void OnParsePicasaPMPDatabaseFinished(bool parse_success,
                                        const std::vector<AlbumInfo>& albums,
                                        const std::vector<AlbumInfo>& folders);

  
  
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  const AlbumTableFiles album_table_files_;

  
  base::WeakPtr<content::UtilityProcessHost> utility_process_host_;

  
  ParserCallback callback_;

  
  
  ParserState parser_state_;

  DISALLOW_COPY_AND_ASSIGN(SafePicasaAlbumTableReader);
};

}  

#endif  
