// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_LOADER_NACL_VALIDATION_QUERY_H_
#define COMPONENTS_NACL_LOADER_NACL_VALIDATION_QUERY_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "crypto/hmac.h"

struct NaClFileToken;
struct NaClValidationCache;
class NaClValidationDB;
class NaClValidationQuery;

class NaClValidationQueryContext {
 public:
  NaClValidationQueryContext(NaClValidationDB* db,
                             const std::string& profile_key,
                             const std::string& nacl_version);

  NaClValidationQuery* CreateQuery();

  bool ResolveFileToken(struct NaClFileToken* file_token, int32* fd,
                        std::string* path);

 private:
  NaClValidationDB* db_;

  
  std::string profile_key_;

  
  
  
  std::string nacl_version_;
};

class NaClValidationQuery {
 public:
  
  static const size_t kDigestLength = 32;

  NaClValidationQuery(NaClValidationDB* db, const std::string& profile_key);

  void AddData(const char* data, size_t length);
  void AddData(const unsigned char* data, size_t length);
  void AddData(const base::StringPiece& data);

  int QueryKnownToValidate();

  void SetKnownToValidate();

 private:
  enum QueryState {
    READY,
    GET_CALLED,
    SET_CALLED
  };

  
  
  
  
  
  
  void CompressBuffer();

  
  QueryState state_;

  crypto::HMAC hasher_;
  NaClValidationDB* db_;

  
  
  
  
  
  char buffer_[kDigestLength * 4];
  size_t buffer_length_;

  DISALLOW_COPY_AND_ASSIGN(NaClValidationQuery);
};

struct NaClValidationCache* CreateValidationCache(
    NaClValidationDB* db, const std::string& profile_key,
    const std::string& nacl_version);

#endif  
