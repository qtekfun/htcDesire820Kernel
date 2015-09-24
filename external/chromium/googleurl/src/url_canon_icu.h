// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLEURL_SRC_URL_CANON_ICU_H__
#define GOOGLEURL_SRC_URL_CANON_ICU_H__

#include "googleurl/src/url_canon.h"

typedef struct UConverter UConverter;

namespace url_canon {

class ICUCharsetConverter : public CharsetConverter {
 public:
  
  
  
  GURL_API ICUCharsetConverter(UConverter* converter);

  GURL_API virtual ~ICUCharsetConverter() {}

  GURL_API virtual void ConvertFromUTF16(const char16* input,
                                         int input_len,
                                         CanonOutput* output);

 private:
  
  UConverter* converter_;
};

}  

#endif  
