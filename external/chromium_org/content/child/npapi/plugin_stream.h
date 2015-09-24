// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_PLUGIN_STREAM_H_
#define CONTENT_CHILD_NPAPI_PLUGIN_STREAM_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "build/build_config.h"
#include "third_party/npapi/bindings/npapi.h"

namespace content {

class PluginInstance;
class WebPluginResourceClient;

class PluginStream : public base::RefCounted<PluginStream> {
 public:
  
  
  PluginStream(PluginInstance* instance,
               const char* url,
               bool need_notify,
               void* notify_data);

  
  
  
  
  
  
  
  bool Open(const std::string &mime_type,
            const std::string &headers,
            uint32 length,
            uint32 last_modified,
            bool request_is_seekable);

  
  int Write(const char* buf, const int len, int data_offset);

  
  void WriteAsFile();

  
  void Notify(NPReason reason);

  
  virtual bool Close(NPReason reason);

  virtual WebPluginResourceClient* AsResourceClient();

  
  virtual void CancelRequest() {}

  NPStream* stream() { return &stream_; }

  PluginInstance* instance() { return instance_.get(); }

  
  bool seekable() const { return seekable_stream_; }

  void set_seekable(bool seekable) { seekable_stream_ = seekable; }

  
  bool notify_needed() const { return notify_needed_; }

  void* notify_data() const { return notify_data_; }

 protected:
  friend class base::RefCounted<PluginStream>;

  virtual ~PluginStream();

  
  bool open() { return opened_; }

 private:
  
  void ResetTempFileHandle();

  
  void ResetTempFileName();

  
  
  
  bool OpenTempFile();

  
  void CloseTempFile();

  
  size_t WriteBytes(const char* buf, size_t length);

  
  bool WriteToFile(const char* buf, size_t length);

  
  
  bool WriteToPlugin(const char* buf, const int length, const int data_offset);

  
  
  int TryWriteToPlugin(const char* buf, const int length,
                       const int data_offset);

  
  void OnDelayDelivery();

  
  bool TempFileIsValid() const;

  
  bool RequestedPluginModeIsAsFile() const;

 private:
  NPStream                      stream_;
  std::string                   headers_;
  scoped_refptr<PluginInstance> instance_;
  bool                          notify_needed_;
  void*                         notify_data_;
  bool                          close_on_write_data_;
  uint16                        requested_plugin_mode_;
  bool                          opened_;
#if defined(OS_WIN)
  char                          temp_file_name_[MAX_PATH];
  HANDLE                        temp_file_handle_;
#elif defined(OS_POSIX)
  FILE*                         temp_file_;
  base::FilePath                temp_file_path_;
#endif
  std::vector<char>             delivery_data_;
  int                           data_offset_;
  bool                          seekable_stream_;
  std::string                   mime_type_;
  DISALLOW_COPY_AND_ASSIGN(PluginStream);
};

}  

#endif  
