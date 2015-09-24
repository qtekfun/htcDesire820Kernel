// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_MOCK_PRINTER_H_
#define CHROME_RENDERER_MOCK_PRINTER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "printing/image.h"
#include "third_party/WebKit/public/web/WebPrintScalingOption.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

struct PrintMsg_Print_Params;
struct PrintMsg_PrintPages_Params;
struct PrintHostMsg_DidPrintPage_Params;

class MockPrinterPage : public base::RefCounted<MockPrinterPage> {
 public:
  MockPrinterPage(const void* source_data,
                  uint32 source_size,
                  const printing::Image& image);

  int width() const { return image_.size().width(); }
  int height() const { return image_.size().height(); }
  const uint8* source_data() const { return source_data_.get(); }
  uint32 source_size() const { return source_size_; }
  const printing::Image& image() const { return image_; }

 private:
  friend class base::RefCounted<MockPrinterPage>;
  virtual ~MockPrinterPage();

  uint32 source_size_;
  scoped_ptr<uint8[]> source_data_;
  printing::Image image_;

  DISALLOW_COPY_AND_ASSIGN(MockPrinterPage);
};

class MockPrinter {
 public:
  enum Status {
    PRINTER_READY,
    PRINTER_PRINTING,
    PRINTER_ERROR,
  };

  MockPrinter();
  ~MockPrinter();

  
  void ResetPrinter();
  void SetDefaultPrintSettings(const PrintMsg_Print_Params& params);
  void UseInvalidSettings();
  void UseInvalidPageSize();
  void UseInvalidContentSize();

  
  void GetDefaultPrintSettings(PrintMsg_Print_Params* params);
  void ScriptedPrint(int cookie,
                     int expected_pages_count,
                     bool has_selection,
                     PrintMsg_PrintPages_Params* settings);
  void UpdateSettings(int cookie, PrintMsg_PrintPages_Params* params,
                      const std::vector<int>& page_range_array,
                      int margins_type);
  void SetPrintedPagesCount(int cookie, int number_pages);
  void PrintPage(const PrintHostMsg_DidPrintPage_Params& params);

  
  Status GetPrinterStatus() const { return printer_status_; }
  int GetPrintedPages() const;

  
  
  const MockPrinterPage* GetPrintedPage(unsigned int pageno) const;

  int GetWidth(unsigned int page) const;
  int GetHeight(unsigned int page) const;
  bool GetBitmapChecksum(unsigned int page, std::string* checksum) const;
  bool GetSource(unsigned int page, const void** data, uint32* size) const;
  bool GetBitmap(unsigned int page, const void** data, uint32* size) const;
  bool SaveSource(unsigned int page, const base::FilePath& filepath) const;
  bool SaveBitmap(unsigned int page, const base::FilePath& filepath) const;

 protected:
  int CreateDocumentCookie();

 private:
  
  void SetPrintParams(PrintMsg_Print_Params* params);

  
  gfx::Size page_size_;
  gfx::Size content_size_;
  int margin_left_;
  int margin_top_;
  gfx::Rect printable_area_;

  
  double dpi_;
  double max_shrink_;
  double min_shrink_;

  
  int desired_dpi_;

  
  bool selection_only_;

  
  bool should_print_backgrounds_;

  
  int document_cookie_;
  int current_document_cookie_;

  
  Status printer_status_;

  
  int number_pages_;
  int page_number_;

  
  bool is_first_request_;
  bool print_to_pdf_;
  int preview_request_id_;

  
  
  blink::WebPrintScalingOption print_scaling_option_;

  
  bool display_header_footer_;
  base::string16 title_;
  base::string16 url_;

  
  bool use_invalid_settings_;

  std::vector<scoped_refptr<MockPrinterPage> > pages_;

  DISALLOW_COPY_AND_ASSIGN(MockPrinter);
};

#endif  
