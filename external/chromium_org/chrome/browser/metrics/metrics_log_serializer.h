// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_METRICS_LOG_SERIALIZER_H_
#define CHROME_BROWSER_METRICS_METRICS_LOG_SERIALIZER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "chrome/common/metrics/metrics_log_manager.h"

namespace base {
class ListValue;
}

class MetricsLogSerializer : public MetricsLogManager::LogSerializer {
 public:
  
  
  enum LogReadStatus {
    RECALL_SUCCESS,         
    LIST_EMPTY,             
    LIST_SIZE_MISSING,      
    LIST_SIZE_TOO_SMALL,    
    LIST_SIZE_CORRUPTION,   
    LOG_STRING_CORRUPTION,  
    CHECKSUM_CORRUPTION,    
    CHECKSUM_STRING_CORRUPTION,  
                                 
    DECODE_FAIL,            
    DEPRECATED_XML_PROTO_MISMATCH,  
                                    
    END_RECALL_STATUS       
  };

  MetricsLogSerializer();
  virtual ~MetricsLogSerializer();

  
  virtual void SerializeLogs(
      const std::vector<MetricsLogManager::SerializedLog>& logs,
      MetricsLogManager::LogType log_type) OVERRIDE;
  virtual void DeserializeLogs(
      MetricsLogManager::LogType log_type,
      std::vector<MetricsLogManager::SerializedLog>* logs) OVERRIDE;

 private:
  
  
  
  
  
  static void WriteLogsToPrefList(
      const std::vector<MetricsLogManager::SerializedLog>& local_list,
      size_t list_length_limit,
      size_t byte_limit,
      base::ListValue* list);

  
  
  static LogReadStatus ReadLogsFromPrefList(
      const base::ListValue& list,
      std::vector<MetricsLogManager::SerializedLog>* local_list);

  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, EmptyLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, SingleElementLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, LongButTinyLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, LongButSmallLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, ShortButLargeLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, LongAndLargeLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, SmallRecoveredListSize);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, RemoveSizeFromLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, CorruptSizeOfLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsLogSerializerTest, CorruptChecksumOfLogList);

  DISALLOW_COPY_AND_ASSIGN(MetricsLogSerializer);
};

#endif  
