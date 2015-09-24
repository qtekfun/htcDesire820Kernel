// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_MEDIA_GALLERIES_PMP_COLUMN_READER_H_
#define CHROME_UTILITY_MEDIA_GALLERIES_PMP_COLUMN_READER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "chrome/common/media_galleries/pmp_constants.h"

namespace base {
class FilePath;
}

namespace picasa {

class PmpColumnReader {
 public:
  PmpColumnReader();
  virtual ~PmpColumnReader();

  
  
  bool ReadFile(base::PlatformFile file, const PmpFieldType expected_type);

  
  
  
  bool ReadString(const uint32 row, std::string* result) const;
  bool ReadUInt32(const uint32 row, uint32* result) const;
  bool ReadDouble64(const uint32 row, double* result) const;
  bool ReadUInt8(const uint32 row, uint8* result) const;
  bool ReadUInt64(const uint32 row, uint64* result) const;

  
  uint32 rows_read() const;

 private:
  bool ParseData(const PmpFieldType expected_type);
  
  int64 IndexStrings();

  
  scoped_ptr<uint8[]> data_;
  int64 length_;

  
  PmpFieldType field_type_;
  uint32 rows_read_;

  
  std::vector<const char*> strings_;

  DISALLOW_COPY_AND_ASSIGN(PmpColumnReader);
};

}  

#endif  
