// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TESTS_TEST_GRAPHICS_2D_H_
#define PPAPI_TESTS_TEST_GRAPHICS_2D_H_

#include <string>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/tests/test_case.h"

namespace pp {
class Graphics2D;
class ImageData;
class Point;
class Rect;
}

class TestGraphics2D : public TestCase {
 public:
  explicit TestGraphics2D(TestingInstance* instance);

  
  virtual void DidChangeView(const pp::View& view);
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

  void QuitMessageLoop();

 private:
  bool ReadImageData(const pp::Graphics2D& dc,
                     pp::ImageData* image,
                     const pp::Point& top_left) const;

  void FillRectInImage(pp::ImageData* image,
                       const pp::Rect& rect,
                       uint32_t color) const;

  
  void FillImageWithGradient(pp::ImageData* image) const;

  
  bool CompareImages(const pp::ImageData& image1,
                     const pp::ImageData& image2);

  
  bool CompareImageRect(const pp::ImageData& image1,
                        const pp::Rect& rc1,
                        const pp::ImageData& image2,
                        const pp::Rect& rc2) const;

  
  
  bool IsSquareInImage(const pp::ImageData& image_data,
                       uint32_t background_color,
                       const pp::Rect& square, uint32_t square_color) const;

  
  
  bool IsSquareInDC(const pp::Graphics2D& dc, uint32_t background_color,
                    const pp::Rect& square, uint32_t square_color) const;

  
  bool IsDCUniformColor(const pp::Graphics2D& dc, uint32_t color) const;

  
  
  
  std::string FlushAndWaitForDone(pp::Graphics2D* context);

  
  
  
  
  
  
  PP_Resource ReplaceContentsAndReturnID(pp::Graphics2D* dc,
                                         const pp::Size& size);

  
  void ResetViewChangedState();

  
  
  
  bool WaitUntilViewChanged();

  std::string TestInvalidResource();
  std::string TestInvalidSize();
  std::string TestHumongous();
  std::string TestInitToZero();
  std::string TestDescribe();
  std::string TestScale();
  std::string TestPaint();
  std::string TestScroll();
  std::string TestReplace();
  std::string TestFlush();
  std::string TestFlushOffscreenUpdate();
  std::string TestDev();
  std::string TestReplaceContentsCaching();
  std::string TestBindNull();

  
  const PPB_Graphics2D_1_1* graphics_2d_interface_;
  const PPB_ImageData_1_0* image_data_interface_;

  
  
  bool is_view_changed_;

  
  
  
  bool post_quit_on_view_changed_;
};

#endif  
