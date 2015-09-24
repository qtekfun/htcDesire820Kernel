// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_BACKEND_H_
#define CHROME_BROWSER_SESSIONS_SESSION_BACKEND_H_
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sessions/base_session_service.h"
#include "chrome/browser/sessions/session_command.h"

namespace net {
class FileStream;
}


//   get written to.
class SessionBackend : public base::RefCountedThreadSafe<SessionBackend> {
 public:
  typedef SessionCommand::id_type id_type;
  typedef SessionCommand::size_type size_type;

  
  
  static const int kFileReadBufferSize;

  
  
  
  
  // |path_to_dir| gives the path the files are written two, and |type|
  
  
  SessionBackend(BaseSessionService::SessionType type,
                 const FilePath& path_to_dir);

  
  
  
  
  void Init();

  
  
  
  
  
  void AppendCommands(std::vector<SessionCommand*>* commands,
                      bool reset_first);

  
  
  void ReadLastSessionCommands(
      scoped_refptr<BaseSessionService::InternalGetCommandsRequest> request);

  
  
  
  
  bool ReadLastSessionCommandsImpl(std::vector<SessionCommand*>* commands);

  
  void DeleteLastSession();

  
  
  
  void MoveCurrentSessionToLastSession();

  
  
  void ReadCurrentSessionCommands(
      scoped_refptr<BaseSessionService::InternalGetCommandsRequest> request);

  
  
  
  
  bool ReadCurrentSessionCommandsImpl(std::vector<SessionCommand*>* commands);

 private:
  friend class base::RefCountedThreadSafe<SessionBackend>;

  ~SessionBackend();

  
  
  // is is opened and the header is written to it. After this
  
  
  // the header couldn't be written.
  void ResetFile();

  
  
  net::FileStream* OpenAndWriteHeader(const FilePath& path);

  
  bool AppendCommandsToFile(net::FileStream* file,
                            const std::vector<SessionCommand*>& commands);

  const BaseSessionService::SessionType type_;

  
  FilePath GetLastSessionPath();

  
  FilePath GetCurrentSessionPath();

  
  const FilePath path_to_dir_;

  
  bool last_session_valid_;

  
  scoped_ptr<net::FileStream> current_session_file_;

  
  
  bool inited_;

  
  bool empty_file_;

  DISALLOW_COPY_AND_ASSIGN(SessionBackend);
};

#endif  
