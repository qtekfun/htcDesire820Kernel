// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINT_SETTINGS_H_
#define PRINTING_PRINT_SETTINGS_H_

#include <string>

#include "base/strings/string16.h"
#include "printing/page_range.h"
#include "printing/page_setup.h"
#include "printing/print_job_constants.h"
#include "printing/printing_export.h"
#include "ui/gfx/rect.h"

namespace printing {

PRINTING_EXPORT bool IsColorModelSelected(int color_mode);

#if defined(USE_CUPS)
PRINTING_EXPORT void GetColorModelForMode(int color_mode,
                                          std::string* color_setting_name,
                                          std::string* color_value);
#endif

class PRINTING_EXPORT PrintSettings {
 public:
  PrintSettings();
  ~PrintSettings();

  
  void Clear();

  void SetCustomMargins(const PageMargins& requested_margins_in_points);
  void set_margin_type(MarginType margin_type) { margin_type_ = margin_type; }
  MarginType margin_type() const { return margin_type_; }

  
  void SetOrientation(bool landscape);
  bool landscape() const { return landscape_; }

  
  
  
  void SetPrinterPrintableArea(const gfx::Size& physical_size_device_units,
                               const gfx::Rect& printable_area_device_units,
                               bool landscape_needs_flip);
  const PageSetup& page_setup_device_units() const {
    return page_setup_device_units_;
  }

  void set_device_name(const base::string16& device_name) {
    device_name_ = device_name;
  }
  const base::string16& device_name() const { return device_name_; }

  void set_dpi(int dpi) { dpi_ = dpi; }
  int dpi() const { return dpi_; }

  void set_supports_alpha_blend(bool supports_alpha_blend) {
    supports_alpha_blend_ = supports_alpha_blend;
  }
  bool supports_alpha_blend() const { return supports_alpha_blend_; }

  int device_units_per_inch() const {
#if defined(OS_MACOSX)
    return 72;
#else  
    return dpi();
#endif  
  }

  void set_ranges(const PageRanges& ranges) { ranges_ = ranges; };
  const PageRanges& ranges() const { return ranges_; };

  void set_selection_only(bool selection_only) {
    selection_only_ = selection_only;
  }
  bool selection_only() const { return selection_only_; }

  void set_should_print_backgrounds(bool should_print_backgrounds) {
    should_print_backgrounds_ = should_print_backgrounds;
  }
  bool should_print_backgrounds() const { return should_print_backgrounds_; }

  void set_display_header_footer(bool display_header_footer) {
    display_header_footer_ = display_header_footer;
  }
  bool display_header_footer() const { return display_header_footer_; }

  void set_title(const base::string16& title) { title_ = title; }
  const base::string16& title() const { return title_; }

  void set_url(const base::string16& url) { url_ = url; }
  const base::string16& url() const { return url_; }

  void set_collate(bool collate) { collate_ = collate; }
  bool collate() const { return collate_; }

  void set_color(ColorModel color) { color_ = color; }
  ColorModel color() const { return color_; }

  void set_copies(int copies) { copies_ = copies; }
  int copies() const { return copies_; }

  void set_duplex_mode(DuplexMode duplex_mode) { duplex_mode_ = duplex_mode; }
  DuplexMode duplex_mode() const { return duplex_mode_; }

  int desired_dpi() const { return desired_dpi_; }

  double max_shrink() const { return max_shrink_; }

  double min_shrink() const { return min_shrink_; }

  
  
  
  static int NewCookie();

 private:
  
  
  PageRanges ranges_;

  
  
  
  
  
  double min_shrink_;

  
  
  
  
  double max_shrink_;

  
  
  int desired_dpi_;

  
  bool selection_only_;

  
  MarginType margin_type_;

  
  base::string16 title_;
  base::string16 url_;

  
  bool display_header_footer_;

  
  bool should_print_backgrounds_;

  
  bool collate_;

  
  ColorModel color_;

  
  int copies_;

  
  DuplexMode duplex_mode_;

  
  base::string16 device_name_;

  
  PageSetup page_setup_device_units_;

  
  int dpi_;

  
  bool landscape_;

  
  bool supports_alpha_blend_;

  
  PageMargins requested_custom_margins_in_points_;
};

}  

#endif  
