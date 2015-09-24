// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_LOCAL_DISCOVERY_PWG_RASTER_CONVERTER_H_
#define CHROME_BROWSER_LOCAL_DISCOVERY_PWG_RASTER_CONVERTER_H_

#include "base/callback.h"
#include "base/memory/ref_counted_memory.h"

namespace base {
class FilePath;
}

namespace gfx {
class Size;
}

namespace printing {
class PdfRenderSettings;
}

namespace local_discovery {

class PWGRasterConverter {
 public:
  
  
  
  
  typedef base::Callback<void(bool ,
                              const base::FilePath& )>
          ResultCallback;
  virtual ~PWGRasterConverter() {}

  static scoped_ptr<PWGRasterConverter> CreateDefault();

  virtual void Start(base::RefCountedMemory* data,
                     const printing::PdfRenderSettings& conversion_settings,
                     const ResultCallback& callback) = 0;
};

}  

#endif  
