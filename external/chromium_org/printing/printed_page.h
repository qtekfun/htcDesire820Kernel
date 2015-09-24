// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINTED_PAGE_H_
#define PRINTING_PRINTED_PAGE_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "printing/metafile.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

namespace printing {

class PRINTING_EXPORT PrintedPage
    : public base::RefCountedThreadSafe<PrintedPage> {
 public:
  PrintedPage(int page_number,
              Metafile* metafile,
              const gfx::Size& page_size,
              const gfx::Rect& page_content_rect,
              double shrink_factor);

  
  int page_number() const { return page_number_; }
  const Metafile* metafile() const;
  const gfx::Size& page_size() const { return page_size_; }
  const gfx::Rect& page_content_rect() const { return page_content_rect_; }
  double shrink_factor() const { return shrink_factor_; }

  
  
  void GetCenteredPageContentRect(const gfx::Size& paper_size,
                                  gfx::Rect* content_rect) const;

 private:
  friend class base::RefCountedThreadSafe<PrintedPage>;

  ~PrintedPage();

  
  const int page_number_;

  
  const scoped_ptr<Metafile> metafile_;

  
  
  const gfx::Size page_size_;

  
  const gfx::Rect page_content_rect_;

  
  double shrink_factor_;

  DISALLOW_COPY_AND_ASSIGN(PrintedPage);
};

}  

#endif  
