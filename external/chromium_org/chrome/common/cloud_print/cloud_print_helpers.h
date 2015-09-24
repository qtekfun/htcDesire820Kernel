// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CLOUD_PRINT_CLOUD_PRINT_HELPERS_H_
#define CHROME_COMMON_CLOUD_PRINT_CLOUD_PRINT_HELPERS_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"

class GURL;

namespace base {
class DictionaryValue;
}

namespace cloud_print {

typedef std::map<std::string, std::string> PrinterTags;

std::string AppendPathToUrl(const GURL& url, const std::string& path);

GURL GetUrlForSearch(const GURL& cloud_print_server_url);
GURL GetUrlForSubmit(const GURL& cloud_print_server_url);
GURL GetUrlForPrinterList(const GURL& cloud_print_server_url,
                          const std::string& proxy_id);
GURL GetUrlForPrinterRegistration(const GURL& cloud_print_server_url);
GURL GetUrlForPrinterUpdate(const GURL& cloud_print_server_url,
                            const std::string& printer_id);
GURL GetUrlForPrinterDelete(const GURL& cloud_print_server_url,
                            const std::string& printer_id,
                            const std::string& reason);
GURL GetUrlForJobFetch(const GURL& cloud_print_server_url,
                       const std::string& printer_id,
                       const std::string& reason);
GURL GetUrlForJobDelete(const GURL& cloud_print_server_url,
                        const std::string& job_id);
GURL GetUrlForJobStatusUpdate(const GURL& cloud_print_server_url,
                              const std::string& job_id,
                              const std::string& status_string,
                              int connector_code);
GURL GetUrlForUserMessage(const GURL& cloud_print_server_url,
                          const std::string& message_id);
GURL GetUrlForGetAuthCode(const GURL& cloud_print_server_url,
                          const std::string& oauth_client_id,
                          const std::string& proxy_id);

scoped_ptr<base::DictionaryValue> ParseResponseJSON(
    const std::string& response_data,
    bool* succeeded);

std::string GetMultipartMimeType(const std::string& mime_boundary);

void CreateMimeBoundaryForUpload(std::string *out);

std::string GetHashOfPrinterTags(const PrinterTags& printer_tags);

std::string GetPostDataForPrinterTags(
    const PrinterTags& printer_tags,
    const std::string& mime_boundary,
    const std::string& proxy_tag_prefix,
    const std::string& tags_hash_tag_name);

std::string GetCloudPrintAuthHeader(const std::string& auth_token);

}  

#endif  
