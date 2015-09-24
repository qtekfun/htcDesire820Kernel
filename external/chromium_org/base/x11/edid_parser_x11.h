// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_X11_EDID_PARSER_X11_H_
#define BASE_X11_EDID_PARSER_X11_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"

typedef unsigned long XID;


namespace base {

BASE_EXPORT bool GetEDIDProperty(XID output,
                                 unsigned long* nitems,
                                 unsigned char** prop);

BASE_EXPORT bool GetDisplayId(XID output, size_t index,
                              int64* display_id_out);

BASE_EXPORT bool GetDisplayIdFromEDID(const unsigned char* prop,
                                      unsigned long nitems,
                                      size_t index,
                                      int64* display_id_out);

BASE_EXPORT bool ParseOutputDeviceData(const unsigned char* prop,
                                       unsigned long nitems,
                                       uint16* manufacturer_id,
                                       std::string* human_readable_name);

}  

#endif  
