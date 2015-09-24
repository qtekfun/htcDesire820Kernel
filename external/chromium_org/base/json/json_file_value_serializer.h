// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_FILE_VALUE_SERIALIZER_H_
#define BASE_JSON_JSON_FILE_VALUE_SERIALIZER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/values.h"

class BASE_EXPORT JSONFileValueSerializer : public base::ValueSerializer {
 public:
  
  
  
  
  explicit JSONFileValueSerializer(const base::FilePath& json_file_path)
    : json_file_path_(json_file_path),
      allow_trailing_comma_(false) {}

  virtual ~JSONFileValueSerializer() {}

  // DO NOT USE except in unit tests to verify the file was written properly.
  
  
  
  
  
  // JSON.  If the return value is true, the result will have been written
  
  virtual bool Serialize(const base::Value& root) OVERRIDE;

  
  
  bool SerializeAndOmitBinaryValues(const base::Value& root);

  
  
  
  
  
  
  
  virtual base::Value* Deserialize(int* error_code,
                                   std::string* error_message) OVERRIDE;

  
  enum JsonFileError {
    JSON_NO_ERROR = 0,
    JSON_ACCESS_DENIED = 1000,
    JSON_CANNOT_READ_FILE,
    JSON_FILE_LOCKED,
    JSON_NO_SUCH_FILE
  };

  
  static const char* kAccessDenied;
  static const char* kCannotReadFile;
  static const char* kFileLocked;
  static const char* kNoSuchFile;

  
  
  static const char* GetErrorMessageForCode(int error_code);

  void set_allow_trailing_comma(bool new_value) {
    allow_trailing_comma_ = new_value;
  }

 private:
  bool SerializeInternal(const base::Value& root, bool omit_binary_values);

  base::FilePath json_file_path_;
  bool allow_trailing_comma_;

  
  
  int ReadFileToString(std::string* json_string);

  DISALLOW_IMPLICIT_CONSTRUCTORS(JSONFileValueSerializer);
};

#endif  

