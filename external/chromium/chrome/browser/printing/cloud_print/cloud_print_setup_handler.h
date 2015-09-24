// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_SETUP_HANDLER_H_
#define CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_SETUP_HANDLER_H_
#pragma once

#include "base/memory/weak_ptr.h"
#include "chrome/browser/printing/cloud_print/cloud_print_setup_flow.h"

class CloudPrintSetupHandler
    : public CloudPrintSetupFlow::Delegate,
      public base::SupportsWeakPtr<CloudPrintSetupHandler> {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    
    virtual void OnCloudPrintSetupClosed() = 0;
  };

  explicit CloudPrintSetupHandler(Delegate* handler);
  virtual ~CloudPrintSetupHandler();

  
  virtual void OnDialogClosed();

 private:
  Delegate* handler_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintSetupHandler);
};

typedef CloudPrintSetupHandler::Delegate CloudPrintSetupHandlerDelegate;

#endif  
