// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_PREDICTOR_TABLES_H_
#define CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_PREDICTOR_TABLES_H_


#include <map>
#include <string>
#include <vector>

#include "base/time/time.h"
#include "chrome/browser/predictors/predictor_table_base.h"
#include "chrome/browser/predictors/resource_prefetch_common.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"

namespace sql {
class Statement;
}

namespace predictors {

class ResourcePrefetchPredictorTables : public PredictorTableBase {
 public:
  
  
  struct ResourceRow {
    ResourceRow();
    ResourceRow(const ResourceRow& other);
    ResourceRow(const std::string& main_frame_url,
                const std::string& resource_url,
                ResourceType::Type resource_type,
                int number_of_hits,
                int number_of_misses,
                int consecutive_misses,
                double average_position);
    void UpdateScore();
    bool operator==(const ResourceRow& rhs) const;

    
    
    
    std::string primary_key;

    GURL resource_url;
    ResourceType::Type resource_type;
    int number_of_hits;
    int number_of_misses;
    int consecutive_misses;
    double average_position;

    
    float score;
  };
  typedef std::vector<ResourceRow> ResourceRows;

  
  struct ResourceRowSorter {
    bool operator()(const ResourceRow& x, const ResourceRow& y) const;
  };

  
  
  
  struct PrefetchData {
    PrefetchData(PrefetchKeyType key_type, const std::string& primary_key);
    PrefetchData(const PrefetchData& other);
    ~PrefetchData();
    bool operator==(const PrefetchData& rhs) const;

    bool is_host() const { return key_type == PREFETCH_KEY_TYPE_HOST; }

    
    PrefetchKeyType key_type;  
    std::string primary_key;  

    base::Time last_visit;
    ResourceRows resources;
  };
  
  typedef std::map<std::string, PrefetchData> PrefetchDataMap;

  
  virtual void GetAllData(PrefetchDataMap* url_data_map,
                          PrefetchDataMap* host_data_map);

  
  
  
  
  virtual void UpdateData(const PrefetchData& url_data,
                          const PrefetchData& host_data);

  
  virtual void DeleteData(const std::vector<std::string>& urls,
                  const std::vector<std::string>& hosts);

  
  virtual void DeleteSingleDataPoint(const std::string& key,
                                     PrefetchKeyType key_type);

  
  virtual void DeleteAllData();

  
  static const size_t kMaxStringLength;

 private:
  friend class PredictorDatabaseInternal;
  friend class MockResourcePrefetchPredictorTables;

  ResourcePrefetchPredictorTables();
  virtual ~ResourcePrefetchPredictorTables();

  
  
  void GetAllDataHelper(PrefetchKeyType key_type,
                        PrefetchDataMap* data_map,
                        std::vector<std::string>* to_delete);
  bool UpdateDataHelper(const PrefetchData& data);
  void DeleteDataHelper(PrefetchKeyType key_type,
                        const std::vector<std::string>& keys);

  
  
  bool StringsAreSmallerThanDBLimit(const PrefetchData& data) const;

  
  virtual void CreateTableIfNonExistent() OVERRIDE;
  virtual void LogDatabaseStats() OVERRIDE;

  
  
  sql::Statement* GetUrlResourceDeleteStatement();
  sql::Statement* GetUrlResourceUpdateStatement();
  sql::Statement* GetUrlMetadataDeleteStatement();
  sql::Statement* GetUrlMetadataUpdateStatement();

  sql::Statement* GetHostResourceDeleteStatement();
  sql::Statement* GetHostResourceUpdateStatement();
  sql::Statement* GetHostMetadataDeleteStatement();
  sql::Statement* GetHostMetadataUpdateStatement();

  DISALLOW_COPY_AND_ASSIGN(ResourcePrefetchPredictorTables);
};

}  

#endif  
