// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_OPERATION_H_
#define CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_OPERATION_H_

#include "base/callback.h"
#include "base/md5.h"
#include "base/memory/ref_counted_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/extensions/api/image_writer_private/image_writer_utils.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/extensions/api/image_writer_private.h"

namespace image_writer_api = extensions::api::image_writer_private;

namespace base {
class FilePath;
}  

namespace extensions {
namespace image_writer {

const int kProgressComplete = 100;

class OperationManager;

class Operation
    : public base::RefCountedThreadSafe<Operation> {
 public:
  typedef base::Callback<void(bool, const std::string&)> StartWriteCallback;
  typedef base::Callback<void(bool, const std::string&)> CancelWriteCallback;
  typedef std::string ExtensionId;

  Operation(base::WeakPtr<OperationManager> manager,
            const ExtensionId& extension_id,
            const std::string& storage_unit_id);

  
  virtual void Start() = 0;

  
  
  
  void Cancel();

  
  void Abort();

  
  int GetProgress();
  image_writer_api::Stage GetStage();

 protected:
  virtual ~Operation();

  
  
  
  virtual void Error(const std::string& error_message);

  
  
  void SetProgress(int progress);
  
  
  void SetStage(image_writer_api::Stage stage);

  
  bool IsCancelled();

  
  
  
  void AddCleanUpFunction(base::Closure);

  void UnzipStart(scoped_ptr<base::FilePath> zip_file);
  void WriteStart();
  void VerifyWriteStart();
  void Finish();

  
  
  
  
  
  
  void GetMD5SumOfFile(
      scoped_ptr<base::FilePath> file,
      int64 file_size,
      int progress_offset,
      int progress_scale,
      const base::Callback<void(scoped_ptr<std::string>)>& callback);

  base::WeakPtr<OperationManager> manager_;
  const ExtensionId extension_id_;

  base::FilePath image_path_;
  const std::string storage_unit_id_;

 private:
  friend class base::RefCountedThreadSafe<Operation>;

  
#if defined(OS_LINUX) && !defined(CHROMEOS)
  void WriteRun();
  void WriteChunk(scoped_ptr<image_writer_utils::ImageReader> reader,
                  scoped_ptr<image_writer_utils::ImageWriter> writer,
                  int64 bytes_written);
  bool WriteCleanUp(scoped_ptr<image_writer_utils::ImageReader> reader,
                    scoped_ptr<image_writer_utils::ImageWriter> writer);
  void WriteComplete();

  void VerifyWriteStage2(scoped_ptr<std::string> image_hash);
  void VerifyWriteCompare(scoped_ptr<std::string> image_hash,
                          scoped_ptr<std::string> device_hash);
#endif

#if defined(OS_CHROMEOS)
  void StartWriteOnUIThread();

  void OnBurnFinished(const std::string& target_path,
                      bool success,
                      const std::string& error);
  void OnBurnProgress(const std::string& target_path,
                      int64 num_bytes_burnt,
                      int64 total_size);
  void OnBurnError();
#endif

  void MD5Chunk(scoped_ptr<image_writer_utils::ImageReader> reader,
                int64 bytes_processed,
                int64 bytes_total,
                int progress_offset,
                int progress_scale,
                const base::Callback<void(scoped_ptr<std::string>)>& callback);

  
  void CleanUp();

  
  
  image_writer_api::Stage stage_;
  int progress_;

  
  
  base::MD5Context md5_context_;

  
  
  std::vector<base::Closure> cleanup_functions_;
};

}  
}  

#endif  
