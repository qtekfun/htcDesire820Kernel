// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_XML_PARSER_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_XML_PARSER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/browser/autofill/form_structure.h"
#include "third_party/expat/files/lib/expat.h"
#include "third_party/libjingle/source/talk/xmllite/xmlparser.h"

class AutofillXmlParser : public buzz::XmlParseHandler {
 public:
  AutofillXmlParser();

  
  bool succeeded() const { return succeeded_; }

 private:
  
  
  
  virtual void EndElement(buzz::XmlParseContext* context, const char* name);

  
  
  
  
  
  virtual void CharacterData(buzz::XmlParseContext* context, const char* text,
                             int len);

  
  
  
  virtual void Error(buzz::XmlParseContext* context, XML_Error error_code);

  
  bool succeeded_;

  DISALLOW_COPY_AND_ASSIGN(AutofillXmlParser);
};

class AutofillQueryXmlParser : public AutofillXmlParser {
 public:
  AutofillQueryXmlParser(std::vector<AutofillFieldType>* field_types,
                         UploadRequired* upload_required,
                         std::string* experiment_id);

 private:
  
  
  
  
  virtual void StartElement(buzz::XmlParseContext* context, const char* name,
                            const char** attrs);

  
  
  
  
  int GetIntValue(buzz::XmlParseContext* context, const char* attribute);

  
  std::vector<AutofillFieldType>* field_types_;

  
  
  UploadRequired* upload_required_;

  
  
  std::string* experiment_id_;

  DISALLOW_COPY_AND_ASSIGN(AutofillQueryXmlParser);
};

class AutofillUploadXmlParser : public AutofillXmlParser {
 public:
  AutofillUploadXmlParser(double* positive_upload_rate,
                          double* negative_upload_rate);

 private:
  
  
  
  
  virtual void StartElement(buzz::XmlParseContext* context, const char* name,
                            const char** attrs);

  
  
  
  
  double GetDoubleValue(buzz::XmlParseContext* context, const char* attribute);

  
  bool succeeded_;

  double* positive_upload_rate_;
  double* negative_upload_rate_;

  DISALLOW_COPY_AND_ASSIGN(AutofillUploadXmlParser);
};

#endif  
