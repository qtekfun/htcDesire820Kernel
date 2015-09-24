// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_METRICS_PRIVATE_METRICS_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_METRICS_PRIVATE_METRICS_PRIVATE_API_H_

#include <string>

#include "base/metrics/histogram.h"
#include "extensions/browser/extension_function.h"

namespace extensions {

class MetricsPrivateGetIsCrashReportingEnabledFunction
    : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.getIsCrashReportingEnabled",
                             METRICSPRIVATE_GETISCRASHRECORDINGENABLED)

 protected:
  virtual ~MetricsPrivateGetIsCrashReportingEnabledFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateGetFieldTrialFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.getFieldTrial",
                             METRICSPRIVATE_GETFIELDTRIAL)

 protected:
  virtual ~MetricsPrivateGetFieldTrialFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateGetVariationParamsFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.getVariationParams",
                             METRICSPRIVATE_GETVARIATIONPARAMS)

 protected:
  virtual ~MetricsPrivateGetVariationParamsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordUserActionFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordUserAction",
                             METRICSPRIVATE_RECORDUSERACTION)

 protected:
  virtual ~MetricsPrivateRecordUserActionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsHistogramHelperFunction : public SyncExtensionFunction {
 protected:
  virtual ~MetricsHistogramHelperFunction() {}
  bool GetNameAndSample(std::string* name, int* sample);
  virtual bool RecordValue(const std::string& name,
                           base::HistogramType type,
                           int min, int max, size_t buckets,
                           int sample);
};

class MetricsPrivateRecordValueFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordValue",
                             METRICSPRIVATE_RECORDVALUE)

 protected:
  virtual ~MetricsPrivateRecordValueFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordPercentageFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordPercentage",
                             METRICSPRIVATE_RECORDPERCENTAGE)

 protected:
  virtual ~MetricsPrivateRecordPercentageFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordCountFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordCount",
                             METRICSPRIVATE_RECORDCOUNT)

 protected:
  virtual ~MetricsPrivateRecordCountFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordSmallCountFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordSmallCount",
                             METRICSPRIVATE_RECORDSMALLCOUNT)

 protected:
  virtual ~MetricsPrivateRecordSmallCountFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordMediumCountFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordMediumCount",
                             METRICSPRIVATE_RECORDMEDIUMCOUNT)

 protected:
  virtual ~MetricsPrivateRecordMediumCountFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordTimeFunction : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordTime",
                             METRICSPRIVATE_RECORDTIME)

 protected:
  virtual ~MetricsPrivateRecordTimeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordMediumTimeFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordMediumTime",
                             METRICSPRIVATE_RECORDMEDIUMTIME)

 protected:
  virtual ~MetricsPrivateRecordMediumTimeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class MetricsPrivateRecordLongTimeFunction
    : public MetricsHistogramHelperFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("metricsPrivate.recordLongTime",
                             METRICSPRIVATE_RECORDLONGTIME)

 protected:
  virtual ~MetricsPrivateRecordLongTimeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

} 

#endif  
