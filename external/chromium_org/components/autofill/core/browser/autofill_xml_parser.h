// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_XML_PARSER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_XML_PARSER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "components/autofill/core/browser/autofill_server_field_info.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/browser/form_structure.h"
#include "third_party/libjingle/source/talk/xmllite/xmlparser.h"

namespace autofill {

class AutofillXmlParser : public buzz::XmlParseHandler {
 public:
  AutofillXmlParser();
  virtual ~AutofillXmlParser();

  
  bool succeeded() const { return succeeded_; }

 private:
  
  
  
  virtual void EndElement(buzz::XmlParseContext* context,
                          const char* name) OVERRIDE;

  
  
  
  
  
  virtual void CharacterData(buzz::XmlParseContext* context,
                             const char* text,
                             int len) OVERRIDE;

  
  
  
  virtual void Error(buzz::XmlParseContext* context,
                     XML_Error error_code) OVERRIDE;

  
  bool succeeded_;

  DISALLOW_COPY_AND_ASSIGN(AutofillXmlParser);
};

class AutofillQueryXmlParser : public AutofillXmlParser {
 public:
  AutofillQueryXmlParser(std::vector<AutofillServerFieldInfo>* field_infos,
                         UploadRequired* upload_required,
                         std::string* experiment_id);
  virtual ~AutofillQueryXmlParser();

 private:
  
  
  
  
  virtual void StartElement(buzz::XmlParseContext* context,
                            const char* name,
                            const char** attrs) OVERRIDE;

  
  
  
  
  void ParseElementDescriptor(buzz::XmlParseContext* context,
                              const char* const* attrs,
                              WebElementDescriptor* element_descriptor);

  
  
  
  
  int GetIntValue(buzz::XmlParseContext* context, const char* attribute);

  
  std::vector<AutofillServerFieldInfo>* field_infos_;

  
  
  UploadRequired* upload_required_;

  
  
  std::string* experiment_id_;

  DISALLOW_COPY_AND_ASSIGN(AutofillQueryXmlParser);
};

class AutofillUploadXmlParser : public AutofillXmlParser {
 public:
  AutofillUploadXmlParser(double* positive_upload_rate,
                          double* negative_upload_rate);

 private:
  
  
  
  
  virtual void StartElement(buzz::XmlParseContext* context,
                            const char* name,
                            const char** attrs) OVERRIDE;

  
  
  
  
  double GetDoubleValue(buzz::XmlParseContext* context, const char* attribute);

  
  bool succeeded_;

  double* positive_upload_rate_;
  double* negative_upload_rate_;

  DISALLOW_COPY_AND_ASSIGN(AutofillUploadXmlParser);
};

}  

#endif  
