// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_FORM_STRUCTURE_H_
#define CHROME_BROWSER_AUTOFILL_FORM_STRUCTURE_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "chrome/browser/autofill/autofill_field.h"
#include "chrome/browser/autofill/autofill_type.h"
#include "chrome/browser/autofill/field_types.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/form_data.h"

namespace buzz {
  class XmlElement;
}  

enum RequestMethod {
  GET,
  POST
};

enum UploadRequired {
  UPLOAD_NOT_REQUIRED,
  UPLOAD_REQUIRED,
  USE_UPLOAD_RATES
};

class AutofillMetrics;

class FormStructure {
 public:
  explicit FormStructure(const webkit_glue::FormData& form);
  virtual ~FormStructure();

  
  
  void DetermineHeuristicTypes();

  
  bool EncodeUploadRequest(bool autofill_used, std::string* encoded_xml) const;

  
  
  
  
  
  static bool EncodeQueryRequest(const ScopedVector<FormStructure>& forms,
                                 std::vector<std::string>* encoded_signatures,
                                 std::string* encoded_xml);

  
  
  static void ParseQueryResponse(const std::string& response_xml,
                                 const std::vector<FormStructure*>& forms,
                                 UploadRequired* upload_required,
                                 const AutofillMetrics& metric_logger);

  
  
  std::string FormSignature() const;

  
  
  
  bool IsAutofillable(bool require_method_post) const;

  
  
  
  
  void UpdateAutofillCount();

  
  
  
  bool ShouldBeParsed(bool require_method_post) const;

  
  void UpdateFromCache(const FormStructure& cached_form);

  
  
  
  void LogQualityMetrics(const AutofillMetrics& metric_logger) const;

  
  void set_possible_types(int index, const FieldTypeSet& types);

  const AutofillField* field(int index) const;
  size_t field_count() const;

  
  size_t autofill_count() const { return autofill_count_; }

  
  std::vector<AutofillField*>::const_iterator begin() const {
    return fields_.begin();
  }
  std::vector<AutofillField*>::const_iterator end() const {
    return fields_.end();
  }

  const GURL& source_url() const { return source_url_; }

  virtual std::string server_experiment_id() const;

  bool operator==(const webkit_glue::FormData& form) const;
  bool operator!=(const webkit_glue::FormData& form) const;

 protected:
  
  ScopedVector<AutofillField>* fields() { return &fields_; }

 private:
  friend class FormStructureTest;
  
  static std::string Hash64Bit(const std::string& str);

  enum EncodeRequestType {
    QUERY,
    UPLOAD,
  };

  
  void GetHeuristicFieldInfo(FieldTypeMap* field_types_map);

  
  
  bool EncodeFormRequest(EncodeRequestType request_type,
                         buzz::XmlElement* encompassing_xml_element) const;

  
  
  std::string ConvertPresenceBitsToString() const;

  
  string16 form_name_;

  
  GURL source_url_;

  
  GURL target_url_;

  bool has_credit_card_field_;
  bool has_autofillable_field_;
  bool has_password_fields_;

  
  size_t autofill_count_;

  
  
  ScopedVector<AutofillField> fields_;

  
  
  
  std::string form_signature_field_names_;

  
  
  std::string server_experiment_id_;

  
  RequestMethod method_;

  DISALLOW_COPY_AND_ASSIGN(FormStructure);
};

#endif  
