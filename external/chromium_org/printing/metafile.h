// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_METAFILE_H_
#define PRINTING_METAFILE_H_

#include "base/basictypes.h"
#include "build/build_config.h"
#include "printing/printing_export.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_MACOSX)
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include "base/mac/scoped_cftyperef.h"
#endif

namespace base {
class FilePath;
}

namespace gfx {
class Rect;
class Size;
}

class SkBaseDevice;

#if defined(OS_CHROMEOS) || defined(OS_ANDROID)
namespace base {
struct FileDescriptor;
}
#endif

namespace printing {

class PRINTING_EXPORT Metafile {
 public:
#if defined(OS_MACOSX)
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct MacRenderPageParams {
    MacRenderPageParams()
        : shrink_to_fit(false),
          stretch_to_fit(false),
          center_horizontally(false),
          center_vertically(false),
          autorotate(false) {
    }

    bool shrink_to_fit;
    bool stretch_to_fit;
    bool center_horizontally;
    bool center_vertically;
    bool autorotate;
  };
#endif  

  virtual ~Metafile() {}

  
  
  
  virtual bool Init() = 0;

  
  
  
  virtual bool InitFromData(const void* src_buffer, uint32 src_buffer_size) = 0;

  
  
  
  virtual SkBaseDevice* StartPageForVectorCanvas(
      const gfx::Size& page_size,
      const gfx::Rect& content_area,
      const float& scale_factor) = 0;

  
  
  
  virtual bool StartPage(const gfx::Size& page_size,
                         const gfx::Rect& content_area,
                         const float& scale_factor) = 0;

  
  
  
  virtual bool FinishPage() = 0;

  
  
  virtual bool FinishDocument() = 0;

  
  
  virtual uint32 GetDataSize() const = 0;

  
  
  
  virtual bool GetData(void* dst_buffer, uint32 dst_buffer_size) const = 0;

  
  
  virtual bool SaveTo(const base::FilePath& file_path) const = 0;

  
  virtual gfx::Rect GetPageBounds(unsigned int page_number) const = 0;
  virtual unsigned int GetPageCount() const = 0;

  
  virtual gfx::NativeDrawingContext context() const = 0;

#if defined(OS_WIN)
  
  
  
  
  
  
  
  
  virtual bool Playback(gfx::NativeDrawingContext hdc,
                        const RECT* rect) const = 0;

  
  
  
  
  virtual bool SafePlayback(gfx::NativeDrawingContext hdc) const = 0;

  virtual HENHMETAFILE emf() const = 0;
#elif defined(OS_MACOSX)
  
  
  
  virtual bool RenderPage(unsigned int page_number,
                          gfx::NativeDrawingContext context,
                          const CGRect rect,
                          const MacRenderPageParams& params) const = 0;
#elif defined(OS_CHROMEOS) || defined(OS_ANDROID)
  
  
  
  virtual bool SaveToFD(const base::FileDescriptor& fd) const = 0;
#endif  
};

}  

#endif  
