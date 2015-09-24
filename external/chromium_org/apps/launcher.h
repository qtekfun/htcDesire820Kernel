// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_LAUNCHER_H_
#define APPS_LAUNCHER_H_

#include <string>

class CommandLine;
class GURL;
class Profile;

namespace base {
class FilePath;
}

namespace extensions {
class Extension;
}

namespace apps {

void LaunchPlatformAppWithCommandLine(Profile* profile,
                                      const extensions::Extension* extension,
                                      const CommandLine* command_line,
                                      const base::FilePath& current_directory);

void LaunchPlatformAppWithPath(Profile* profile,
                               const extensions::Extension* extension,
                               const base::FilePath& file_path);

void LaunchPlatformApp(Profile* profile,
                       const extensions::Extension* extension);

void LaunchPlatformAppWithFileHandler(Profile* profile,
                                      const extensions::Extension* extension,
                                      const std::string& handler_id,
                                      const base::FilePath& file_path);

void LaunchPlatformAppWithUrl(Profile* profile,
                              const extensions::Extension* extension,
                              const std::string& handler_id,
                              const GURL& url,
                              const GURL& referrer_url);

void RestartPlatformApp(Profile* profile,
                        const extensions::Extension* extension);

}  

#endif  
