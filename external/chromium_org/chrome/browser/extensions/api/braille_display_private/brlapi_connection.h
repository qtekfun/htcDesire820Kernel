// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRLAPI_CONNECTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRLAPI_CONNECTION_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "library_loaders/libbrlapi.h"

namespace extensions {
namespace api {
namespace braille_display_private {

class BrlapiConnection {
 public:
  typedef base::Closure OnDataReadyCallback;

  enum ConnectResult {
    CONNECT_ERROR_RETRY,
    CONNECT_ERROR_NO_RETRY,
    CONNECT_SUCCESS,
  };

  static scoped_ptr<BrlapiConnection> Create(LibBrlapiLoader* loader);

  virtual ~BrlapiConnection();

  virtual ConnectResult Connect(const OnDataReadyCallback& onDataReady) = 0;

  virtual void Disconnect() = 0;

  virtual bool Connected() = 0;

  
  
  
  
  
  virtual brlapi_error_t* BrlapiError() = 0;

  
  
  virtual std::string BrlapiStrError() = 0;

  
  
  
  virtual bool GetDisplaySize(size_t* size) = 0;

  
  
  virtual bool WriteDots(const unsigned char* cells) = 0;

  
  
  
  
  virtual int ReadKey(brlapi_keyCode_t* keyCode) = 0;

 protected:
  BrlapiConnection();
  DISALLOW_COPY_AND_ASSIGN(BrlapiConnection);
};

}  
}  
}  

#endif  
