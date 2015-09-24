// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_RESOURCE_RESOURCE_HANDLE_H_
#define UI_BASE_RESOURCE_RESOURCE_HANDLE_H_

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "ui/base/layout.h"
#include "ui/base/ui_export.h"

namespace base {
class RefCountedStaticMemory;
}

namespace ui {

class UI_EXPORT ResourceHandle {
 public:
  
  enum TextEncodingType {
    BINARY,
    UTF8,
    UTF16
  };

  virtual ~ResourceHandle() {}

  
  virtual bool HasResource(uint16 resource_id) const = 0;

  
  
  
  virtual bool GetStringPiece(uint16 resource_id,
                              base::StringPiece* data) const = 0;

  
  
  virtual base::RefCountedStaticMemory* GetStaticMemory(
      uint16 resource_id) const = 0;

  
  virtual TextEncodingType GetTextEncodingType() const = 0;

  
  
  virtual ScaleFactor GetScaleFactor() const = 0;
};

}  

#endif  
