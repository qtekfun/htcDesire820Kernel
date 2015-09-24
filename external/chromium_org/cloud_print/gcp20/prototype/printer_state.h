// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_PRINTER_STATE_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_PRINTER_STATE_H_

#include <string>

#include "base/memory/linked_ptr.h"
#include "base/time/time.h"
#include "cloud_print/gcp20/prototype/local_settings.h"

namespace base {
class DictionaryValue;
class FilePath;
}  

struct PrinterState {
  enum RegistrationState {
    UNREGISTERED,
    REGISTRATION_STARTED,  
                           
    REGISTRATION_CLAIM_TOKEN_READY,  
                                     
    REGISTRATION_COMPLETING,  
                              
    REGISTRATION_ERROR,  
    REGISTERED,
  };

  enum ConfirmationState {
    CONFIRMATION_PENDING,
    CONFIRMATION_CONFIRMED,
    CONFIRMATION_DISCARDED,
    CONFIRMATION_TIMEOUT,
  };

  PrinterState();
  ~PrinterState();

  
  std::string user;
  std::string registration_token;
  std::string complete_invite_url;
  RegistrationState registration_state;
  ConfirmationState confirmation_state;

  
  std::string refresh_token;
  std::string device_id;
  std::string xmpp_jid;
  LocalSettings local_settings;
  linked_ptr<base::DictionaryValue> cdd;

  
  std::string access_token;
  base::Time access_token_update;

  
  std::string error_description;
};

namespace printer_state {

bool SaveToFile(const base::FilePath& path, const PrinterState& state);

bool LoadFromFile(const base::FilePath& path, PrinterState* state);

}  

#endif  

