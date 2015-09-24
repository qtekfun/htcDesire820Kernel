// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_METRICS_METRICS_LOG_MANAGER_H_
#define CHROME_COMMON_METRICS_METRICS_LOG_MANAGER_H_


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/metrics/metrics_log_base.h"

class MetricsLogManager {
 public:
  typedef MetricsLogBase::LogType LogType;

  MetricsLogManager();
  ~MetricsLogManager();

  class SerializedLog {
   public:
    SerializedLog();
    ~SerializedLog();

    const std::string& log_text() const { return log_text_; }
    const std::string& log_hash() const { return log_hash_; }

    
    bool IsEmpty() const;

    
    
    void SwapLogText(std::string* log_text);

    
    void Clear();

    
    void Swap(SerializedLog* other);

   private:
    
    std::string log_text_;

    
    std::string log_hash_;

    
    
  };

  enum StoreType {
    NORMAL_STORE,       
    PROVISIONAL_STORE,  
  };

  
  
  void BeginLoggingWithLog(MetricsLogBase* log, LogType log_type);

  
  MetricsLogBase* current_log() { return current_log_.get(); }

  
  
  void FinishCurrentLog();

  
  bool has_unsent_logs() const {
    return !unsent_initial_logs_.empty() || !unsent_ongoing_logs_.empty();
  }

  
  
  void StageNextLogForUpload();

  
  bool has_staged_log() const;

  
  
  const std::string& staged_log_text() const { return staged_log_.log_text(); }

  
  
  const std::string& staged_log_hash() const { return staged_log_.log_hash(); }

  
  void DiscardStagedLog();

  
  void DiscardCurrentLog();

  
  
  
  
  
  void PauseCurrentLog();

  
  
  void ResumePausedLog();

  
  
  
  
  
  
  
  void StoreStagedLogAsUnsent(StoreType store_type);

  
  
  
  void DiscardLastProvisionalStore();

  // Sets the threshold for how large an onging log can be and still be written
  
  
  void set_max_ongoing_log_store_size(size_t max_size) {
    max_ongoing_log_store_size_ = max_size;
  }

  
  
  class LogSerializer {
   public:
    virtual ~LogSerializer() {}

    
    
    virtual void SerializeLogs(const std::vector<SerializedLog>& logs,
                               LogType log_type) = 0;

    
    
    virtual void DeserializeLogs(LogType log_type,
                                 std::vector<SerializedLog>* logs) = 0;
  };

  
  
  void set_log_serializer(LogSerializer* serializer) {
    log_serializer_.reset(serializer);
  }

  
  
  void PersistUnsentLogs();

  
  
  void LoadPersistedUnsentLogs();

 private:
  
  
  
  
  void StoreLog(SerializedLog* log,
                LogType log_type,
                StoreType store_type);

  
  void CompressCurrentLog(SerializedLog* compressed_log);

  
  bool unsent_logs_loaded_;

  
  scoped_ptr<MetricsLogBase> current_log_;
  LogType current_log_type_;

  
  scoped_ptr<MetricsLogBase> paused_log_;
  LogType paused_log_type_;

  
  
  scoped_ptr<LogSerializer> log_serializer_;

  
  SerializedLog staged_log_;
  LogType staged_log_type_;

  
  
  
  std::vector<SerializedLog> unsent_initial_logs_;
  std::vector<SerializedLog> unsent_ongoing_logs_;

  size_t max_ongoing_log_store_size_;

  
  
  
  
  
  
  int last_provisional_store_index_;
  LogType last_provisional_store_type_;

  DISALLOW_COPY_AND_ASSIGN(MetricsLogManager);
};

#endif  
