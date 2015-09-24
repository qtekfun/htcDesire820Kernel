// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_IME_IME_POPUP_OBSERVER_H_
#define WIN8_METRO_DRIVER_IME_IME_POPUP_OBSERVER_H_

namespace metro_driver {

class ImePopupObserver {
 public:
  enum EventType {
    kPopupShown,
    kPopupHidden,
    kPopupUpdated,
  };
  virtual ~ImePopupObserver() {}

  
  virtual void OnImePopupChanged(EventType type) = 0;
};

}  

#endif  
