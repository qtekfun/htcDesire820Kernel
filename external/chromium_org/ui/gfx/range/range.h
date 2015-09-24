// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RANGE_RANGE_H_
#define UI_GFX_RANGE_RANGE_H_

#include <ostream>
#include <string>

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"

#if defined(OS_MACOSX)
#if __OBJC__
#import <Foundation/Foundation.h>
#else
typedef struct _NSRange NSRange;
#endif
#endif  

#if defined(OS_WIN)
#include <windows.h>
#include <richedit.h>
#endif

namespace gfx {

class GFX_EXPORT Range {
 public:
  
  Range();

  
  Range(size_t start, size_t end);

  
  explicit Range(size_t position);

  
#if defined(OS_MACOSX)
  explicit Range(const NSRange& range);
#elif defined(OS_WIN)
  
  
  Range(const CHARRANGE& range, LONG total_length = -1);
#endif

  
  static const Range InvalidRange();

  
  bool IsValid() const;

  
  size_t start() const { return start_; }
  void set_start(size_t start) { start_ = start; }

  size_t end() const { return end_; }
  void set_end(size_t end) { end_ = end; }

  
  size_t length() const {
    ptrdiff_t length = end() - start();
    return length >= 0 ? length : -length;
  }

  bool is_reversed() const { return start() > end(); }
  bool is_empty() const { return start() == end(); }

  
  size_t GetMin() const;
  size_t GetMax() const;

  bool operator==(const Range& other) const;
  bool operator!=(const Range& other) const;
  bool EqualsIgnoringDirection(const Range& other) const;

  
  bool Intersects(const Range& range) const;

  
  bool Contains(const Range& range) const;

  
  
  
  Range Intersect(const Range& range) const;

#if defined(OS_MACOSX)
  Range& operator=(const NSRange& range);

  
  
  NSRange ToNSRange() const;
#elif defined(OS_WIN)
  CHARRANGE ToCHARRANGE() const;
#endif
  

  std::string ToString() const;

 private:
  size_t start_;
  size_t end_;
};

GFX_EXPORT std::ostream& operator<<(std::ostream& os, const Range& range);

}  

#endif  
