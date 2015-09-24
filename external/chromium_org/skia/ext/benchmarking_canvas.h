// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_BENCHMARKING_CANVAS_H_
#define SKIA_EXT_BENCHMARKING_CANVAS_H_

#include "base/compiler_specific.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/utils/SkNWayCanvas.h"
#include "third_party/skia/src/utils/debugger/SkDebugCanvas.h"

namespace skia {

class TimingCanvas;

class SK_API BenchmarkingCanvas : public SkNWayCanvas {
public:
  BenchmarkingCanvas(int width, int height);
  virtual ~BenchmarkingCanvas();

  
  size_t CommandCount() const;

  
  SkDrawCommand* GetCommand(size_t index);

  
  double GetTime(size_t index);

private:
  
  
  
  
  
  
  
  
  
  
  
  

  skia::RefPtr<SkDebugCanvas> debug_canvas_;
  skia::RefPtr<TimingCanvas> timing_canvas_;
};

}
#endif 
