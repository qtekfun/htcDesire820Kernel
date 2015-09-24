// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DBUS_PRINTER_SERVICE_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_DBUS_PRINTER_SERVICE_PROVIDER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/dbus/cros_dbus_service.h"
#include "dbus/exported_object.h"

namespace dbus {
class MethodCall;
class Response;
}

namespace chromeos {


class PrinterServiceProvider
    : public CrosDBusService::ServiceProviderInterface {
 public:
  PrinterServiceProvider();
  virtual ~PrinterServiceProvider();

  
  virtual void Start(
      scoped_refptr<dbus::ExportedObject> exported_object) OVERRIDE;

 protected:
  virtual void ShowCloudPrintHelp(const std::string& vendor,
                                  const std::string& product);

 private:
  
  
  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

  
  
  void PrinterAdded(dbus::MethodCall* method_call,
                    dbus::ExportedObject::ResponseSender response_sender);

  scoped_refptr<dbus::ExportedObject> exported_object_;
  base::WeakPtrFactory<PrinterServiceProvider> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PrinterServiceProvider);
};

}  

#endif  

