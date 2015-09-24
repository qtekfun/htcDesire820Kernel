// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_METRICS_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_METRICS_H_
#pragma once

#include <stddef.h>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/autofill/field_types.h"

class AutofillMetrics {
 public:
  enum CreditCardInfoBarMetric {
    CREDIT_CARD_INFOBAR_SHOWN = 0,  
                                    
    CREDIT_CARD_INFOBAR_ACCEPTED,   
    CREDIT_CARD_INFOBAR_DENIED,     
    CREDIT_CARD_INFOBAR_IGNORED,    
                                    
    NUM_CREDIT_CARD_INFO_BAR_METRICS
  };

  
  
  enum HeuristicTypeQualityMetric {
    HEURISTIC_TYPE_UNKNOWN = 0,  
    HEURISTIC_TYPE_MATCH,        
    HEURISTIC_TYPE_MISMATCH,     
    NUM_HEURISTIC_TYPE_QUALITY_METRICS
  };
  enum ServerTypeQualityMetric {
    SERVER_TYPE_UNKNOWN = 0,  
    SERVER_TYPE_MATCH,        
    SERVER_TYPE_MISMATCH,     
    NUM_SERVER_TYPE_QUALITY_METRICS
  };
  enum PredictedTypeQualityMetric {
    PREDICTED_TYPE_UNKNOWN = 0,  
                                 
    PREDICTED_TYPE_MATCH,        
    PREDICTED_TYPE_MISMATCH,     
    NUM_PREDICTED_TYPE_QUALITY_METRICS
  };

  enum QualityMetric {
    
    FIELD_SUBMITTED = 0,

    
    
    FIELD_AUTOFILLED,

    
    
    
    FIELD_NOT_AUTOFILLED,

    
    NOT_AUTOFILLED_HEURISTIC_TYPE_UNKNOWN,
    NOT_AUTOFILLED_HEURISTIC_TYPE_MATCH,
    NOT_AUTOFILLED_HEURISTIC_TYPE_MISMATCH,
    NOT_AUTOFILLED_SERVER_TYPE_UNKNOWN,
    NOT_AUTOFILLED_SERVER_TYPE_MATCH,
    NOT_AUTOFILLED_SERVER_TYPE_MISMATCH,
    NUM_QUALITY_METRICS
  };

  
  
  enum ServerQueryMetric {
    QUERY_SENT = 0,           
    QUERY_RESPONSE_RECEIVED,  
    QUERY_RESPONSE_PARSED,    

    
    
    QUERY_RESPONSE_MATCHED_LOCAL_HEURISTICS,

    
    
    QUERY_RESPONSE_OVERRODE_LOCAL_HEURISTICS,

    
    
    QUERY_RESPONSE_WITH_NO_LOCAL_HEURISTICS,
    NUM_SERVER_QUERY_METRICS
  };

  AutofillMetrics();
  virtual ~AutofillMetrics();

  virtual void Log(CreditCardInfoBarMetric metric) const;
  virtual void Log(HeuristicTypeQualityMetric metric,
                   AutofillFieldType field_type,
                   const std::string& experiment_id) const;
  virtual void Log(PredictedTypeQualityMetric metric,
                   AutofillFieldType field_type,
                   const std::string& experiment_id) const;
  virtual void Log(QualityMetric metric,
                   const std::string& experiment_id) const;
  virtual void Log(ServerQueryMetric metric) const;
  virtual void Log(ServerTypeQualityMetric metric,
                   AutofillFieldType field_type,
                   const std::string& experiment_id) const;

  
  virtual void LogIsAutofillEnabledAtPageLoad(bool enabled) const;

  
  virtual void LogIsAutofillEnabledAtStartup(bool enabled) const;

  
  virtual void LogStoredProfileCount(size_t num_profiles) const;

  
  
  virtual void LogAddressSuggestionsCount(size_t num_suggestions) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutofillMetrics);
};

#endif  
