// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_FILE_ERROR_INJECTOR_H_
#define CONTENT_PUBLIC_TEST_TEST_FILE_ERROR_INJECTOR_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/download_interrupt_reasons.h"
#include "url/gurl.h"

namespace content {

class DownloadId;
class DownloadFileWithErrorsFactory;
class DownloadManager;
class DownloadManagerImpl;

class TestFileErrorInjector
    : public base::RefCountedThreadSafe<TestFileErrorInjector> {
 public:
  enum FileOperationCode {
    FILE_OPERATION_INITIALIZE,
    FILE_OPERATION_WRITE,
    FILE_OPERATION_RENAME_UNIQUIFY,
    FILE_OPERATION_RENAME_ANNOTATE,
  };

  
  struct FileErrorInfo {
    std::string url;  
    FileOperationCode code;  
    int operation_instance;  
    DownloadInterruptReason error;  
  };

  typedef std::map<std::string, FileErrorInfo> ErrorMap;

  
  
  
  
  static scoped_refptr<TestFileErrorInjector> Create(
      DownloadManager* download_manager);

  
  
  
  
  bool AddError(const FileErrorInfo& error_info);

  
  
  void ClearErrors();

  
  
  
  
  bool InjectErrors();

  
  size_t CurrentFileCount() const;

  
  
  size_t TotalFileCount() const;

  
  bool HadFile(const GURL& url) const;

  
  void ClearFoundFiles();

  static std::string DebugString(FileOperationCode code);

 private:
  friend class base::RefCountedThreadSafe<TestFileErrorInjector>;

  typedef std::set<GURL> FileSet;

  explicit TestFileErrorInjector(DownloadManager* download_manager);

  virtual ~TestFileErrorInjector();

  
  
  void RecordDownloadFileConstruction(const GURL& url);
  void RecordDownloadFileDestruction(const GURL& url);

  
  void DownloadFileCreated(GURL url);
  void DestroyingDownloadFile(GURL url);

  
  
  ErrorMap injected_errors_;

  
  FileSet files_;

  
  FileSet found_files_;

  
  DownloadFileWithErrorsFactory* created_factory_;

  
  DownloadManagerImpl* download_manager_;

  DISALLOW_COPY_AND_ASSIGN(TestFileErrorInjector);
};

}  

#endif  
