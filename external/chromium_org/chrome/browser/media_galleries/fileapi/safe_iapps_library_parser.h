// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_SAFE_IAPPS_LIBRARY_PARSER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_SAFE_IAPPS_LIBRARY_PARSER_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "chrome/common/media_galleries/iphoto_library.h"
#include "chrome/common/media_galleries/itunes_library.h"
#include "content/public/browser/utility_process_host.h"
#include "content/public/browser/utility_process_host_client.h"

namespace IPC {
class Message;
}

namespace iapps {

class SafeIAppsLibraryParser : public content::UtilityProcessHostClient {
 public:
  typedef base::Callback<void(bool, const iphoto::parser::Library&)>
      IPhotoParserCallback;
  typedef base::Callback<void(bool, const itunes::parser::Library&)>
      ITunesParserCallback;

  SafeIAppsLibraryParser();

  
  void ParseIPhotoLibrary(const base::FilePath& library_file,
                          const IPhotoParserCallback& callback);

  
  void ParseITunesLibrary(const base::FilePath& library_file,
                          const ITunesParserCallback& callback);


 private:
  enum ParserState {
    INITIAL_STATE,
    PINGED_UTILITY_PROCESS_STATE,
    STARTED_PARSING_STATE,
    FINISHED_PARSING_STATE,
  };

  
  virtual ~SafeIAppsLibraryParser();

  
  void Start();

  
  void StartProcessOnIOThread();

  
  
  
  void OnUtilityProcessStarted();

  
  
#if defined(OS_MACOSX)
  void OnGotIPhotoLibrary(bool result, const iphoto::parser::Library& library);
#endif

  
  
  void OnGotITunesLibrary(bool result, const itunes::parser::Library& library);

  
  
  void OnOpenLibraryFileFailed();

  
  void OnError();

  
  
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  base::FilePath library_file_;

  
  
  base::PlatformFile library_platform_file_;

  
  base::WeakPtr<content::UtilityProcessHost> utility_process_host_;

  
  ITunesParserCallback itunes_callback_;

  
  IPhotoParserCallback iphoto_callback_;

  
  
  ParserState parser_state_;

  DISALLOW_COPY_AND_ASSIGN(SafeIAppsLibraryParser);
};

}  

#endif  
