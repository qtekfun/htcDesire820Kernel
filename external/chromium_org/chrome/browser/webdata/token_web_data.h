// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// information and MUST not be extracted, overwritten or modified except

#ifndef CHROME_BROWSER_WEBDATA_TOKEN_WEB_DATA_H__
#define CHROME_BROWSER_WEBDATA_TOKEN_WEB_DATA_H__

#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "components/webdata/common/web_data_results.h"
#include "components/webdata/common/web_data_service_base.h"
#include "components/webdata/common/web_data_service_consumer.h"
#include "components/webdata/common/web_database.h"

class TokenWebDataBackend;
class WebDatabaseService;
class WebDataServiceConsumer;

namespace content {
class BrowserContext;
}


class TokenWebData : public WebDataServiceBase {
 public:
  
  static scoped_refptr<TokenWebData> FromBrowserContext(
      content::BrowserContext* context);

  TokenWebData(scoped_refptr<WebDatabaseService> wdbs,
               const ProfileErrorCallback& callback);

  
  void SetTokenForService(const std::string& service,
                          const std::string& token);

  
  void RemoveAllTokens();

  
  void RemoveTokenForService(const std::string& service);

  
  virtual Handle GetAllTokens(WebDataServiceConsumer* consumer);

 protected:
  
  TokenWebData();

  virtual ~TokenWebData();

 private:
  scoped_refptr<TokenWebDataBackend> token_backend_;

  DISALLOW_COPY_AND_ASSIGN(TokenWebData);
};

#endif  
