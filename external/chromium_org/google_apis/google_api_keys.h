// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GOOGLE_API_KEYS_H_
#define GOOGLE_APIS_GOOGLE_API_KEYS_H_

#include <string>


namespace google_apis {

bool HasKeysConfigured();

std::string GetAPIKey();

enum OAuth2Client {
  CLIENT_MAIN,         
  CLIENT_CLOUD_PRINT,
  CLIENT_REMOTING,
  CLIENT_REMOTING_HOST,

  CLIENT_NUM_ITEMS     
};

std::string GetOAuth2ClientID(OAuth2Client client);

std::string GetOAuth2ClientSecret(OAuth2Client client);

}  

#endif  
