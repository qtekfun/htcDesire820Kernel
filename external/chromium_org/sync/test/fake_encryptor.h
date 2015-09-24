// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_FAKE_ENCRYPTOR_H_
#define SYNC_TEST_FAKE_ENCRYPTOR_H_

#include "base/compiler_specific.h"
#include "sync/util/encryptor.h"

namespace syncer {

class FakeEncryptor : public Encryptor {
 public:
  virtual ~FakeEncryptor();

  virtual bool EncryptString(const std::string& plaintext,
                             std::string* ciphertext) OVERRIDE;

  virtual bool DecryptString(const std::string& ciphertext,
                             std::string* plaintext) OVERRIDE;
};

}  

#endif  
