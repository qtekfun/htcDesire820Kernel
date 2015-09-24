// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_FORM_FIELD_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_FORM_FIELD_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/field_types.h"

namespace autofill {

class AutofillField;
class AutofillScanner;

class FormField {
 public:
  virtual ~FormField() {}

  
  
  
  static void ParseFormFields(const std::vector<AutofillField*>& fields,
                              ServerFieldTypeMap* map);

 protected:
  
  enum MatchType {
    
    MATCH_LABEL      = 1 << 0,
    MATCH_NAME       = 1 << 1,
    MATCH_VALUE      = 1 << 2,

    
    MATCH_TEXT       = 1 << 3,
    MATCH_EMAIL      = 1 << 4,
    MATCH_TELEPHONE  = 1 << 5,
    MATCH_SELECT     = 1 << 6,
    MATCH_TEXT_AREA  = 1 << 7,
    MATCH_ALL_INPUTS =
        MATCH_TEXT | MATCH_EMAIL | MATCH_TELEPHONE | MATCH_SELECT |
        MATCH_TEXT_AREA,

    
    MATCH_DEFAULT    = MATCH_LABEL | MATCH_NAME | MATCH_VALUE | MATCH_TEXT,
  };

  
  FormField() {}

  
  
  static bool ParseField(AutofillScanner* scanner,
                         const base::string16& pattern,
                         const AutofillField** match);

  
  
  
  
  
  static bool ParseFieldSpecifics(AutofillScanner* scanner,
                                  const base::string16& pattern,
                                  int match_type,
                                  const AutofillField** match);

  
  
  static bool ParseEmptyLabel(AutofillScanner* scanner,
                              const AutofillField** match);

  
  static bool AddClassification(const AutofillField* field,
                                ServerFieldType type,
                                ServerFieldTypeMap* map);

  
  
  
  
  virtual bool ClassifyField(ServerFieldTypeMap* map) const = 0;

 private:
  FRIEND_TEST_ALL_PREFIXES(FormFieldTest, Match);

  
  
  typedef FormField* ParseFunction(AutofillScanner* scanner);

  
  
  
  
  static bool MatchAndAdvance(AutofillScanner* scanner,
                              const base::string16& pattern,
                              int match_type,
                              const AutofillField** match);

  
  
  static bool Match(const AutofillField* field,
                    const base::string16& pattern,
                    int match_type);

  
  
  
  
  
  static void ParseFormFieldsPass(ParseFunction parse,
                                  std::vector<const AutofillField*>* fields,
                                  ServerFieldTypeMap* map);

  
  static bool MatchesFormControlType(const std::string& type, int match_type);

  DISALLOW_COPY_AND_ASSIGN(FormField);
};

}  

#endif  
