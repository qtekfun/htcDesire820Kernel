// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_SERVICE_HELPERS_H_
#define CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_SERVICE_HELPERS_H_

#include <string>
#include <vector>

#include "chrome/service/cloud_print/print_system.h"
#include "url/gurl.h"

namespace cloud_print {

GURL GetUrlForJobStatusUpdate(const GURL& cloud_print_server_url,
                              const std::string& job_id,
                              PrintJobStatus status,
                              int connector_code);

GURL GetUrlForJobStatusUpdate(const GURL& cloud_print_server_url,
                              const std::string& job_id,
                              const PrintJobDetails& details);

std::string GetHashOfPrinterInfo(
    const printing::PrinterBasicInfo& printer_info);

std::string GetPostDataForPrinterInfo(
    const printing::PrinterBasicInfo& printer_info,
    const std::string& mime_boundary);

bool IsDryRunJob(const std::vector<std::string>& tags);

std::string GetCloudPrintAuthHeaderFromStore();

}  

#endif  
