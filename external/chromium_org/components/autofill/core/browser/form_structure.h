// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_FORM_STRUCTURE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_FORM_STRUCTURE_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "components/autofill/core/browser/autofill_field.h"
#include "components/autofill/core/browser/autofill_type.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/common/web_element_descriptor.h"
#include "url/gurl.h"

enum RequestMethod {
  GET,
  POST
};

enum UploadRequired {
  UPLOAD_NOT_REQUIRED,
  UPLOAD_REQUIRED,
  USE_UPLOAD_RATES
};

namespace base {
class TimeTicks;
}

namespace buzz {
class XmlElement;
}

namespace autofill {

class AutofillMetrics;

struct FormData;
struct FormDataPredictions;

class FormStructure {
 public:
  FormStructure(const FormData& form);
  virtual ~FormStructure();

  
  
  void DetermineHeuristicTypes(const AutofillMetrics& metric_logger);

  
  bool EncodeUploadRequest(const ServerFieldTypeSet& available_field_types,
                           bool form_was_autofilled,
                           std::string* encoded_xml) const;

  
  // This XML will be written VLOG only, never be sent to server. It will
  
  
  bool EncodeFieldAssignments(const ServerFieldTypeSet& available_field_types,
                              std::string* encoded_xml) const;

  
  
  
  
  
  static bool EncodeQueryRequest(const std::vector<FormStructure*>& forms,
                                 std::vector<std::string>* encoded_signatures,
                                 std::string* encoded_xml);

  
  
  static void ParseQueryResponse(
      const std::string& response_xml,
      const std::vector<FormStructure*>& forms,
      const AutofillMetrics& metric_logger);

  
  
  static void GetFieldTypePredictions(
      const std::vector<FormStructure*>& form_structures,
      std::vector<FormDataPredictions>* forms);

  
  
  std::string FormSignature() const;

  
  
  
  bool IsAutofillable(bool require_method_post) const;

  
  
  
  
  void UpdateAutofillCount();

  
  
  
  bool ShouldBeParsed(bool require_method_post) const;

  
  
  
  bool ShouldBeCrowdsourced() const;

  
  void UpdateFromCache(const FormStructure& cached_form);

  
  
  
  
  
  void LogQualityMetrics(const AutofillMetrics& metric_logger,
                         const base::TimeTicks& load_time,
                         const base::TimeTicks& interaction_time,
                         const base::TimeTicks& submission_time) const;

  
  
  
  
  
  
  
  void ParseFieldTypesFromAutocompleteAttributes(bool* found_types,
                                                 bool* found_sections);

  const AutofillField* field(size_t index) const;
  AutofillField* field(size_t index);
  size_t field_count() const;

  
  size_t autofill_count() const { return autofill_count_; }

  
  std::vector<AutofillField*>::const_iterator begin() const {
    return fields_.begin();
  }
  std::vector<AutofillField*>::const_iterator end() const {
    return fields_.end();
  }

  const GURL& source_url() const { return source_url_; }

  void set_upload_required(UploadRequired required) {
    upload_required_ = required;
  }
  UploadRequired upload_required() const { return upload_required_; }

  virtual std::string server_experiment_id() const;

  
  
  FormData ToFormData() const;

  bool operator==(const FormData& form) const;
  bool operator!=(const FormData& form) const;

 private:
  friend class FormStructureTest;
  FRIEND_TEST_ALL_PREFIXES(AutofillDownloadTest, QueryAndUploadTest);

  
  static std::string Hash64Bit(const std::string& str);

  enum EncodeRequestType {
    QUERY,
    UPLOAD,
    FIELD_ASSIGNMENTS,
  };

  
  
  bool EncodeFormRequest(EncodeRequestType request_type,
                         buzz::XmlElement* encompassing_xml_element) const;

  
  
  
  
  
  
  
  void IdentifySections(bool has_author_specified_sections);

  
  bool ShouldSkipField(const FormFieldData& field) const;

  size_t active_field_count() const;

  
  base::string16 form_name_;

  
  GURL source_url_;

  
  GURL target_url_;

  
  size_t autofill_count_;

  
  ScopedVector<AutofillField> fields_;

  
  
  size_t active_field_count_;

  
  
  
  std::string form_signature_field_names_;

  
  
  UploadRequired upload_required_;

  
  
  std::string server_experiment_id_;

  
  RequestMethod method_;

  
  
  bool has_author_specified_types_;

  DISALLOW_COPY_AND_ASSIGN(FormStructure);
};

}  

#endif  
