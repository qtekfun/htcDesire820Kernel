// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_FIELD_TYPES_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_FIELD_TYPES_H_

#include <map>
#include <set>

#include "base/strings/string16.h"

namespace autofill {

enum ServerFieldType {
  
  NO_SERVER_DATA = 0,
  
  
  UNKNOWN_TYPE = 1,
  
  
  EMPTY_TYPE = 2,
  
  NAME_FIRST = 3,
  NAME_MIDDLE = 4,
  NAME_LAST = 5,
  NAME_MIDDLE_INITIAL = 6,
  NAME_FULL = 7,
  NAME_SUFFIX = 8,
  EMAIL_ADDRESS = 9,
  PHONE_HOME_NUMBER = 10,
  PHONE_HOME_CITY_CODE = 11,
  PHONE_HOME_COUNTRY_CODE = 12,
  PHONE_HOME_CITY_AND_NUMBER = 13,
  PHONE_HOME_WHOLE_NUMBER = 14,

  

  
  
  PHONE_FAX_NUMBER = 20,
  PHONE_FAX_CITY_CODE = 21,
  PHONE_FAX_COUNTRY_CODE = 22,
  PHONE_FAX_CITY_AND_NUMBER = 23,
  PHONE_FAX_WHOLE_NUMBER = 24,

  

  ADDRESS_HOME_LINE1 = 30,
  ADDRESS_HOME_LINE2 = 31,
  ADDRESS_HOME_APT_NUM = 32,
  ADDRESS_HOME_CITY = 33,
  ADDRESS_HOME_STATE = 34,
  ADDRESS_HOME_ZIP = 35,
  ADDRESS_HOME_COUNTRY = 36,
  ADDRESS_BILLING_LINE1 = 37,
  ADDRESS_BILLING_LINE2 = 38,
  ADDRESS_BILLING_APT_NUM = 39,
  ADDRESS_BILLING_CITY = 40,
  ADDRESS_BILLING_STATE = 41,
  ADDRESS_BILLING_ZIP = 42,
  ADDRESS_BILLING_COUNTRY = 43,

  

  CREDIT_CARD_NAME = 51,
  CREDIT_CARD_NUMBER = 52,
  CREDIT_CARD_EXP_MONTH = 53,
  CREDIT_CARD_EXP_2_DIGIT_YEAR = 54,
  CREDIT_CARD_EXP_4_DIGIT_YEAR = 55,
  CREDIT_CARD_EXP_DATE_2_DIGIT_YEAR = 56,
  CREDIT_CARD_EXP_DATE_4_DIGIT_YEAR = 57,
  CREDIT_CARD_TYPE = 58,
  CREDIT_CARD_VERIFICATION_CODE = 59,

  COMPANY_NAME = 60,

  
  FIELD_WITH_DEFAULT_VALUE = 61,

  PHONE_BILLING_NUMBER = 62,
  PHONE_BILLING_CITY_CODE = 63,
  PHONE_BILLING_COUNTRY_CODE = 64,
  PHONE_BILLING_CITY_AND_NUMBER = 65,
  PHONE_BILLING_WHOLE_NUMBER = 66,

  NAME_BILLING_FIRST = 67,
  NAME_BILLING_MIDDLE = 68,
  NAME_BILLING_LAST = 69,
  NAME_BILLING_MIDDLE_INITIAL = 70,
  NAME_BILLING_FULL = 71,
  NAME_BILLING_SUFFIX = 72,

  
  
  
  MERCHANT_EMAIL_SIGNUP = 73,
  MERCHANT_PROMO_CODE = 74,

  
  
  
  PASSWORD = 75,
  ACCOUNT_CREATION_PASSWORD = 76,

  
  
  
  ADDRESS_HOME_STREET_ADDRESS = 77,
  ADDRESS_BILLING_STREET_ADDRESS = 78,

  
  
  
  
  
  ADDRESS_HOME_SORTING_CODE = 79,
  ADDRESS_BILLING_SORTING_CODE = 80,

  
  
  
  ADDRESS_HOME_DEPENDENT_LOCALITY = 81,
  ADDRESS_BILLING_DEPENDENT_LOCALITY = 82,

  
  

  MAX_VALID_FIELD_TYPE = 83,
};

enum HtmlFieldType {
  
  HTML_TYPE_UNKNOWN,

  
  HTML_TYPE_NAME,
  HTML_TYPE_GIVEN_NAME,
  HTML_TYPE_ADDITIONAL_NAME,
  HTML_TYPE_FAMILY_NAME,

  
  HTML_TYPE_ORGANIZATION,

  
  HTML_TYPE_STREET_ADDRESS,
  HTML_TYPE_ADDRESS_LINE1,
  HTML_TYPE_ADDRESS_LINE2,
  HTML_TYPE_LOCALITY,      
  HTML_TYPE_REGION,        
  HTML_TYPE_COUNTRY_CODE,  
  HTML_TYPE_COUNTRY_NAME,  
  HTML_TYPE_POSTAL_CODE,

  
  HTML_TYPE_CREDIT_CARD_NAME,
  HTML_TYPE_CREDIT_CARD_NUMBER,
  HTML_TYPE_CREDIT_CARD_EXP,
  HTML_TYPE_CREDIT_CARD_EXP_MONTH,
  HTML_TYPE_CREDIT_CARD_EXP_YEAR,
  HTML_TYPE_CREDIT_CARD_VERIFICATION_CODE,
  HTML_TYPE_CREDIT_CARD_TYPE,

  
  HTML_TYPE_TEL,
  HTML_TYPE_TEL_COUNTRY_CODE,
  HTML_TYPE_TEL_NATIONAL,
  HTML_TYPE_TEL_AREA_CODE,
  HTML_TYPE_TEL_LOCAL,
  HTML_TYPE_TEL_LOCAL_PREFIX,
  HTML_TYPE_TEL_LOCAL_SUFFIX,

  
  HTML_TYPE_EMAIL,

  
  
  
  
  HTML_TYPE_ADDITIONAL_NAME_INITIAL,
  HTML_TYPE_CREDIT_CARD_EXP_DATE_2_DIGIT_YEAR,
  HTML_TYPE_CREDIT_CARD_EXP_DATE_4_DIGIT_YEAR,
  HTML_TYPE_CREDIT_CARD_EXP_2_DIGIT_YEAR,
  HTML_TYPE_CREDIT_CARD_EXP_4_DIGIT_YEAR,
};

enum HtmlFieldMode {
  HTML_MODE_NONE,
  HTML_MODE_BILLING,
  HTML_MODE_SHIPPING,
};

enum FieldTypeGroup {
  NO_GROUP,
  NAME,
  NAME_BILLING,
  EMAIL,
  COMPANY,
  ADDRESS_HOME,
  ADDRESS_BILLING,
  PHONE_HOME,
  PHONE_BILLING,
  CREDIT_CARD,
  PASSWORD_FIELD,
};

typedef std::set<ServerFieldType> ServerFieldTypeSet;
typedef std::map<base::string16, ServerFieldType> ServerFieldTypeMap;

}  

#endif  