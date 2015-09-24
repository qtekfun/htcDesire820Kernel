// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CONSTANTS_MAC_H_
#define REMOTING_HOST_CONSTANTS_MAC_H_

namespace remoting {

extern const char kServiceName[];

#define UPDATE_SUCCEEDED_NOTIFICATION_NAME \
    "org.chromium.chromoting.update_succeeded"
#define UPDATE_FAILED_NOTIFICATION_NAME "org.chromium.chromoting.update_failed"


extern const char kPrefPaneFileName[];
extern const char kPrefPaneFilePath[];

extern const char kHostConfigFileName[];
extern const char kHostConfigFilePath[];

extern const char kHostHelperScriptPath[];

extern const char kHostBinaryPath[];

extern const char kHostEnabledPath[];

extern const char kServicePlistPath[];

extern const char kLogFilePath[];

extern const char kLogFileConfigPath[];

extern const char kNativeMessagingManifestPath[];

extern const char kBrandedUninstallerPath[];
extern const char kUnbrandedUninstallerPath[];

}  

#endif  
