// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_PRINT_RESPONSE_PARSER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_PRINT_RESPONSE_PARSER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/time/time.h"
#include "cloud_print/gcp20/prototype/local_settings.h"

namespace base {

class DictionaryValue;

}  

namespace cloud_print_response_parser {

struct Job {
  Job();
  ~Job();

  std::string job_id;
  base::Time create_time;
  std::string file_url;
  std::string ticket_url;
  std::string title;

  
  std::string file;
  std::string ticket;
};

bool ParseRegisterStartResponse(const std::string& response,
                                std::string* error_description,
                                std::string* polling_url,
                                std::string* registration_token,
                                std::string* complete_invite_url,
                                std::string* device_id);

bool ParseRegisterCompleteResponse(const std::string& response,
                                   std::string* error_description,
                                   std::string* authorization_code,
                                   std::string* xmpp_jid);

bool ParseFetchResponse(const std::string& response,
                        std::string* error_description,
                        std::vector<Job>* list);

bool ParseLocalSettingsResponse(const std::string& response,
                                std::string* error_description,
                                LocalSettings::State* state,
                                LocalSettings* settings);

}  

#endif  

