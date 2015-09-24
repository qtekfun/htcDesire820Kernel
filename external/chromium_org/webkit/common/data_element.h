// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_DATA_ELEMENT_H_
#define WEBKIT_COMMON_DATA_ELEMENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "url/gurl.h"
#include "webkit/common/webkit_common_export.h"

namespace webkit_common {

class WEBKIT_COMMON_EXPORT DataElement {
 public:
  enum Type {
    TYPE_UNKNOWN = -1,
    TYPE_BYTES,
    TYPE_FILE,
    TYPE_BLOB,
    TYPE_FILE_FILESYSTEM,
  };

  DataElement();
  ~DataElement();

  Type type() const { return type_; }
  const char* bytes() const { return bytes_ ? bytes_ : &buf_[0]; }
  const base::FilePath& path() const { return path_; }
  const GURL& filesystem_url() const { return filesystem_url_; }
  const std::string& blob_uuid() const { return blob_uuid_; }
  uint64 offset() const { return offset_; }
  uint64 length() const { return length_; }
  const base::Time& expected_modification_time() const {
    return expected_modification_time_;
  }

  
  void SetToBytes(const char* bytes, int bytes_len) {
    type_ = TYPE_BYTES;
    buf_.assign(bytes, bytes + bytes_len);
    length_ = buf_.size();
  }

  
  
  void SetToSharedBytes(const char* bytes, int bytes_len) {
    type_ = TYPE_BYTES;
    bytes_ = bytes;
    length_ = bytes_len;
  }

  
  void SetToFilePath(const base::FilePath& path) {
    SetToFilePathRange(path, 0, kuint64max, base::Time());
  }

  
  void SetToBlob(const std::string& uuid) {
    SetToBlobRange(uuid, 0, kuint64max);
  }

  
  void SetToFilePathRange(const base::FilePath& path,
                          uint64 offset, uint64 length,
                          const base::Time& expected_modification_time);

  
  void SetToBlobRange(const std::string& blob_uuid,
                      uint64 offset, uint64 length);

  
  void SetToFileSystemUrlRange(const GURL& filesystem_url,
                               uint64 offset, uint64 length,
                               const base::Time& expected_modification_time);

 private:
  Type type_;
  std::vector<char> buf_;  
  const char* bytes_;  
  base::FilePath path_;  
  GURL filesystem_url_;  
  std::string blob_uuid_;
  uint64 offset_;
  uint64 length_;
  base::Time expected_modification_time_;
};

#if defined(UNIT_TEST)
inline bool operator==(const DataElement& a, const DataElement& b) {
  if (a.type() != b.type() ||
      a.offset() != b.offset() ||
      a.length() != b.length())
    return false;
  switch (a.type()) {
    case DataElement::TYPE_BYTES:
      return memcmp(a.bytes(), b.bytes(), b.length()) == 0;
    case DataElement::TYPE_FILE:
      return a.path() == b.path() &&
             a.expected_modification_time() == b.expected_modification_time();
    case DataElement::TYPE_BLOB:
      return a.blob_uuid() == b.blob_uuid();
    case DataElement::TYPE_FILE_FILESYSTEM:
      return a.filesystem_url() == b.filesystem_url();
    case DataElement::TYPE_UNKNOWN:
      NOTREACHED();
      return false;
  }
  return false;
}

inline bool operator!=(const DataElement& a, const DataElement& b) {
  return !(a == b);
}
#endif  

}  

#endif  
