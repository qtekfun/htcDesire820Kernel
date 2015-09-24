// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_SEARCH_OPERATION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_SEARCH_OPERATION_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/file_system_interface.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace base {
class SequencedTaskRunner;
}  

namespace google_apis {
class ResourceEntry;
class ResourceList;
}  

namespace drive {

class JobScheduler;

namespace internal {
class ResourceMetadata;
}  

namespace file_system {

class SearchOperation {
 public:
  SearchOperation(base::SequencedTaskRunner* blocking_task_runner,
                  JobScheduler* scheduler,
                  internal::ResourceMetadata* metadata);
  ~SearchOperation();

  
  
  
  
  
  
  void Search(const std::string& search_query,
              const GURL& next_link,
              const SearchCallback& callback);

 private:
  
  void SearchAfterGetResourceList(
      const SearchCallback& callback,
      google_apis::GDataErrorCode gdata_error,
      scoped_ptr<google_apis::ResourceList> resource_list);

  
  void SearchAfterResolveSearchResult(
      const SearchCallback& callback,
      const GURL& next_link,
      scoped_ptr<std::vector<SearchResultInfo> > result,
      FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  JobScheduler* scheduler_;
  internal::ResourceMetadata* metadata_;

  
  
  base::WeakPtrFactory<SearchOperation> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(SearchOperation);
};

}  
}  

#endif  
