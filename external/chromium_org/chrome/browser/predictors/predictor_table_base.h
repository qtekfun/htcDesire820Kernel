// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_PREDICTOR_TABLE_BASE_H_
#define CHROME_BROWSER_PREDICTORS_PREDICTOR_TABLE_BASE_H_

#include "base/memory/ref_counted.h"
#include "base/synchronization/cancellation_flag.h"

namespace sql {
class Connection;
}

namespace predictors {

class PredictorTableBase
    : public base::RefCountedThreadSafe<PredictorTableBase> {
 protected:
  PredictorTableBase();
  virtual ~PredictorTableBase();

  
  virtual void CreateTableIfNonExistent() = 0;
  virtual void LogDatabaseStats() = 0;
  void Initialize(sql::Connection* db);
  void SetCancelled();
  sql::Connection* DB();
  void ResetDB();

  bool CantAccessDatabase();

 private:
  base::CancellationFlag cancelled_;

  friend class base::RefCountedThreadSafe<PredictorTableBase>;

  sql::Connection* db_;

  DISALLOW_COPY_AND_ASSIGN(PredictorTableBase);
};

}  

#endif  
