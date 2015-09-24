// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_RECT_H_
#define PPAPI_CPP_RECT_H_

#include "ppapi/c/pp_rect.h"
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/size.h"


namespace pp {

class Rect {
 public:

  
  
  Rect() {
    rect_.point.x = 0;
    rect_.point.y = 0;
    rect_.size.width = 0;
    rect_.size.height = 0;
  }

  
  
  
  
  
  Rect(const PP_Rect& rect) {  
    set_x(rect.point.x);
    set_y(rect.point.y);
    set_width(rect.size.width);
    set_height(rect.size.height);
  }

  
  
  
  
  
  
  Rect(int32_t w, int32_t h) {
    set_x(0);
    set_y(0);
    set_width(w);
    set_height(h);
  }

  
  
  
  
  
  
  
  
  Rect(int32_t x, int32_t y, int32_t w, int32_t h) {
    set_x(x);
    set_y(y);
    set_width(w);
    set_height(h);
  }

  
  
  
  
  
  explicit Rect(const Size& s) {
    set_x(0);
    set_y(0);
    set_size(s);
  }

  
  
  
  
  
  
  
  
  Rect(const Point& origin, const Size& size) {
    set_point(origin);
    set_size(size);
  }

  
  ~Rect() {
  }

  
  
  
  
  operator PP_Rect() const {
    return rect_;
  }

  
  
  
  const PP_Rect& pp_rect() const {
    return rect_;
  }

  
  
  
  PP_Rect& pp_rect() {
    return rect_;
  }


  
  
  
  int32_t x() const {
    return rect_.point.x;
  }

  
  
  
  void set_x(int32_t in_x) {
    rect_.point.x = in_x;
  }

  
  
  
  int32_t y() const {
    return rect_.point.y;
  }

  
  
  
  void set_y(int32_t in_y) {
    rect_.point.y = in_y;
  }

  
  
  
  int32_t width() const {
    return rect_.size.width;
  }

  
  
  
  void set_width(int32_t w) {
    if (w < 0) {
      PP_DCHECK(w >= 0);
      w = 0;
    }
    rect_.size.width = w;
  }

  
  
  
  int32_t height() const {
    return rect_.size.height;
  }

  
  
  
  void set_height(int32_t h) {
    if (h < 0) {
      PP_DCHECK(h >= 0);
      h = 0;
    }
    rect_.size.height = h;
  }

  
  
  
  Point point() const {
    return Point(rect_.point);
  }

  
  
  
  
  void set_point(const Point& origin) {
    rect_.point = origin;
  }

  
  
  
  Size size() const {
    return Size(rect_.size);
  }

  
  
  
  
  void set_size(const Size& s) {
    rect_.size.width = s.width();
    rect_.size.height = s.height();
  }

  
  
  
  
  
  
  
  int32_t right() const {
    return x() + width();
  }

  
  
  
  
  
  
  
  int32_t bottom() const {
    return y() + height();
  }

  
  
  
  
  
  
  void SetRect(int32_t x, int32_t y, int32_t w, int32_t h) {
    set_x(x);
    set_y(y);
    set_width(w);
    set_height(h);
  }

  
  
  
  void SetRect(const PP_Rect& rect) {
    rect_ = rect;
  }

  
  
  
  
  
  
  
  void Inset(int32_t horizontal, int32_t vertical) {
    Inset(horizontal, vertical, horizontal, vertical);
  }

  
  
  
  
  
  
  
  
  
  
  
  void Inset(int32_t left, int32_t top, int32_t right, int32_t bottom);

  
  
  
  
  
  
  void Offset(int32_t horizontal, int32_t vertical);

  
  
  
  
  void Offset(const Point& point) {
    Offset(point.x(), point.y());
  }

  
  
  
  
  bool IsEmpty() const {
    return rect_.size.width == 0 && rect_.size.height == 0;
  }

  
  
  
  
  
  
  
  
  bool Contains(int32_t point_x, int32_t point_y) const;

  
  
  
  
  
  
  bool Contains(const Point& point) const {
    return Contains(point.x(), point.y());
  }

  
  
  
  
  
  bool Contains(const Rect& rect) const;

  
  
  
  
  
  
  bool Intersects(const Rect& rect) const;

  
  
  
  
  
  
  Rect Intersect(const Rect& rect) const;

  
  
  
  
  
  
  Rect Union(const Rect& rect) const;

  
  
  
  
  
  
  
  
  
  Rect Subtract(const Rect& rect) const;

  
  
  
  
  
  
  
  
  
  
  Rect AdjustToFit(const Rect& rect) const;

  
  
  
  Point CenterPoint() const;

  
  
  
  
  
  
  
  bool SharesEdgeWith(const Rect& rect) const;

 private:
  PP_Rect rect_;
};

}  

inline bool operator==(const pp::Rect& lhs, const pp::Rect& rhs) {
  return lhs.x() == rhs.x() &&
         lhs.y() == rhs.y() &&
         lhs.width() == rhs.width() &&
         lhs.height() == rhs.height();
}

inline bool operator!=(const pp::Rect& lhs, const pp::Rect& rhs) {
  return !(lhs == rhs);
}

#endif

