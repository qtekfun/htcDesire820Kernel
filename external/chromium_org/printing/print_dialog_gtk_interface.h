// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINT_DIALOG_GTK_INTERFACE_H_
#define PRINTING_PRINT_DIALOG_GTK_INTERFACE_H_

#include "base/strings/string16.h"
#include "printing/printing_context_gtk.h"
#include "ui/gfx/native_widget_types.h"

namespace printing {

class Metafile;
class PrintSettings;

class PrintDialogGtkInterface {
 public:
  
  virtual void UseDefaultSettings() = 0;

  
  
  virtual bool UpdateSettings(PrintSettings* settings) = 0;

  
  
  virtual void ShowDialog(
      gfx::NativeView parent_view,
      bool has_selection,
      const PrintingContextGtk::PrintSettingsCallback& callback) = 0;

  
  
  
  virtual void PrintDocument(const Metafile* metafile,
                             const base::string16& document_name) = 0;

  
  
  virtual void AddRefToDialog() = 0;
  virtual void ReleaseDialog() = 0;

 protected:
  virtual ~PrintDialogGtkInterface() {}
};

}  

#endif  
