// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_CAPTURE_TRACKING_VIEW_
#define UI_VIEWS_TEST_CAPTURE_TRACKING_VIEW_

#include "base/compiler_specific.h"
#include "ui/views/view.h"

namespace views {
namespace test {

class CaptureTrackingView : public views::View {
 public:
  CaptureTrackingView();
  virtual ~CaptureTrackingView();

  
  bool got_press() const { return got_press_; }

  
  bool got_capture_lost() const { return got_capture_lost_; }

  void reset() { got_press_ = got_capture_lost_ = false; }

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;

 private:
  
  bool got_press_;
  bool got_capture_lost_;

  DISALLOW_COPY_AND_ASSIGN(CaptureTrackingView);
};

}  
}  

#endif  
