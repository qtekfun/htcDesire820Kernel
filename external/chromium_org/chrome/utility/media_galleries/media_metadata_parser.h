// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_MEDIA_GALLERIES_MEDIA_METADATA_PARSER_H_
#define CHROME_UTILITY_MEDIA_GALLERIES_MEDIA_METADATA_PARSER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/extensions/api/media_galleries.h"

namespace metadata {

class DataReader {
 public:
  typedef base::Callback<void(const std::string& bytes)> ReadBytesCallback;

  virtual ~DataReader() {}

  
  virtual void ReadBytes(int64 offset, int64 length,
                         const ReadBytesCallback& callback) = 0;
};

class MediaMetadataParser {
 public:
  typedef extensions::api::media_galleries::MediaMetadata MediaMetadata;
  typedef base::Callback<void(scoped_ptr<MediaMetadata>)> MetadataCallback;

  
  
  
  MediaMetadataParser(DataReader* reader, const std::string& mime_type);

  ~MediaMetadataParser();

  
  void Start(const MetadataCallback& callback);

 private:
  scoped_ptr<DataReader> reader_;

  MetadataCallback callback_;

  scoped_ptr<MediaMetadata> metadata_;

  DISALLOW_COPY_AND_ASSIGN(MediaMetadataParser);
};

}  

#endif  
