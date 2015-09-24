// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_BACKEND_CUPS_HELPER_H_
#define PRINTING_BACKEND_CUPS_HELPER_H_

#include <cups/cups.h>

#include <string>

#include "printing/printing_export.h"

class GURL;

namespace printing {

struct PrinterSemanticCapsAndDefaults;

class PRINTING_EXPORT HttpConnectionCUPS {
 public:
  HttpConnectionCUPS(const GURL& print_server_url,
                     http_encryption_t encryption);
  ~HttpConnectionCUPS();

  void SetBlocking(bool blocking);

  http_t* http();

 private:
  http_t* http_;
};

PRINTING_EXPORT bool ParsePpdCapabilities(
    const std::string& printer_name,
    const std::string& printer_capabilities,
    PrinterSemanticCapsAndDefaults* printer_info);

}  

#endif  
