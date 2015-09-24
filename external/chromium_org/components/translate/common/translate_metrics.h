// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_TRANSLATE_COMMON_TRANSLATE_METRICS_H_
#define COMPONENTS_TRANSLATE_COMMON_TRANSLATE_METRICS_H_

#include <string>

#include "base/time/time.h"

namespace translate {

enum MetricsNameIndex {
  UMA_LANGUAGE_DETECTION,
  UMA_CONTENT_LANGUAGE,
  UMA_HTML_LANG,
  UMA_LANGUAGE_VERIFICATION,
  UMA_TIME_TO_BE_READY,
  UMA_TIME_TO_LOAD,
  UMA_TIME_TO_TRANSLATE,
  UMA_USER_ACTION_DURATION,
  UMA_PAGE_SCHEME,
  UMA_SIMILAR_LANGUAGE_MATCH,
  UMA_MAX,
};

enum LanguageCheckType {
  LANGUAGE_NOT_PROVIDED,
  LANGUAGE_VALID,
  LANGUAGE_INVALID,
  LANGUAGE_MAX,
};

enum LanguageVerificationType {
  LANGUAGE_VERIFICATION_CLD_DISABLED,  
  LANGUAGE_VERIFICATION_CLD_ONLY,
  LANGUAGE_VERIFICATION_UNKNOWN,
  LANGUAGE_VERIFICATION_CLD_AGREE,
  LANGUAGE_VERIFICATION_CLD_DISAGREE,
  LANGUAGE_VERIFICATION_TRUST_CLD,
  LANGUAGE_VERIFICATION_CLD_COMPLEMENT_SUB_CODE,
  LANGUAGE_VERIFICATION_MAX,
};

enum SchemeType {
  SCHEME_HTTP,
  SCHEME_HTTPS,
  SCHEME_OTHERS,
  SCHEME_MAX,
};

void ReportContentLanguage(const std::string& provided_code,
                           const std::string& revised_code);

void ReportHtmlLang(const std::string& provided_code,
                    const std::string& revised_code);

void ReportLanguageVerification(LanguageVerificationType type);

void ReportTimeToBeReady(double time_in_msec);

void ReportTimeToLoad(double time_in_msec);

void ReportTimeToTranslate(double time_in_msec);

void ReportUserActionDuration(base::TimeTicks begin, base::TimeTicks end);

void ReportPageScheme(const std::string& scheme);

void ReportLanguageDetectionTime(base::TimeTicks begin, base::TimeTicks end);

void ReportSimilarLanguageMatch(bool match);

const char* GetMetricsName(MetricsNameIndex index);

}  

#endif  
