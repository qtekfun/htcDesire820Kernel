// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_CHROMEOS_EBML_WRITER_H_
#define MEDIA_WEBM_CHROMEOS_EBML_WRITER_H_

#include "base/callback.h"

struct EbmlGlobal {
  EbmlGlobal();
  ~EbmlGlobal();

  base::Callback<void(const void* buffer, unsigned long len)> write_cb;
  base::Callback<void(const void* buffer, int buffer_size, unsigned long len)>
      serialize_cb;
};

#endif  
