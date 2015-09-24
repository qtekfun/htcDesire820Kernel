// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_POINT_H_
#define PPAPI_CPP_POINT_H_

#include "ppapi/c/pp_point.h"


namespace pp {

class Point {
 public:
  
  Point() {
    point_.x = 0;
    point_.y = 0;
  }

  
  
  
  
  
  
  
  Point(int32_t in_x, int32_t in_y) {
    point_.x = in_x;
    point_.y = in_y;
  }

  
  
  
  
  Point(const PP_Point& point) {  
    point_.x = point.x;
    point_.y = point.y;
  }

  
  ~Point() {
  }

  
  
  operator PP_Point() const {
    return point_;
  }

  
  
  
  const PP_Point& pp_point() const {
    return point_;
  }

  
  
  
  PP_Point& pp_point() {
    return point_;
  }

  
  
  
  int32_t x() const { return point_.x; }

  
  
  
  void set_x(int32_t in_x) {
    point_.x = in_x;
  }

  
  
  
  int32_t y() const { return point_.y; }

  
  
  
  void set_y(int32_t in_y) {
    point_.y = in_y;
  }

  
  
  
  
  
  
  Point operator+(const Point& other) const {
    return Point(x() + other.x(), y() + other.y());
  }

  
  
  
  
  
  
  Point operator-(const Point& other) const {
    return Point(x() - other.x(), y() - other.y());
  }

  
  
  
  
  
  
  Point& operator+=(const Point& other) {
    point_.x += other.x();
    point_.y += other.y();
    return *this;
  }

  
  
  
  
  
  
  Point& operator-=(const Point& other) {
    point_.x -= other.x();
    point_.y -= other.y();
    return *this;
  }

  
  
  
  void swap(Point& other) {
    int32_t x = point_.x;
    int32_t y = point_.y;
    point_.x = other.point_.x;
    point_.y = other.point_.y;
    other.point_.x = x;
    other.point_.y = y;
  }

 private:
  PP_Point point_;
};

class FloatPoint {
 public:
  
  FloatPoint() {
    float_point_.x = 0.0f;
    float_point_.y = 0.0f;
  }

  
  
  
  
  
  
  
  
  FloatPoint(float in_x, float in_y) {
    float_point_.x = in_x;
    float_point_.y = in_y;
  }

  
  
  
  
  FloatPoint(const PP_FloatPoint& point) {  
    float_point_.x = point.x;
    float_point_.y = point.y;
  }
  
  ~FloatPoint() {
  }

  
  
  operator PP_FloatPoint() const {
    return float_point_;
  }

  
  
  
  const PP_FloatPoint& pp_float_point() const {
    return float_point_;
  }

  
  
  
  PP_FloatPoint& pp_float_point() {
    return float_point_;
  }

  
  
  
  float x() const { return float_point_.x; }

  
  
  
  void set_x(float in_x) {
    float_point_.x = in_x;
  }

  
  
  
  float y() const { return float_point_.y; }

  
  
  
  void set_y(float in_y) {
    float_point_.y = in_y;
  }

  
  
  
  
  
  
  FloatPoint operator+(const FloatPoint& other) const {
    return FloatPoint(x() + other.x(), y() + other.y());
  }

  
  
  
  
  
  
  FloatPoint operator-(const FloatPoint& other) const {
    return FloatPoint(x() - other.x(), y() - other.y());
  }

  
  
  
  
  
  
  FloatPoint& operator+=(const FloatPoint& other) {
    float_point_.x += other.x();
    float_point_.y += other.y();
    return *this;
  }

  
  
  
  
  
  
  FloatPoint& operator-=(const FloatPoint& other) {
    float_point_.x -= other.x();
    float_point_.y -= other.y();
    return *this;
  }

  
  
  
  void swap(FloatPoint& other) {
    float x = float_point_.x;
    float y = float_point_.y;
    float_point_.x = other.float_point_.x;
    float_point_.y = other.float_point_.y;
    other.float_point_.x = x;
    other.float_point_.y = y;
  }

 private:
  PP_FloatPoint float_point_;
};

}  

inline bool operator==(const pp::Point& lhs, const pp::Point& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

inline bool operator!=(const pp::Point& lhs, const pp::Point& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const pp::FloatPoint& lhs, const pp::FloatPoint& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

inline bool operator!=(const pp::FloatPoint& lhs, const pp::FloatPoint& rhs) {
  return !(lhs == rhs);
}

#endif  
