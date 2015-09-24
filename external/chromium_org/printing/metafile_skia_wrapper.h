// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_METAFILE_SKIA_WRAPPER_H_
#define PRINTING_METAFILE_SKIA_WRAPPER_H_

#include "printing/printing_export.h"
#include "third_party/skia/include/core/SkRefCnt.h"

class SkCanvas;

namespace printing {

class Metafile;

class PRINTING_EXPORT MetafileSkiaWrapper : public SkRefCnt {
 public:
  static void SetMetafileOnCanvas(const SkCanvas& canvas, Metafile* metafile);

  static Metafile* GetMetafileFromCanvas(const SkCanvas& canvas);

  
  static void SetCustomScaleOnCanvas(const SkCanvas& canvas, double scale);
  static bool GetCustomScaleOnCanvas(const SkCanvas& canvas, double* scale);

 private:
  explicit MetafileSkiaWrapper(Metafile* metafile);

  Metafile* metafile_;
};

}  

#endif  
