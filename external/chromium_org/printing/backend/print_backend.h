// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_BACKEND_PRINT_BACKEND_H_
#define PRINTING_BACKEND_PRINT_BACKEND_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "printing/print_job_constants.h"
#include "printing/printing_export.h"

namespace base {
class DictionaryValue;
}

namespace printing {

struct PRINTING_EXPORT PrinterBasicInfo {
  PrinterBasicInfo();
  ~PrinterBasicInfo();

  std::string printer_name;
  std::string printer_description;
  int printer_status;
  int is_default;
  std::map<std::string, std::string> options;
};

typedef std::vector<PrinterBasicInfo> PrinterList;

struct PRINTING_EXPORT PrinterSemanticCapsAndDefaults {
  PrinterSemanticCapsAndDefaults();
  ~PrinterSemanticCapsAndDefaults();

  
  bool color_changeable;
  bool duplex_capable;

#if defined(USE_CUPS)
  ColorModel color_model;
  ColorModel bw_model;
#endif

  
  bool color_default;
  DuplexMode duplex_default;
};

struct PRINTING_EXPORT PrinterCapsAndDefaults {
  PrinterCapsAndDefaults();
  ~PrinterCapsAndDefaults();

  std::string printer_capabilities;
  std::string caps_mime_type;
  std::string printer_defaults;
  std::string defaults_mime_type;
};

class PRINTING_EXPORT PrintBackend
    : public base::RefCountedThreadSafe<PrintBackend> {
 public:
  
  virtual bool EnumeratePrinters(PrinterList* printer_list) = 0;

  
  virtual std::string GetDefaultPrinterName() = 0;

  
  
  
  
  virtual bool GetPrinterSemanticCapsAndDefaults(
      const std::string& printer_name,
      PrinterSemanticCapsAndDefaults* printer_info) = 0;

  
  virtual bool GetPrinterCapsAndDefaults(
      const std::string& printer_name,
      PrinterCapsAndDefaults* printer_info) = 0;

  
  virtual std::string GetPrinterDriverInfo(
      const std::string& printer_name) = 0;

  
  virtual bool IsValidPrinter(const std::string& printer_name) = 0;

  
  
  
  static scoped_refptr<PrintBackend> CreateInstance(
      const base::DictionaryValue* print_backend_settings);

 protected:
  friend class base::RefCountedThreadSafe<PrintBackend>;
  virtual ~PrintBackend();
};

}  

#endif  
