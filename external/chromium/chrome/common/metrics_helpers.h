// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_METRICS_HELPERS_H_
#define CHROME_COMMON_METRICS_HELPERS_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/histogram.h"
#include "base/time.h"
#include "content/common/page_transition_types.h"

class GURL;
class MetricsLog;

class MetricsLogBase {
 public:
  
  
  
  MetricsLogBase(const std::string& client_id, int session_id,
                 const std::string& version_string);
  virtual ~MetricsLogBase();

  
  void RecordUserAction(const char* key);

  enum WindowEventType {
    WINDOW_CREATE = 0,
    WINDOW_OPEN,
    WINDOW_CLOSE,
    WINDOW_DESTROY
  };

  void RecordWindowEvent(WindowEventType type, int window_id, int parent_id);

  
  
  
  
  
  void RecordLoadEvent(int window_id,
                       const GURL& url,
                       PageTransition::Type origin,
                       int session_index,
                       base::TimeDelta load_time);

  
  void RecordHistogramDelta(const base::Histogram& histogram,
                            const base::Histogram::SampleSet& snapshot);

  
  
  void CloseLog();

  
  
  
  
  int GetEncodedLogSize();
  bool GetEncodedLog(char* buffer, int buffer_size);
  
  std::string GetEncodedLogString();

  
  int GetElapsedSeconds();

  int num_events() { return num_events_; }

  void set_hardware_class(const std::string& hardware_class) {
    hardware_class_ = hardware_class;
  }

  
  
  static std::string CreateHash(const std::string& value);

  
  static std::string CreateBase64Hash(const std::string& string);

  
  
  
  
  static int64 GetBuildTime();

  
  
  static void set_version_extension(const std::string& extension) {
    version_extension_ = extension;
  }

  virtual MetricsLog* AsMetricsLog();

 protected:
  class XmlWrapper;

  
  
  virtual std::string GetCurrentTimeString();
  
  
  
  
  class ScopedElement {
   public:
    ScopedElement(MetricsLogBase* log, const std::string& name) : log_(log) {
      DCHECK(log);
      log->StartElement(name.c_str());
    }

    ScopedElement(MetricsLogBase* log, const char* name) : log_(log) {
      DCHECK(log);
      log->StartElement(name);
    }

    ~ScopedElement() {
      log_->EndElement();
    }

   private:
     MetricsLogBase* log_;
  };
  friend class ScopedElement;

  static const char* WindowEventTypeToString(WindowEventType type);

  
  
  
  void FreeDocWriter();

  
  void StartElement(const char* name);
  void EndElement();
  void WriteAttribute(const std::string& name, const std::string& value);
  void WriteIntAttribute(const std::string& name, int value);
  void WriteInt64Attribute(const std::string& name, int64 value);

  
  void WriteCommonEventAttributes();

  
  static std::string version_extension_;

  base::Time start_time_;
  base::Time end_time_;

  std::string client_id_;
  std::string session_id_;
  std::string hardware_class_;

  
  // no longer be written to.  It is only used for sanity checking and is
  
  bool locked_;

  
  XmlWrapper* xml_wrapper_;

  int num_events_;  

  DISALLOW_COPY_AND_ASSIGN(MetricsLogBase);
};

class HistogramSender {
 protected:
  HistogramSender();
  virtual ~HistogramSender();

  
  
  
  void TransmitAllHistograms(base::Histogram::Flags flags_to_set,
                             bool send_only_uma);

  
  
  
  virtual void TransmitHistogramDelta(
      const base::Histogram& histogram,
      const base::Histogram::SampleSet& snapshot) = 0;

  
  virtual void InconsistencyDetected(int problem) = 0;
  virtual void UniqueInconsistencyDetected(int problem) = 0;
  virtual void SnapshotProblemResolved(int amount) = 0;

 private:
  
  typedef std::map<std::string, base::Histogram::SampleSet> LoggedSampleMap;
  
  typedef std::map<std::string, int> ProblemMap;

  
  void TransmitHistogram(const base::Histogram& histogram);

  
  
  LoggedSampleMap logged_samples_;

  
  scoped_ptr<ProblemMap> inconsistencies_;

  DISALLOW_COPY_AND_ASSIGN(HistogramSender);
};

class MetricsServiceBase : public HistogramSender {
 protected:
  MetricsServiceBase();
  virtual ~MetricsServiceBase();

  
  bool pending_log() const {
    return pending_log_ || !compressed_log_.empty();
  }

  
  
  bool Bzip2Compress(const std::string& input, std::string* output);

  
  
  void DiscardPendingLog();

  
  
  void RecordCurrentHistograms();

  
  MetricsLogBase* pending_log_;

  
  
  
  std::string compressed_log_;

  
  MetricsLogBase* current_log_;

 private:
  
  virtual void TransmitHistogramDelta(
      const base::Histogram& histogram,
      const base::Histogram::SampleSet& snapshot);
  virtual void InconsistencyDetected(int problem);
  virtual void UniqueInconsistencyDetected(int problem);
  virtual void SnapshotProblemResolved(int amount);

  DISALLOW_COPY_AND_ASSIGN(MetricsServiceBase);
};

#endif  
