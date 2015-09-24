// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SIGNATURE_UTIL_H_
#define CHROME_BROWSER_SAFE_BROWSING_SIGNATURE_UTIL_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace base {
class FilePath;
}

namespace safe_browsing {
class ClientDownloadRequest_SignatureInfo;

class SignatureUtil : public base::RefCountedThreadSafe<SignatureUtil> {
 public:
  SignatureUtil();

  
  
  virtual void CheckSignature(
      const base::FilePath& file_path,
      ClientDownloadRequest_SignatureInfo* signature_info);

 protected:
  friend class base::RefCountedThreadSafe<SignatureUtil>;
  virtual ~SignatureUtil();

 private:
  DISALLOW_COPY_AND_ASSIGN(SignatureUtil);
};
}  

#endif  
