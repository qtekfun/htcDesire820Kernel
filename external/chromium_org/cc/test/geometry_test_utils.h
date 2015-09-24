// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_GEOMETRY_TEST_UTILS_H_
#define CC_TEST_GEOMETRY_TEST_UTILS_H_

namespace gfx {
class Transform;
}

namespace cc {

#define EXPECT_FLOAT_RECT_EQ(expected, actual) \
do { \
  EXPECT_FLOAT_EQ((expected).x(), (actual).x()); \
  EXPECT_FLOAT_EQ((expected).y(), (actual).y()); \
  EXPECT_FLOAT_EQ((expected).width(), (actual).width()); \
  EXPECT_FLOAT_EQ((expected).height(), (actual).height()); \
} while (false)

#define EXPECT_RECT_NEAR(expected, actual, abs_error) \
do { \
  EXPECT_NEAR((expected).x(), (actual).x(), (abs_error)); \
  EXPECT_NEAR((expected).y(), (actual).y(), (abs_error)); \
  EXPECT_NEAR((expected).width(), (actual).width(), (abs_error)); \
  EXPECT_NEAR((expected).height(), (actual).height(), (abs_error)); \
} while (false)

#define EXPECT_RECT_EQ(expected, actual) \
do { \
  EXPECT_EQ((expected).x(), (actual).x()); \
  EXPECT_EQ((expected).y(), (actual).y()); \
  EXPECT_EQ((expected).width(), (actual).width()); \
  EXPECT_EQ((expected).height(), (actual).height()); \
} while (false)

#define EXPECT_SIZE_EQ(expected, actual) \
do { \
  EXPECT_EQ((expected).width(), (actual).width()); \
  EXPECT_EQ((expected).height(), (actual).height()); \
} while (false)

#define EXPECT_POINT_EQ(expected, actual) \
do { \
  EXPECT_EQ((expected).x(), (actual).x()); \
  EXPECT_EQ((expected).y(), (actual).y()); \
} while (false)

#define EXPECT_POINT3F_EQ(expected, actual) \
do { \
  EXPECT_FLOAT_EQ((expected).x(), (actual).x()); \
  EXPECT_FLOAT_EQ((expected).y(), (actual).y()); \
  EXPECT_FLOAT_EQ((expected).z(), (actual).z()); \
} while (false)

#define EXPECT_VECTOR_EQ(expected, actual) \
do { \
  EXPECT_EQ((expected).x(), (actual).x()); \
  EXPECT_EQ((expected).y(), (actual).y()); \
} while (false)

#define EXPECT_VECTOR2DF_EQ(expected, actual) \
do { \
  EXPECT_FLOAT_EQ((expected).x(), (actual).x()); \
  EXPECT_FLOAT_EQ((expected).y(), (actual).y()); \
} while (false)

#define EXPECT_FLOAT_ARRAY_EQ(expected, actual, count) \
do { \
  for (int i = 0; i < count; i++) { \
    EXPECT_FLOAT_EQ((expected)[i], (actual)[i]); \
  } \
} while (false)

void ExpectTransformationMatrixEq(const gfx::Transform& expected,
                                  const gfx::Transform& actual);

#define EXPECT_TRANSFORMATION_MATRIX_EQ(expected, actual) \
  do {                                                    \
    SCOPED_TRACE("");                                     \
    ExpectTransformationMatrixEq(expected, actual);       \
  } while (false)

gfx::Transform Inverse(const gfx::Transform& transform);

}  

#endif  
