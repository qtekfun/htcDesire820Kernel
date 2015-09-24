// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_DATA_H_
#define NET_BASE_UPLOAD_DATA_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "googleurl/src/gurl.h"
#include "base/time.h"

namespace net {

class FileStream;

class ChunkCallback {
 public:
  
  virtual void OnChunkAvailable() = 0;

 protected:
  virtual ~ChunkCallback() {}
};

class UploadData : public base::RefCounted<UploadData> {
 public:
  enum Type {
    TYPE_BYTES,
    TYPE_FILE,
    TYPE_BLOB,

    
    
    TYPE_CHUNK,
  };

  class Element {
   public:
    Element();
    ~Element();

    Type type() const { return type_; }
    
    
    void set_type(Type type) {
      type_ = type;
    }

    const std::vector<char>& bytes() const { return bytes_; }
    const FilePath& file_path() const { return file_path_; }
    uint64 file_range_offset() const { return file_range_offset_; }
    uint64 file_range_length() const { return file_range_length_; }
    
    const base::Time& expected_file_modification_time() const {
      return expected_file_modification_time_;
    }
    const GURL& blob_url() const { return blob_url_; }

    void SetToBytes(const char* bytes, int bytes_len) {
      type_ = TYPE_BYTES;
      bytes_.assign(bytes, bytes + bytes_len);
    }

    void SetToFilePath(const FilePath& path) {
      SetToFilePathRange(path, 0, kuint64max, base::Time());
    }

    
    
    
    void SetToFilePathRange(const FilePath& path,
                            uint64 offset, uint64 length,
                            const base::Time& expected_modification_time) {
      type_ = TYPE_FILE;
      file_path_ = path;
      file_range_offset_ = offset;
      file_range_length_ = length;
      expected_file_modification_time_ = expected_modification_time;
    }

    
    
    void SetToBlobUrl(const GURL& blob_url) {
      type_ = TYPE_BLOB;
      blob_url_ = blob_url;
    }

    
    
    
    void SetToChunk(const char* bytes, int bytes_len, bool is_last_chunk);

    bool is_last_chunk() const { return is_last_chunk_; }
    
    void set_is_last_chunk(bool is_last_chunk) {
      is_last_chunk_ = is_last_chunk;
    }

    
    
    
    uint64 GetContentLength();

    
    
    
    
    FileStream* NewFileStreamForReading();

   private:
    
    void SetContentLength(uint64 content_length) {
      override_content_length_ = true;
      content_length_ = content_length;
    }

    Type type_;
    std::vector<char> bytes_;
    FilePath file_path_;
    uint64 file_range_offset_;
    uint64 file_range_length_;
    base::Time expected_file_modification_time_;
    GURL blob_url_;
    bool is_last_chunk_;
    bool override_content_length_;
    bool content_length_computed_;
    uint64 content_length_;
    FileStream* file_stream_;

    FRIEND_TEST_ALL_PREFIXES(UploadDataStreamTest, FileSmallerThanLength);
    FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionTest,
                             UploadFileSmallerThanLength);
  };

  UploadData();

  void AppendBytes(const char* bytes, int bytes_len);

  void AppendFile(const FilePath& file_path);

  void AppendFileRange(const FilePath& file_path,
                       uint64 offset, uint64 length,
                       const base::Time& expected_modification_time);

  void AppendBlob(const GURL& blob_url);

  
  
  void AppendChunk(const char* bytes, int bytes_len, bool is_last_chunk);

  
  void set_chunk_callback(ChunkCallback* callback);

  
  
  void set_is_chunked(bool set) { is_chunked_ = set; }
  bool is_chunked() const { return is_chunked_; }

  
  uint64 GetContentLength();

  std::vector<Element>* elements() {
    return &elements_;
  }

  void SetElements(const std::vector<Element>& elements);

  void swap_elements(std::vector<Element>* elements) {
    elements_.swap(*elements);
  }

  
  
  
  void set_identifier(int64 id) { identifier_ = id; }
  int64 identifier() const { return identifier_; }

 private:
  friend class base::RefCounted<UploadData>;

  ~UploadData();

  std::vector<Element> elements_;
  int64 identifier_;
  ChunkCallback* chunk_callback_;
  bool is_chunked_;

  DISALLOW_COPY_AND_ASSIGN(UploadData);
};

#if defined(UNIT_TEST)
inline bool operator==(const UploadData::Element& a,
                       const UploadData::Element& b) {
  if (a.type() != b.type())
    return false;
  if (a.type() == UploadData::TYPE_BYTES)
    return a.bytes() == b.bytes();
  if (a.type() == UploadData::TYPE_FILE) {
    return a.file_path() == b.file_path() &&
           a.file_range_offset() == b.file_range_offset() &&
           a.file_range_length() == b.file_range_length() &&
           a.expected_file_modification_time() ==
              b.expected_file_modification_time();
  }
  if (a.type() == UploadData::TYPE_BLOB)
    return a.blob_url() == b.blob_url();
  return false;
}

inline bool operator!=(const UploadData::Element& a,
                       const UploadData::Element& b) {
  return !(a == b);
}
#endif  

}  

#endif  
