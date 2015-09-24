// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_CRX_FILE_H_
#define EXTENSIONS_COMMON_CRX_FILE_H_

#include <sys/types.h>
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace extensions {

class CrxFile {
 public:

  
  
  static const size_t kCrxFileHeaderMagicSize = 4;

  
  
  
  struct Header {
    char magic[kCrxFileHeaderMagicSize];
    uint32 version;
    uint32 key_size;  
    uint32 signature_size;  
    
    
  };

  enum Error {
    kWrongMagic,
    kInvalidVersion,
    kInvalidKeyTooLarge,
    kInvalidKeyTooSmall,
    kInvalidSignatureTooLarge,
    kInvalidSignatureTooSmall,
  };

  
  
  
  static scoped_ptr<CrxFile> Parse(const Header& header, Error* error);

  
  
  
  
  
  
  static scoped_ptr<CrxFile> Create(const uint32 key_size,
                                    const uint32 signature_size,
                                    Error* error);

  
  const Header& header() const { return header_; }

  
  
  static bool HeaderIsDelta(const Header& header);

 private:
  Header header_;

  
  explicit CrxFile(const Header& header);

  
  
  static bool HeaderIsValid(const Header& header, Error* error);
};

}  

#endif  
