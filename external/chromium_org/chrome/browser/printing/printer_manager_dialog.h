// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINTER_MANAGER_DIALOG_H_
#define CHROME_BROWSER_PRINTING_PRINTER_MANAGER_DIALOG_H_

#include "base/basictypes.h"

namespace printing {

class PrinterManagerDialog {
 public:
  
  static void ShowPrinterManagerDialog();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(PrinterManagerDialog);
};

}  

#endif  
