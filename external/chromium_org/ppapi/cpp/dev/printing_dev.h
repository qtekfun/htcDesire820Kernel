// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_PRINTING_DEV_H_
#define PPAPI_CPP_DEV_PRINTING_DEV_H_

#include "ppapi/c/dev/ppp_printing_dev.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class Instance;

class Printing_Dev : public Resource {
 public:
  
  explicit Printing_Dev(Instance* instance);
  virtual ~Printing_Dev();

  
  
  virtual uint32_t QuerySupportedPrintOutputFormats() = 0;
  virtual int32_t PrintBegin(const PP_PrintSettings_Dev& print_settings) = 0;
  virtual Resource PrintPages(const PP_PrintPageNumberRange_Dev* page_ranges,
                              uint32_t page_range_count) = 0;
  virtual void PrintEnd() = 0;
  virtual bool IsPrintScalingDisabled() = 0;

  
  
  
  static bool IsAvailable();

  
  int32_t GetDefaultPrintSettings(
      const CompletionCallbackWithOutput<PP_PrintSettings_Dev>& callback) const;

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
