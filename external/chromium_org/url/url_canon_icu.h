// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_CANON_ICU_H_
#define URL_URL_CANON_ICU_H_


#include "base/compiler_specific.h"
#include "url/url_canon.h"
#include "url/url_export.h"

typedef struct UConverter UConverter;

namespace url_canon {

class URL_EXPORT ICUCharsetConverter : public CharsetConverter {
 public:
  
  
  
  ICUCharsetConverter(UConverter* converter);

  virtual ~ICUCharsetConverter();

  virtual void ConvertFromUTF16(const base::char16* input,
                                int input_len,
                                CanonOutput* output) OVERRIDE;

 private:
  
  UConverter* converter_;
};

}  

#endif  
