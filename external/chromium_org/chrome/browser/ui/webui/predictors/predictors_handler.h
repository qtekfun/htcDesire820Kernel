// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PREDICTORS_PREDICTORS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_PREDICTORS_PREDICTORS_HANDLER_H_

#include "base/compiler_specific.h"
#include "chrome/browser/predictors/resource_prefetch_predictor.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

namespace predictors {
class AutocompleteActionPredictor;
class ResourcePrefetchPredictor;
}

class Profile;

class PredictorsHandler : public content::WebUIMessageHandler {
 public:
  explicit PredictorsHandler(Profile* profile);
  virtual ~PredictorsHandler();

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  
  void RequestAutocompleteActionPredictorDb(const base::ListValue* args);

  
  
  void RequestResourcePrefetchPredictorDb(const base::ListValue* args);

  
  void AddPrefetchDataMapToListValue(
      const predictors::ResourcePrefetchPredictor::PrefetchDataMap& data_map,
      base::ListValue* db) const;

  predictors::AutocompleteActionPredictor* autocomplete_action_predictor_;
  predictors::ResourcePrefetchPredictor* resource_prefetch_predictor_;

  DISALLOW_COPY_AND_ASSIGN(PredictorsHandler);
};

#endif  
