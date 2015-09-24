// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BLUETOOTH_BLUETOOTH_EXTENSION_FUNCTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_BLUETOOTH_BLUETOOTH_EXTENSION_FUNCTION_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/chrome_extension_function.h"

namespace device {

class BluetoothAdapter;

}  

namespace extensions {

namespace api {

class BluetoothExtensionFunction : public ChromeAsyncExtensionFunction {
 public:
  BluetoothExtensionFunction();

 protected:
  virtual ~BluetoothExtensionFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void RunOnAdapterReady(scoped_refptr<device::BluetoothAdapter> adapter);

  
  
  virtual bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) = 0;

  DISALLOW_COPY_AND_ASSIGN(BluetoothExtensionFunction);
};

}  

}  

#endif  
