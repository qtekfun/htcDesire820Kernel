// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_FILE_DOWNLOADER_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_FILE_DOWNLOADER_H_

#include <deque>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/trusted/validator/nacl_file_info.h"
#include "ppapi/c/private/pp_file_handle.h"
#include "ppapi/c/private/ppb_file_io_private.h"
#include "ppapi/c/trusted/ppb_url_loader_trusted.h"
#include "ppapi/cpp/file_io.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_response_info.h"
#include "ppapi/native_client/src/trusted/plugin/callback_source.h"
#include "ppapi/utility/completion_callback_factory.h"

namespace plugin {

class Plugin;

typedef enum {
  DOWNLOAD_TO_FILE = 0,
  DOWNLOAD_TO_BUFFER,
  DOWNLOAD_STREAM,
  DOWNLOAD_NONE
} DownloadMode;

typedef enum {
  SCHEME_CHROME_EXTENSION,
  SCHEME_DATA,
  SCHEME_OTHER
} UrlSchemeType;

typedef std::vector<char>* FileStreamData;
typedef CallbackSource<FileStreamData> StreamCallbackSource;
typedef pp::CompletionCallbackWithOutput<FileStreamData> StreamCallback;

class FileDownloader {
 public:
  
  
  FileDownloader()
      : instance_(NULL),
        file_open_notify_callback_(pp::BlockUntilComplete()),
        stream_finish_callback_(pp::BlockUntilComplete()),
        file_handle_(PP_kInvalidFileHandle),
        file_io_private_interface_(NULL),
        url_loader_trusted_interface_(NULL),
        open_time_(-1),
        mode_(DOWNLOAD_NONE),
        open_and_stream_(true),
        url_scheme_(SCHEME_OTHER),
        data_stream_callback_source_(NULL) {}
  ~FileDownloader() {}

  
  void Initialize(Plugin* instance);

  
  
  
  
  
  
  
  
  
  
  
  bool Open(const nacl::string& url,
            DownloadMode mode,
            const pp::CompletionCallback& callback,
            bool record_progress,
            PP_URLLoaderTrusted_StatusCallback progress_callback);

  
  
  
  
  
  bool OpenStream(const nacl::string& url,
                  const pp::CompletionCallback& callback,
                  StreamCallbackSource* stream_callback_source);

  
  
  
  
  
  void FinishStreaming(const pp::CompletionCallback& callback);

  
  
  void OpenFast(const nacl::string& url, PP_FileHandle file_handle,
                uint64_t file_token_lo, uint64_t file_token_hi);

  
  
  
  
  
  struct NaClFileInfo GetFileInfo();

  
  int64_t TimeSinceOpenMilliseconds() const;

  
  
  
  
  
  const nacl::string& url() const { return url_; }

  
  const nacl::string& url_to_open() const { return url_to_open_; }

  
  PP_Resource url_loader() const { return url_loader_.pp_resource(); }

  
  
  
  
  
  
  
  
  
  
  bool GetDownloadProgress(int64_t* bytes_received,
                           int64_t* total_bytes_to_be_received) const;

  
  const std::deque<char>& buffer() const { return buffer_; }

  bool streaming_to_file() const;
  bool streaming_to_buffer() const;
  bool streaming_to_user() const;
  bool not_streaming() const;

  int status_code() const { return status_code_; }
  nacl::string GetResponseHeaders() const;

  void set_request_headers(const nacl::string& extra_request_headers) {
    extra_request_headers_ = extra_request_headers;
  }


 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(FileDownloader);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool InitialResponseIsValid();
  void URLLoadStartNotify(int32_t pp_error);
  void URLLoadFinishNotify(int32_t pp_error);
  void URLReadBodyNotify(int32_t pp_error);
  void StreamFinishNotify(int32_t pp_error);
  void GotFileHandleNotify(int32_t pp_error, PP_FileHandle handle);

  Plugin* instance_;
  nacl::string url_to_open_;
  nacl::string url_;
  nacl::string extra_request_headers_;
  pp::URLResponseInfo url_response_;
  pp::CompletionCallback file_open_notify_callback_;
  pp::CompletionCallback stream_finish_callback_;
  pp::FileIO file_reader_;
  PP_FileHandle file_handle_;
  struct NaClFileToken file_token_;
  const PPB_FileIO_Private* file_io_private_interface_;
  const PPB_URLLoaderTrusted* url_loader_trusted_interface_;
  pp::URLLoader url_loader_;
  pp::CompletionCallbackFactory<FileDownloader> callback_factory_;
  int64_t open_time_;
  int32_t status_code_;
  DownloadMode mode_;
  bool open_and_stream_;
  static const uint32_t kTempBufferSize = 2048;
  std::vector<char> temp_buffer_;
  std::deque<char> buffer_;
  UrlSchemeType url_scheme_;
  StreamCallbackSource* data_stream_callback_source_;
  NaClFileInfo cached_file_info_;
};
}  
#endif  
