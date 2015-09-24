// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CLIENT_HINTS_H_
#define CHROME_BROWSER_NET_CLIENT_HINTS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"

class ClientHints {
 public:
  static const char kDevicePixelRatioHeader[];

  ClientHints();
  ~ClientHints();

  void Init();

  
  
  const std::string& GetDevicePixelRatioHeader() const;

 private:
  friend class ClientHintsTest;

  
  bool RetrieveScreenInfo();

  void UpdateScreenInfo(float device_pixel_ratio_value);

  std::string screen_hints_;

  base::WeakPtrFactory<ClientHints> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ClientHints);
};

#endif  
