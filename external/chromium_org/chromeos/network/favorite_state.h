// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_FAVORITE_STATE_H_
#define CHROMEOS_NETWORK_FAVORITE_STATE_H_

#include <string>

#include "base/values.h"
#include "chromeos/network/managed_state.h"
#include "chromeos/network/network_ui_data.h"
#include "components/onc/onc_constants.h"

namespace chromeos {

class CHROMEOS_EXPORT FavoriteState : public ManagedState {
 public:
  explicit FavoriteState(const std::string& path);
  virtual ~FavoriteState();

  
  virtual bool PropertyChanged(const std::string& key,
                               const base::Value& value) OVERRIDE;

  
  const std::string& profile_path() const { return profile_path_; }
  bool is_favorite() const { return !profile_path_.empty(); }
  const NetworkUIData& ui_data() const { return ui_data_; }
  const base::DictionaryValue& proxy_config() const { return proxy_config_; }
  const std::string& guid() const { return guid_; }

  
  bool IsPrivate() const;

 private:
  std::string profile_path_;
  NetworkUIData ui_data_;
  std::string guid_;

  
  
  base::DictionaryValue proxy_config_;

  DISALLOW_COPY_AND_ASSIGN(FavoriteState);
};

}  

#endif  
