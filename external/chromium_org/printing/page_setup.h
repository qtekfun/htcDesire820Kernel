// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PAGE_SETUP_H_
#define PRINTING_PAGE_SETUP_H_

#include "printing/printing_export.h"
#include "ui/gfx/rect.h"

namespace printing {

class PRINTING_EXPORT PageMargins {
 public:
  PageMargins();

  void Clear();

  
  bool Equals(const PageMargins& rhs) const;

  
  int header;
  
  int footer;
  
  int left;
  int right;
  int top;
  int bottom;
};

class PRINTING_EXPORT PageSetup {
 public:
  PageSetup();
  ~PageSetup();

  void Clear();

  
  bool Equals(const PageSetup& rhs) const;

  void Init(const gfx::Size& physical_size, const gfx::Rect& printable_area,
            int text_height);

  
  void SetRequestedMargins(const PageMargins& requested_margins);

  
  void ForceRequestedMargins(const PageMargins& requested_margins);

  
  void FlipOrientation();

  const gfx::Size& physical_size() const { return physical_size_; }
  const gfx::Rect& overlay_area() const { return overlay_area_; }
  const gfx::Rect& content_area() const { return content_area_; }
  const gfx::Rect& printable_area() const { return printable_area_; }
  const PageMargins& effective_margins() const {
    return effective_margins_;
  }

 private:
  
  void SetRequestedMarginsAndCalculateSizes(
      const PageMargins& requested_margins);

  
  
  void CalculateSizesWithinRect(const gfx::Rect& bounds, int text_height);

  
  gfx::Size physical_size_;

  
  
  gfx::Rect printable_area_;

  
  gfx::Rect overlay_area_;

  
  gfx::Rect content_area_;

  
  PageMargins effective_margins_;

  
  PageMargins requested_margins_;

  
  bool forced_margins_;

  
  int text_height_;
};

}  

#endif  
